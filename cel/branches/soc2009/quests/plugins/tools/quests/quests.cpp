/*
    Crystal Space Entity Layer
    Copyright (C) 2004 by Jorrit Tyberghein

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include "cssysdef.h"
#include "csutil/objreg.h"
#include "csutil/dirtyaccessarray.h"
#include "csutil/util.h"
#include "csutil/xmltiny.h"
#include "csutil/scanstr.h"

#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/document.h"
#include "iutil/plugin.h"
#include "ivaria/reporter.h"

#include "physicallayer/persist.h"
#include "tools/parameters.h"

#include "plugins/tools/quests/quests.h"
#include "plugins/tools/quests/trig_entersector.h"
#include "plugins/tools/quests/trig_meshentersector.h"
#include "plugins/tools/quests/trig_timeout.h"
#include "plugins/tools/quests/trig_propertychange.h"
#include "plugins/tools/quests/trig_sequencefinish.h"
#include "plugins/tools/quests/trig_trigger.h"
#include "plugins/tools/quests/trig_message.h"
#include "plugins/tools/quests/trig_inventory.h"
#include "plugins/tools/quests/trig_meshsel.h"
#include "plugins/tools/quests/trig_watch.h"
#include "plugins/tools/quests/trig_operation.h"
#include "plugins/tools/quests/reward_debugprint.h"
#include "plugins/tools/quests/reward_newstate.h"
#include "plugins/tools/quests/reward_changeproperty.h"
#include "plugins/tools/quests/reward_inventory.h"
#include "plugins/tools/quests/reward_cssequence.h"
#include "plugins/tools/quests/reward_sequence.h"
#include "plugins/tools/quests/reward_sequencefinish.h"
#include "plugins/tools/quests/reward_message.h"
#include "plugins/tools/quests/reward_action.h"
#include "plugins/tools/quests/reward_createentity.h"
#include "plugins/tools/quests/reward_destroyentity.h"
#include "plugins/tools/quests/seqop_debugprint.h"
#include "plugins/tools/quests/seqop_transform.h"
#include "plugins/tools/quests/seqop_movepath.h"
#include "plugins/tools/quests/seqop_light.h"
#include "plugins/tools/quests/seqop_property.h"



//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celQuestManager)

//---------------------------------------------------------------------------

static const celData celDataNone;

static const char* ToString (csString& str, const celData* data)
{
  switch (data->type)
  {
    case CEL_DATA_STRING: return data->value.s->GetData ();
    case CEL_DATA_BOOL: if (data->value.bo) str = "true"; else str = "false"; break;
    case CEL_DATA_BYTE: str.Format ("%d", data->value.b); break;
    case CEL_DATA_UBYTE: str.Format ("%d", data->value.ub); break;
    case CEL_DATA_WORD: str.Format ("%d", data->value.w); break;
    case CEL_DATA_UWORD: str.Format ("%d", data->value.uw); break;
    case CEL_DATA_LONG: str.Format ("%d", data->value.l); break;
    case CEL_DATA_ULONG: str.Format ("%d", data->value.ul); break;
    case CEL_DATA_FLOAT: str.Format ("%g", data->value.f); break;
    case CEL_DATA_VECTOR2: str.Format ("%g,%g", data->value.v.x, data->value.v.y); break;
    case CEL_DATA_VECTOR3: str.Format ("%g,%g,%g",
				 data->value.v.x, data->value.v.y, data->value.v.z);
			     break;
    case CEL_DATA_VECTOR4: str.Format ("%g,%g,%g,%g", data->value.v.x, data->value.v.y,
				 data->value.v.z, data->value.v.w);
			     break;
    case CEL_DATA_COLOR4: str.Format ("%g,%g,%g,%g", data->value.col.red,
				data->value.col.green, data->value.col.blue, data->value.col.alpha);
			     break;
    case CEL_DATA_PCLASS: str.Format ("pc(%p)", (iCelPropertyClass*)data->value.pc); break;
    case CEL_DATA_IBASE: str.Format ("ibase(%p)", (iBase*)data->value.ibase); break;
    case CEL_DATA_ENTITY: str.Format ("ent('%s')", data->value.ent->GetName ()); break;
    case CEL_DATA_ACTION: str.Format ("action('%s')", data->value.s->GetData ()); break;
    case CEL_DATA_PARAMETER: str.Format ("par('%s')", data->value.par.parname->GetData ()); break;
    case CEL_DATA_NONE: str.Free (); break;
    default: str = "unknown()"; break;
  }
  return str;
}

static int32 ToLong (const celData* data)
{
  switch (data->type)
  {
    case CEL_DATA_STRING: return atol (data->value.s->GetData ());
    case CEL_DATA_BOOL: return data->value.bo;
    case CEL_DATA_BYTE: return data->value.b;
    case CEL_DATA_UBYTE: return data->value.ub;
    case CEL_DATA_WORD: return data->value.w;
    case CEL_DATA_UWORD: return data->value.uw;
    case CEL_DATA_LONG: return data->value.l;
    case CEL_DATA_ULONG: return data->value.ul;
    case CEL_DATA_FLOAT: return (int32) data->value.f;
    default: return 0;
  }
}

static float ToFloat (const celData* data)
{
  switch (data->type)
  {
    case CEL_DATA_STRING: return atof (data->value.s->GetData ());
    case CEL_DATA_BOOL: return (float)data->value.bo;
    case CEL_DATA_BYTE: return (float)data->value.b;
    case CEL_DATA_UBYTE: return (float)data->value.ub;
    case CEL_DATA_WORD: return (float)data->value.w;
    case CEL_DATA_UWORD: return (float)data->value.uw;
    case CEL_DATA_LONG: return (float)data->value.l;
    case CEL_DATA_ULONG: return (float)data->value.ul;
    case CEL_DATA_FLOAT: return data->value.f;
    default: return 0.0f;
  }
}

static bool ToBool (const celData* data)
{
  bool rc;
  switch (data->type)
  {
    case CEL_DATA_STRING: csScanStr (data->value.s->GetData (), "%b", &rc);
			  return rc;
    case CEL_DATA_BOOL: return data->value.bo;
    case CEL_DATA_BYTE: return data->value.b != 0;
    case CEL_DATA_UBYTE: return data->value.ub != 0;
    case CEL_DATA_WORD: return data->value.w != 0;
    case CEL_DATA_UWORD: return data->value.uw != 0;
    case CEL_DATA_LONG: return data->value.l != 0;
    case CEL_DATA_ULONG: return data->value.ul != 0;
    case CEL_DATA_FLOAT: return fabs (data->value.f) > 0.000001;
    case CEL_DATA_PCLASS: return data->value.pc != 0;
    case CEL_DATA_IBASE: return data->value.ibase != 0;
    case CEL_DATA_ENTITY: return data->value.ent != 0;
    default: return false;
  }
}

static csVector2 ToVector2 (const celData* data)
{
  csVector2 v;
  switch (data->type)
  {
    case CEL_DATA_STRING: csScanStr (data->value.s->GetData (), "%f,%f", &v.x, &v.y);
			  return v;
    case CEL_DATA_VECTOR2: v.x = data->value.v.x; v.y = data->value.v.y;
			   return v;
    default: v.x = v.y = 0.0f;
	     return v;
  }
}

static csVector3 ToVector3 (const celData* data)
{
  csVector3 v;
  switch (data->type)
  {
    case CEL_DATA_STRING: csScanStr (data->value.s->GetData (), "%f,%f,%f", &v.x, &v.y, &v.z);
			  return v;
    case CEL_DATA_VECTOR3: v.x = data->value.v.x; v.y = data->value.v.y; v.z = data->value.v.z;
			   return v;
    default: v.x = v.y = v.z = 0.0f;
	     return v;
  }
}

static csColor ToColor (const celData* data)
{
  csColor v;
  switch (data->type)
  {
    case CEL_DATA_STRING: csScanStr (data->value.s->GetData (), "%f,%f,%f",
			      &v.red, &v.green, &v.blue);
			  return v;
    case CEL_DATA_VECTOR2: v.red = data->value.col.red;
			   v.green = data->value.col.green;
			   v.blue = data->value.col.blue;
			   return v;
    default: v.red = v.green = v.blue = 0.0f;
	     return v;
  }
}

const char* celQuestConstantParameter::Get (iCelParameterBlock*)
{
  return ToString (str, &data);
}

int32 celQuestConstantParameter::GetLong (iCelParameterBlock*)
{
  return ToLong (&data);
}

const celData* celQuestDynamicParameter::GetData (iCelParameterBlock* params)
{
  if (!params)
  {
    csReport (object_reg,
	CS_REPORTER_SEVERITY_ERROR, "cel.questmanager.parameter",
	"Cannot resolve dynamic parameter '%s' (no parameters given)!",
	(const char*)parname);
    return &celDataNone;
  }

  const celData* data = params->GetParameter (dynamic_id);
  if (!data)
  {
    csReport (object_reg,
	CS_REPORTER_SEVERITY_ERROR, "cel.questmanager.parameter",
	"Cannot resolve dynamic parameter '%s'!", (const char*)parname);
    return &celDataNone;
  }
  return data;
}

const char* celQuestDynamicParameter::Get (iCelParameterBlock* params)
{
  const celData* data = GetData (params);
  return ToString (str, data);
}

int32 celQuestDynamicParameter::GetLong (iCelParameterBlock* params)
{
  const celData* data = GetData (params);
  return ToLong (data);
}

const char* celQuestDynamicParameter::Get (iCelParameterBlock* params,
    bool& changed)
{
  const char* s = Get (params);
  if (s == 0)
  {
    changed = !oldvalue.IsEmpty ();
    return s;
  }
  changed = oldvalue == s;
  oldvalue = s;
  return s;
}

//---------------------------------------------------------------------------

const celData* celQuestExpressionParameter::GetData (iCelParameterBlock* params)
{
  if (!expression->Execute (entity, data, params))
  {
    csReport (object_reg,
	CS_REPORTER_SEVERITY_ERROR, "cel.questmanager.parameter",
	"Cannot execute expression '%s'!", (const char*)parname);
    return &celDataNone;
  }
  return &data;
}

const char* celQuestExpressionParameter::Get (iCelParameterBlock* params)
{
  const celData* data = GetData (params);
  return ToString (str, data);
}

int32 celQuestExpressionParameter::GetLong (iCelParameterBlock* params)
{
  const celData* data = GetData (params);
  return ToLong (data);
}

const char* celQuestExpressionParameter::Get (iCelParameterBlock* params,
    bool& changed)
{
  const char* s = Get (params);
  if (s == 0)
  {
    changed = !oldvalue.IsEmpty ();
    return s;
  }
  changed = oldvalue == s;
  oldvalue = s;
  return s;
}

//---------------------------------------------------------------------------

void celQuestTriggerResponseFactory::SetTriggerFactory (
	iQuestTriggerFactory* trigger_fact)
{
  trigger_factory = trigger_fact;
}

void celQuestTriggerResponseFactory::AddRewardFactory (
	iQuestRewardFactory* reward_fact)
{
  reward_factories.Push (reward_fact);
}

void celQuestTriggerResponseFactory::AddRewardFactory_NEW (
	iRewardFactory* reward_fact)
{
  reward_factories_NEW.Push (reward_fact);
}

//---------------------------------------------------------------------------

celQuestStateFactory::celQuestStateFactory (const char* name) :
	scfImplementationType (this)
{
  celQuestStateFactory::name = name;
}

iQuestTriggerResponseFactory* celQuestStateFactory::
	CreateTriggerResponseFactory ()
{
  celQuestTriggerResponseFactory* resp = new celQuestTriggerResponseFactory ();
  responses.Push (resp);
  resp->DecRef ();
  return resp;
}

void celQuestStateFactory::AddInitRewardFactory (iQuestRewardFactory* reward_fact)
{
  oninit_reward_factories.Push (reward_fact);
}

void celQuestStateFactory::AddExitRewardFactory (iQuestRewardFactory* reward_fact)
{
  onexit_reward_factories.Push (reward_fact);
}

//---------------------------------------------------------------------------

celQuestSequence::celQuestSequence (const char* name,
	iCelPlLayer* pl, iVirtualClock* vc) : scfImplementationType (this)
{
  celQuestSequence::name = name;
  celQuestSequence::pl = pl;
  celQuestSequence::vc = vc;
  idx = csArrayItemNotFound;
}

celQuestSequence::~celQuestSequence ()
{
  Abort ();
}

void celQuestSequence::AddSeqOp (iQuestSeqOp* seqop, csTicks start, csTicks end)
{
  celSeqOp seq;
  seq.seqop = seqop;
  seq.start = start;
  seq.end = end;
  seq.idx = seqops.GetSize ();
  seqops.Push (seq);
}

bool celQuestSequence::Start (csTicks delay)
{
  if (IsRunning ()) return false;
  idx = 0;
  pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
  start_time = vc->GetCurrentTicks () + delay;
  return true;
}

void celQuestSequence::Finish ()
{
  if (!IsRunning ()) return;
  Perform (total_time+1);
}

void celQuestSequence::Abort ()
{
  if (!IsRunning ()) return;
  pl->RemoveCallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
  idx = csArrayItemNotFound;
  ops_in_progress.Empty ();
}

bool celQuestSequence::IsRunning ()
{
  return idx != csArrayItemNotFound;
}

void celQuestSequence::Perform (csTicks rel)
{
  // Find all operations that have to be performed.
  while (idx < seqops.GetSize () && rel >= seqops[idx].start)
  {
    seqops[idx].seqop->Init ();
    if (rel >= seqops[idx].end)
    {
      // Single shot operation or operation has already ended. Will not
      // be put in the progress array.
      seqops[idx].seqop->Do (1.0f);
    }
    else
    {
      ops_in_progress.Push (seqops[idx]);
    }
    idx++;
  }

  // Perform the operations that are still in progress.
  size_t i = 0;
  while (i < ops_in_progress.GetSize ())
  {
    if (rel >= ops_in_progress[i].end)
    {
      ops_in_progress[i].seqop->Do (1.0f);
      ops_in_progress.DeleteIndex (i);
    }
    else
    {
      float dt = float (rel - ops_in_progress[i].start)
      	/ float (ops_in_progress[i].end - ops_in_progress[i].start);
      ops_in_progress[i].seqop->Do (dt);
      i++;
    }
  }

  if (rel > total_time)
  {
    // Sequence has ended.
    FireSequenceCallbacks ();
    Abort ();
    return;
  }
}

void celQuestSequence::TickEveryFrame ()
{
  csTicks current_time = vc->GetCurrentTicks ();
  if (current_time < start_time) return;
  csTicks rel = current_time - start_time;
  Perform (rel);
}

void celQuestSequence::SaveState (iCelDataBuffer* databuf)
{
  databuf->Add ((uint32)(vc->GetCurrentTicks ()-start_time));

  // Save all operations that are still in progress.
  databuf->Add ((uint16)ops_in_progress.GetSize ());
  size_t i;
  for (i = 0 ; i < ops_in_progress.GetSize () ; i++)
  {
    databuf->Add ((uint32)ops_in_progress[i].idx);
    ops_in_progress[i].seqop->Save (databuf);
  }
}

bool celQuestSequence::LoadState (iCelDataBuffer* databuf)
{
  // First start the sequence.
  Start (0);

  csTicks current_time = vc->GetCurrentTicks ();
  start_time = current_time - databuf->GetUInt32 ();
  //csTicks rel = current_time - start_time;

  // When loading state it is important to realize that we assume
  // that the objects on which this sequence operates will load
  // their own state on their own. So we don't have to actually
  // perform the already performed operations again here. We just
  // have to setup the right datastructures so that we can resume
  // de sequence where we left off.

  uint16 cnt_op = databuf->GetUInt16 ();
  size_t i;
  idx = 0;
  for (i = 0 ; i < cnt_op ; i++)
  {
    uint32 id = databuf->GetUInt32 ();
    if (id > idx) idx = id;
    if (!seqops[id].seqop->Load (databuf))
      return false;
    ops_in_progress.Push (seqops[id]);
  }
#if 0
  // Find all operations that have to be performed.
  idx = 0;
  while (idx < seqops.GetSize () && rel >= seqops[idx].start)
  {
    if (rel < seqops[idx].end)
      ops_in_progress.Push (seqops[idx]);
    idx++;
  }
#endif

  return true;
}

void celQuestSequence::AddSequenceCallback (iQuestSequenceCallback* cb)
{
  callbacks.Push (cb);
}

void celQuestSequence::RemoveSequenceCallback (iQuestSequenceCallback* cb)
{
  callbacks.Delete (cb);
}

void celQuestSequence::FireSequenceCallbacks ()
{
  size_t i = callbacks.GetSize ();
  while (i > 0)
  {
    i--;
    callbacks[i]->SequenceFinished (this);
  }
}

celQuestSequenceFactory::celQuestSequenceFactory (const char* name,
	celQuestFactory* parent) : scfImplementationType (this)
{
  celQuestSequenceFactory::name = name;
  parent_factory = parent;
}

bool celQuestSequenceFactory::Load (iDocumentNode* node)
{
  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    csStringID id = parent_factory->xmltokens.Request (value);
    switch (id)
    {
      case celQuestFactory::XMLTOKEN_OP:
        {
	  csString type = child->GetAttributeValue ("type");
	  iQuestSeqOpType* seqoptype = parent_factory->GetQuestManager ()
	  	->GetSeqOpType ("cel.questseqop."+type);
	  if (!seqoptype)
	    seqoptype = parent_factory->GetQuestManager ()->GetSeqOpType (type);
	  if (!seqoptype)
	  {
            csReport (parent_factory->GetQuestManager ()->object_reg,
	    	CS_REPORTER_SEVERITY_ERROR, "cel.questmanager.load",
		"Unknown sequence type '%s' while loading quest '%s'!",
		(const char*)type, (const char*)name);
	    return false;
	  }
	  csRef<iQuestSeqOpFactory> seqopfact = seqoptype
		->CreateSeqOpFactory ();
	  if (!seqopfact->Load (child))
	    return false;
	  const char* duration = child->GetAttributeValue ("duration");
	  AddSeqOpFactory (seqopfact, duration);
	}
        break;
      case celQuestFactory::XMLTOKEN_DELAY:
        {
	  const char* time = child->GetAttributeValue ("time");
	  AddDelay (time);
	}
        break;

      default:
        csReport (parent_factory->GetQuestManager ()->object_reg,
		CS_REPORTER_SEVERITY_ERROR, "cel.questmanager.load",
		"Unknown token '%s' while loading sequence!",
		value);
        return false;
    }
  }
  return true;
}

void celQuestSequenceFactory::AddSeqOpFactory (iQuestSeqOpFactory* seqopfact,
  	const char* duration)
{
  celSeqOpFact s;
  s.seqop = seqopfact;
  s.duration = duration;
  seqops.Push (s);
}

void celQuestSequenceFactory::AddDelay (const char* delay)
{
  celSeqOpFact s;
  s.seqop = 0;
  s.duration = delay;
  seqops.Push (s);
}

static uint ToUInt (const char* s)
{
  if (!s) return 0;
  int f;
  sscanf (s, "%ud", &f);
  return f;
}

csPtr<celQuestSequence> celQuestSequenceFactory::CreateSequence (
	const celQuestParams& params)
{
  celQuestSequence* seq = new celQuestSequence (name,
  	parent_factory->GetQuestManager ()->pl,
	parent_factory->GetQuestManager ()->vc);
  size_t i;
  csTicks total_time = 0;
  csTicks max_time = 0;
  for (i = 0 ; i < seqops.GetSize () ; i++)
  {
    // @@@ Support dynamic parameters here?
    csTicks duration = ToUInt (parent_factory->GetQuestManager ()->
    	ResolveParameter (params, seqops[i].duration));
    if (total_time + duration > max_time) max_time = total_time + duration;
    if (seqops[i].seqop)
    {
      // It is not a delay.
      csRef<iQuestSeqOp> seqop = seqops[i].seqop->CreateSeqOp (params);
      seq->AddSeqOp (seqop, total_time, total_time+duration);
    }
    else
    {
      // A delay.
      total_time += duration;
    }
  }
  seq->SetTotalTime (max_time);
  return csPtr<celQuestSequence> (seq);
}

//---------------------------------------------------------------------------

celQuestFactory::celQuestFactory (celQuestManager* questmgr, const char* name) :
	scfImplementationType (this)
{
  celQuestFactory::questmgr = questmgr;
  celQuestFactory::name = name;
  InitTokenTable (xmltokens);
}

const char* celQuestFactory::GetDefaultParameter (const char* name) const
{
  return defaults.Get (name, (const char*)0);
}

void celQuestFactory::SetDefaultParameter (const char* name,const char* value)
{
  defaults.PutUnique (name,value);
}

void celQuestFactory::ClearDefaultParameters ()
{
  defaults.DeleteAll ();
}

csPtr<iQuest> celQuestFactory::CreateQuest (
      const celQuestParams& params,
	  const celParams& params_NEW)
{
  celQuest* q = new celQuest (questmgr->pl);
  // Set defaults
  const celQuestParams *p_params;
  celQuestParams result_params;

  const celParams *p_params_NEW;
  celParams result_params_NEW;

  if (params.GetSize() && defaults.GetSize())
  {
    result_params = params;
	result_params_NEW = params_NEW;
    celQuestParams::GlobalIterator def_it = defaults.GetIterator ();
    csStringBase it_key;
    const char* name;
    while (def_it.HasNext ())
    {
      name = def_it.Next (it_key);
      if (!params.Contains(it_key))
        result_params.PutUnique(it_key,name);
	    result_params_NEW.PutUnique(it_key,name);
    }
    p_params=&result_params;
	p_params_NEW=&result_params_NEW;
  }
  else if (defaults.GetSize())
    p_params = &defaults;
  else
    p_params = &params;

  // Set states
  celQuestFactoryStates::GlobalIterator sta_it = states.GetIterator ();
  while (sta_it.HasNext ())
  {
    celQuestStateFactory* sf = sta_it.Next ();
    const csRefArray<iQuestRewardFactory>& oninit_reward_Factories
        = sf->GetOninitRewardFactories ();
    const csRefArray<iQuestRewardFactory>& onexit_reward_Factories
        = sf->GetOnexitRewardFactories ();
    const csRefArray<celQuestTriggerResponseFactory>& responses
    	= sf->GetResponses ();
    size_t stateidx = q->AddState (sf->GetName ());
    size_t i;
    for (i = 0 ; i < oninit_reward_Factories.GetSize () ; i++)
    {
      csRef<iQuestReward> rew = oninit_reward_Factories[i]->CreateReward ((iQuest*)q,
	  *p_params);
      q->AddOninitReward (stateidx, rew);
    }
    for (i = 0 ; i < onexit_reward_Factories.GetSize () ; i++)
    {
      csRef<iQuestReward> rew = onexit_reward_Factories[i]->CreateReward ((iQuest*)q,
	  *p_params);
      q->AddOnexitReward (stateidx, rew);
    }
    for (i = 0 ; i < responses.GetSize () ; i++)
    {
      celQuestTriggerResponseFactory* respfact = responses[i];
      iQuestTriggerFactory* trigfact = respfact->GetTriggerFactory ();
      const csRefArray<iQuestRewardFactory>& rewfacts
        = respfact->GetRewardFactories ();

	  const csRefArray<iRewardFactory>& rewfacts_NEW
        = respfact->GetRewardFactories_NEW ();

      size_t respidx = q->AddStateResponse (stateidx);
      csRef<iQuestTrigger> trig = trigfact->CreateTrigger ((iQuest*)q,
      		*p_params);
      if (!trig) return 0;	// @@@ Report
      q->SetStateTrigger (stateidx, respidx, trig);
      size_t j;
      for (j = 0 ; j < rewfacts.GetSize () ; j++)
      {
        csRef<iQuestReward> rew = rewfacts[j]->CreateReward ((iQuest*)q,
		*p_params);

	    if (!rew) return 0;
        q->AddStateReward (stateidx, respidx, rew);
      }

      for (j = 0 ; j < rewfacts_NEW.GetSize () ; j++)
      {
	  	csRef<iReward> rew = rewfacts_NEW[j]->CreateReward (*p_params_NEW);

	    if (!rew) return 0;
        q->AddStateReward_NEW (stateidx, respidx, rew);
	  }
    }
  }

  // Set sequences
  celQuestFactorySequences::GlobalIterator seq_it = sequences.GetIterator ();
  while (seq_it.HasNext ())
  {
    celQuestSequenceFactory* sf = seq_it.Next ();
    csRef<celQuestSequence> seq = sf->CreateSequence (*p_params);
    q->AddSequence (seq);
  }

  return csPtr<iQuest> (q);
}

csRef<iQuestRewardFactory> celQuestFactory::LoadReward (iDocumentNode* child)
{
  csString type = child->GetAttributeValue ("type");
  iQuestRewardType* rewardtype = questmgr->GetRewardType ("cel.questreward."+type);
  if (!rewardtype)
    rewardtype = questmgr->GetRewardType (type);
  if (!rewardtype)
  {
    csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"Unknown reward type '%s' while loading quest '%s'!",
		(const char*)type, (const char*)name);
    return 0;
  }
  csRef<iQuestRewardFactory> rewardfact = rewardtype->CreateRewardFactory ();
  if (!rewardfact->Load (child))
    return 0;
  return rewardfact;
}

bool celQuestFactory::LoadRewards (
	iQuestStateFactory* statefact, bool oninit,
  	iDocumentNode* node)
{
  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    csStringID id = xmltokens.Request (value);
    switch (id)
    {
      case XMLTOKEN_REWARD:
        {
	  csRef<iQuestRewardFactory> rewardfact = LoadReward (child);
	  if (!rewardfact) return false;
	  if (oninit)
	    statefact->AddInitRewardFactory (rewardfact);
	  else
	    statefact->AddExitRewardFactory (rewardfact);
	}
        break;
      default:
        csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"Unknown token '%s' while loading '%s' in quest '%s'!",
		(const char*)value, oninit ? "oninit" : "onexit", (const char*)name);
        return false;
    }
  }
  return true;
}

bool celQuestFactory::LoadTriggerResponse (
	iQuestTriggerResponseFactory* respfact,
  	iQuestTriggerFactory* trigfact, iDocumentNode* node)
{
  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    csStringID id = xmltokens.Request (value);
    switch (id)
    {
      case XMLTOKEN_FIREON:
	if (!trigfact->Load (child))
	  return false;
        break;
      case XMLTOKEN_REWARD:
        {
	  csRef<iQuestRewardFactory> rewardfact = LoadReward (child);
	  if (!rewardfact) return false;
	  respfact->AddRewardFactory (rewardfact);
	}
        break;
      default:
        csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"Unknown token '%s' while loading trigger in quest '%s'!",
		(const char*)value, (const char*)name);
        return false;
    }
  }
  return true;
}

bool celQuestFactory::LoadState (iQuestStateFactory* statefact,
	iDocumentNode* node)
{
  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    csStringID id = xmltokens.Request (value);
    switch (id)
    {
      case XMLTOKEN_ONINIT:
	if (!LoadRewards (statefact, true, child))
	  return false;
	break;
      case XMLTOKEN_ONEXIT:
	if (!LoadRewards (statefact, false, child))
	  return false;
	break;
      case XMLTOKEN_TRIGGER:
        {
	  csString type = child->GetAttributeValue ("type");
	  iQuestTriggerType* triggertype = questmgr->GetTriggerType (
	  	"cel.questtrigger."+type);
	  if (!triggertype)
	    triggertype = questmgr->GetTriggerType (type);
	  if (!triggertype)
	  {
            csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"Unknown trigger type '%s' while loading state '%s/%s'!",
		(const char*)type, (const char*)name, statefact->GetName ());
	    return false;
	  }
	  // First we create a trigger response factory.
	  csRef<iQuestTriggerResponseFactory> respfact = statefact
	  	->CreateTriggerResponseFactory ();
	  // We create the actual trigger factory for that response factory.
	  csRef<iQuestTriggerFactory> triggerfact = triggertype
		->CreateTriggerFactory ();
	  respfact->SetTriggerFactory (triggerfact);
	  if (!LoadTriggerResponse (respfact, triggerfact, child))
	    return false;
	}
        break;
      default:
        csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"Unknown token '%s' while loading state '%s/%s'!",
		(const char*)value, (const char*)name, statefact->GetName ());
        return false;
    }
  }
  return true;
}

bool celQuestFactory::Load (iDocumentNode* node)
{
  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    csStringID id = xmltokens.Request (value);
    switch (id)
    {
      case celQuestFactory::XMLTOKEN_DEFAULT:
        {
	  const char* name = child->GetAttributeValue ("name");
	  if (!name)
	  {
            csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"'name' missing for default quest parameter!");
	    return false;
	  }
	  const char* value = child->GetAttributeValue ("value");
	  if (!value)
	  {
	    value = child->GetAttributeValue ("string");
	    if (!value)
	    {
              csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_WARNING,
		"cel.questmanager.load",
		"'string' is deprecated for quest default parameters. Use 'value'!");
	      return false;
	    }
	    else
	    {
              csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"'value' missing for default quest parameter!");
	    }
	  }
	  SetDefaultParameter (name,value);
	}
        break;
      case XMLTOKEN_STATE:
        {
	  const char* statename = child->GetAttributeValue ("name");
	  iQuestStateFactory* statefact = CreateState (statename);
	  if (!statefact)
	  {
            csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"Couldn't load state '%s' while loading quest '%s'!",
		(const char*)statename, (const char*)name);
	    return false;
	  }
	  if (!LoadState (statefact, child))
	    return false;
	}
        break;
      case XMLTOKEN_START:
        break;
      case XMLTOKEN_SEQUENCE:
        {
	  const char* seqname = child->GetAttributeValue ("name");
	  iQuestSequenceFactory* seqfact = CreateSequence (seqname);
	  if (!seqfact)
	  {
            csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"Couldn't load sequence '%s' while loading quest '%s'!",
		(const char*)seqname, (const char*)name);
	    return false;
	  }
	  if (!seqfact->Load (child))
	    return false;
	}
	break;
      default:
        csReport (questmgr->object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"Unknown token '%s' while loading quest '%s'!",
		value, (const char*)name);
        return false;
    }
  }
  return true;
}

iQuestStateFactory* celQuestFactory::GetState (const char* name)
{
  celQuestStateFactory* state = states.Get (name, 0);
  return (iQuestStateFactory*)state;
}

iQuestStateFactory* celQuestFactory::CreateState (const char* name)
{
  iQuestStateFactory* istate = GetState (name);
  if (istate) return 0;

  celQuestStateFactory* state = new celQuestStateFactory (name);
  states.Put (name, state);
  state->DecRef ();
  return state;
}

iQuestSequenceFactory* celQuestFactory::GetSequence (const char* name)
{
  celQuestSequenceFactory* seq = sequences.Get (name, 0);
  return (iQuestSequenceFactory*)seq;
}

iQuestSequenceFactory* celQuestFactory::CreateSequence (const char* name)
{
  iQuestSequenceFactory* iseq = GetSequence (name);
  if (iseq) return 0;

  celQuestSequenceFactory* seq = new celQuestSequenceFactory (name,
  	this);
  sequences.Put (name, seq);
  seq->DecRef ();
  return seq;
}

//---------------------------------------------------------------------------

celQuestStateResponse::celQuestStateResponse (iCelPlLayer* pl,
	celQuest* quest) : scfImplementationType (this)
{
  celQuestStateResponse::pl = pl;
  celQuestStateResponse::quest = quest;
}

void celQuestStateResponse::SetTrigger (iQuestTrigger* trigger)
{
  celQuestStateResponse::trigger = trigger;
  trigger->RegisterCallback (this);
}

void celQuestStateResponse::AddReward (iQuestReward* reward)
{
  rewards.Push (reward);
}

void celQuestStateResponse::AddReward_NEW (iReward* reward)
{
  rewards_NEW.Push (reward);
}

void celQuestStateResponse::TriggerFired (iQuestTrigger* trigger,
    iCelParameterBlock* params)
{
  printf("TRIGGER FIRED \n"); //NEW
  printf("SIZE = %i", rewards.GetSize ());

  size_t i;
  for (i = 0 ; i < rewards.GetSize () ; i++)
    rewards[i]->Reward (params);
  //return;

  printf("NEW SIZE = %i", rewards_NEW.GetSize ());
  //NEW
  size_t i2;
  for (i2 = 0 ; i2 < rewards_NEW.GetSize () ; i2++)
    //printf("A REFACTORED REWARD HAS BEEN TRIGGERED");
    rewards_NEW[i2]->Reward (params);
  return;
}

//---------------------------------------------------------------------------

size_t celQuestState::AddResponse (celQuest* quest)
{
  celQuestStateResponse* response = new celQuestStateResponse (pl, quest);
  size_t idx = responses.Push (response);
  response->DecRef ();
  return idx;
}

//---------------------------------------------------------------------------

celQuest::celQuest (iCelPlLayer* pl) : scfImplementationType (this)
{
  celQuest::pl = pl;
  current_state = csArrayItemNotFound;
}

celQuest::~celQuest ()
{
  DeactivateState (current_state, false);
}

void celQuest::DeactivateState (size_t stateidx, bool exec_onexit)
{
  if (stateidx == (size_t)-1) return;
  celQuestState* st = states[stateidx];
  size_t j;
  for (j = 0 ; j < st->GetResponseCount () ; j++)
    st->GetResponse (j)->GetTrigger ()->DeactivateTrigger ();
  if (exec_onexit)
    for (j = 0 ; j < st->GetOnexitRewardCount () ; j++)
      st->GetOnexitReward (j)->Reward (0);
}

bool celQuest::SwitchState (const char* state, iCelDataBuffer* databuf)
{
  // @@@ This code could be slow with really complex
  // quests that have lots of states. In practice most quests
  // will probably only have few states and will not switch
  // THAT often either.

  // Check if we are going to switch to the same state. In that case we don't
  // fire the oninit/onexit rewards.
  bool samestate = current_state != (size_t)-1 && strcmp (state, states[current_state]->GetName ()) == 0;

  size_t i, j;
  for (i = 0 ; i < states.GetSize () ; i++)
  {
    if (strcmp (state, states[i]->GetName ()) == 0)
    {
      DeactivateState (current_state, !samestate);
      current_state = i;
      celQuestState* st = states[current_state];
      for (j = 0 ; j < st->GetResponseCount () ; j++)
      {
        iQuestTrigger* trigger = st->GetResponse (j)->GetTrigger ();
	if (databuf)
	{
          if (!trigger->LoadAndActivateTrigger (databuf))
	    return false;	// @@@ Report?
	  if (trigger->Check ())
	    return true;
	}
	else
	{
          trigger->ActivateTrigger ();
	  if (trigger->Check ())
	    return true;
        }
      }
      if (!samestate)
        for (j = 0 ; j < st->GetOninitRewardCount () ; j++)
          st->GetOninitReward (j)->Reward (0);
      return true;
    }
  }
  return false;
}

bool celQuest::SwitchState (const char* state)
{
  return SwitchState (state, 0);
}

bool celQuest::LoadState (const char* state, iCelDataBuffer* databuf)
{
  bool rc = SwitchState (state, databuf);
  if (!rc) return false;

  iString* seqname = databuf->GetString ();
  while (!seqname->IsEmpty ())
  {
    celQuestSequence* seq = FindCelSequence (seqname->GetData ());
    if (!seq)
    {
      //@@@
      //csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
		//"cel.questmanager.load",
		//"Error finding sequence '%s'!", seqname->GetData ());
      return false;
    }
    if (!seq->LoadState (databuf))
      return false;

    seqname = databuf->GetString ();
  }

  return true;
}

void celQuest::SaveState (iCelDataBuffer* databuf)
{
  size_t i;
  if (current_state != csArrayItemNotFound)
  {
    celQuestState* st = states[current_state];
    for (i = 0 ; i < st->GetResponseCount () ; i++)
      st->GetResponse (i)->GetTrigger ()->SaveTriggerState (databuf);
  }

  for (i = 0 ; i < sequences.GetSize () ; i++)
    if (sequences[i]->IsRunning ())
    {
      databuf->Add (sequences[i]->GetName ());
      sequences[i]->SaveState (databuf);
    }
  databuf->Add ((const char*)0);
}

const char* celQuest::GetCurrentState () const
{
  if (current_state == csArrayItemNotFound) return 0;
  return states[current_state]->GetName ();
}

size_t celQuest::AddState (const char* name)
{
  return states.Push (new celQuestState (pl, name));
}

size_t celQuest::AddStateResponse (size_t stateidx)
{
  return states[stateidx]->AddResponse (this);
}

void celQuest::SetStateTrigger (size_t stateidx, size_t responseidx,
	iQuestTrigger* trigger)
{
  states[stateidx]->GetResponse (responseidx)->SetTrigger (trigger);
}

void celQuest::AddStateReward (size_t stateidx, size_t responseidx,
	iQuestReward* reward)
{
  states[stateidx]->GetResponse (responseidx)->AddReward (reward);
}

void celQuest::AddStateReward_NEW (size_t stateidx, size_t responseidx,
	iReward* reward)
{
  states[stateidx]->GetResponse (responseidx)->AddReward_NEW (reward);
}

void celQuest::AddOninitReward (size_t stateidx, iQuestReward* reward)
{
  states[stateidx]->AddOninitReward (reward);
}

void celQuest::AddOnexitReward (size_t stateidx, iQuestReward* reward)
{
  states[stateidx]->AddOnexitReward (reward);
}

void celQuest::AddSequence (celQuestSequence* sequence)
{
  sequences.Push (sequence);
}

celQuestSequence* celQuest::FindCelSequence (const char* name)
{
  size_t i;
  for (i = 0 ; i < sequences.GetSize () ; i++)
    if (!strcmp (name, sequences[i]->GetName ()))
      return sequences[i];
  return 0;
}

//---------------------------------------------------------------------------

celQuestManager::celQuestManager (iBase* parent) : scfImplementationType (this,
	parent)
{
}

celQuestManager::~celQuestManager ()
{
}

bool celQuestManager::Initialize (iObjectRegistry* object_reg)
{
  celQuestManager::object_reg = object_reg;
  pl = csQueryRegistry<iCelPlLayer> (object_reg);
  vc = csQueryRegistry<iVirtualClock> (object_reg);

  //--- Triggers -----------------------------------------------------
  {
    celPropertyChangeTriggerType* type = new celPropertyChangeTriggerType (
  	object_reg);
    RegisterTriggerType (type);
    type->DecRef ();
  }

  {
    celMeshSelectTriggerType* type = new celMeshSelectTriggerType (
  	object_reg);
    RegisterTriggerType (type);
    type->DecRef ();
  }

  {
    celInventoryTriggerType* type = new celInventoryTriggerType (
  	object_reg);
    RegisterTriggerType (type);
    type->DecRef ();
  }

  {
    celTimeoutTriggerType* type = new celTimeoutTriggerType (
  	object_reg);
    RegisterTriggerType (type);
    type->DecRef ();
  }

  {
    celEnterSectorTriggerType* type = new celEnterSectorTriggerType (
  	object_reg);
    RegisterTriggerType (type);
    type->DecRef ();
  }

  {
    celMeshEnterSectorTriggerType* type = new celMeshEnterSectorTriggerType (
  	object_reg);
    RegisterTriggerType (type);
    type->DecRef ();
  }

  {
    celSequenceFinishTriggerType* type = new celSequenceFinishTriggerType (
  	object_reg);
    RegisterTriggerType (type);
    type->DecRef ();
  }

  {
    celTriggerTriggerType* type = new celTriggerTriggerType (
  	object_reg);
    RegisterTriggerType (type);
    type->DecRef ();
  }

  {
    celWatchTriggerType* type = new celWatchTriggerType (
  	object_reg);
    RegisterTriggerType (type);
    type->DecRef ();
  }

  {
    celMessageTriggerType* type = new celMessageTriggerType (
  	object_reg);
    RegisterTriggerType (type);
    type->DecRef ();
  }

  {
    celOperationTriggerType* type = new celOperationTriggerType (
  	object_reg);
    RegisterTriggerType (type);
    type->DecRef ();
  }

  //--- Rewards ------------------------------------------------------
  {
    celDebugPrintRewardType* type = new celDebugPrintRewardType (
    	object_reg);
    	
    RegisterRewardType (type);
    type->DecRef ();
  }
    	
  {
    csRef<iPluginManager> plugin_mgr = 
      csQueryRegistry<iPluginManager> (object_reg);
    csRef<iRewardType> type = csLoadPlugin<iRewardType> (plugin_mgr,
      "cel.rewards.debugprint");        
    if (type.IsValid())
    {
      RegisterRewardType_NEW (type);
      type->DecRef ();
    }
  }

  {
    celNewStateRewardType* type = new celNewStateRewardType (
    	object_reg);
    RegisterRewardType (type);
    type->DecRef ();
  }

  {
    celChangePropertyRewardType* type = new celChangePropertyRewardType (
    	object_reg);
    RegisterRewardType (type);
    type->DecRef ();
  }

  {
    csRef<iPluginManager> plugin_mgr = 
      csQueryRegistry<iPluginManager> (object_reg);
    csRef<iRewardType> type = csLoadPlugin<iRewardType> (plugin_mgr,
      "cel.rewards.changeproperty");        
    if (type.IsValid())
    {
      RegisterRewardType_NEW (type);
      type->DecRef ();
    }
  }

  {
    celInventoryRewardType* type = new celInventoryRewardType (
    	object_reg);
    RegisterRewardType (type);
    type->DecRef ();
  }

  {
    csRef<iPluginManager> plugin_mgr = 
      csQueryRegistry<iPluginManager> (object_reg);
    csRef<iRewardType> type = csLoadPlugin<iRewardType> (plugin_mgr,
      "cel.rewards.inventory");        
    if (type.IsValid())
    {
      RegisterRewardType_NEW (type);
      type->DecRef ();
    }
  }

  {
    celCsSequenceRewardType* type = new celCsSequenceRewardType (
    	object_reg);
    RegisterRewardType (type);
    type->DecRef ();
  }

  {
    celSequenceRewardType* type = new celSequenceRewardType (
    	object_reg);
    RegisterRewardType (type);
    type->DecRef ();
  }

  {
    celSequenceFinishRewardType* type = new celSequenceFinishRewardType (
    	object_reg);
    RegisterRewardType (type);
    type->DecRef ();
  }

  {
    celMessageRewardType* type = new celMessageRewardType (
    	object_reg);
    RegisterRewardType (type);
    type->DecRef ();
  }

  {
    csRef<iPluginManager> plugin_mgr = 
      csQueryRegistry<iPluginManager> (object_reg);
    csRef<iRewardType> type = csLoadPlugin<iRewardType> (plugin_mgr,
      "cel.rewards.message");        
    if (type.IsValid())
    {
      RegisterRewardType_NEW (type);
      type->DecRef ();
    }
  }

  {
    celActionRewardType* type = new celActionRewardType (
    	object_reg);
    RegisterRewardType (type);
    type->DecRef ();
  }

  {
    csRef<iPluginManager> plugin_mgr = 
      csQueryRegistry<iPluginManager> (object_reg);
    csRef<iRewardType> type = csLoadPlugin<iRewardType> (plugin_mgr,
      "cel.rewards.action");        
    if (type.IsValid())
    {
      RegisterRewardType_NEW (type);
      type->DecRef ();
    }
  }

  {
    celCreateEntityRewardType* type = new celCreateEntityRewardType (
    	object_reg);
    RegisterRewardType (type);
    type->DecRef ();
  }

  {
    csRef<iPluginManager> plugin_mgr = 
      csQueryRegistry<iPluginManager> (object_reg);
    csRef<iRewardType> type = csLoadPlugin<iRewardType> (plugin_mgr,
      "cel.rewards.createentity");        
    if (type.IsValid())
    {
      RegisterRewardType_NEW (type);
      type->DecRef ();
    }
  }

  {
    celDestroyEntityRewardType* type = new celDestroyEntityRewardType (
    	object_reg);
    RegisterRewardType (type);
    type->DecRef ();
  }
  {
    csRef<iPluginManager> plugin_mgr = 
      csQueryRegistry<iPluginManager> (object_reg);
    csRef<iRewardType> type = csLoadPlugin<iRewardType> (plugin_mgr,
      "cel.rewards.destroyentity");        
    if (type.IsValid())
    {
      RegisterRewardType_NEW (type);
      type->DecRef ();
    }
  }
  //--- Sequence Operations ------------------------------------------
  {
    celDebugPrintSeqOpType* type = new celDebugPrintSeqOpType (
    	object_reg);
    RegisterSeqOpType (type);
    type->DecRef ();
  }

  {
    celTransformSeqOpType* type = new celTransformSeqOpType (
    	object_reg);
    RegisterSeqOpType (type);
    type->DecRef ();
  }

  {
    celMovePathSeqOpType* type = new celMovePathSeqOpType (
    	object_reg);
    RegisterSeqOpType (type);
    type->DecRef ();
  }

  {
    celLightSeqOpType* type = new celLightSeqOpType (
    	object_reg);
    RegisterSeqOpType (type);
    type->DecRef ();
  }

  {
    celPropertySeqOpType* type = new celPropertySeqOpType (
    	object_reg);
    RegisterSeqOpType (type);
    type->DecRef ();
  }

  return true;
}

iQuestFactory* celQuestManager::GetQuestFactory (const char* name)
{
  celQuestFactory* fact = quest_factories.Get (name, 0);
  return (iQuestFactory*)fact;
}

void celQuestManager::RemoveQuestFactories ()
{
  quest_factories.DeleteAll();
}

void celQuestManager::RemoveQuestFactory (const char* name)
{
  celQuestFactory* fact = quest_factories.Get (name, 0);
  if (fact)
    quest_factories.Delete(name,fact);
}


iQuestFactory* celQuestManager::CreateQuestFactory (const char* name)
{
  iQuestFactory* ifact = GetQuestFactory (name);
  if (ifact) return 0;

  celQuestFactory* fact = new celQuestFactory (this, name);
  quest_factories.Put (name, fact);
  fact->DecRef ();
  return fact;
}

bool celQuestManager::RegisterTriggerType (iQuestTriggerType* trigger)
{
  const char* name = trigger->GetName ();
  if (trigger_types.Get (name, 0) != 0)
    return false;
  trigger_types.Put (name, trigger);
  return true;
}

iQuestTriggerType* celQuestManager::GetTriggerType (const char* name)
{
  return trigger_types.Get (name, 0);
}

bool celQuestManager::RegisterRewardType (iQuestRewardType* reward)
{
  const char* name = reward->GetName ();
  if (reward_types.Get (name, 0) != 0)
    return false;
  reward_types.Put (name, reward);
  return true;
}

bool celQuestManager::RegisterRewardType_NEW (iRewardType* reward)
{
  const char* name = reward->GetName ();
  if (reward_types_NEW.Get (name, 0) != 0)
    return false;
  reward_types_NEW.Put (name, reward);
  return true;
}

iQuestRewardType* celQuestManager::GetRewardType (const char* name)
{
  return reward_types.Get (name, 0);
}

iRewardType* celQuestManager::GetRewardType_NEW (const char* name)
{
  return reward_types_NEW.Get (name, 0);
}

bool celQuestManager::RegisterSeqOpType (iQuestSeqOpType* seqop)
{
  const char* name = seqop->GetName ();
  if (seqop_types.Get (name, 0) != 0)
    return false;
  seqop_types.Put (name, seqop);
  return true;
}

iQuestSeqOpType* celQuestManager::GetSeqOpType (const char* name)
{
  return seqop_types.Get (name, 0);
}

iCelExpressionParser* celQuestManager::GetParser ()
{
  csRef<iObjectRegistryIterator> it = object_reg->Get (
      scfInterfaceTraits<iCelExpressionParser>::GetID (),
      scfInterfaceTraits<iCelExpressionParser>::GetVersion ());
  iBase* b = it->Next ();
  if (b)
  {
    expparser = scfQueryInterface<iCelExpressionParser> (b);
  }
  if (!expparser)
  {
    csRef<iPluginManager> plugmgr = csQueryRegistry<iPluginManager> (
	object_reg);
    expparser = csLoadPlugin<iCelExpressionParser> (plugmgr,
      "cel.behaviourlayer.xml");
    if (!expparser)

    {
      csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
		"cel.questmanager",
		"Can't find the expression parser!");
      return 0;
    }
    object_reg->Register (expparser, "iCelExpressionParser");
  }
  return expparser;
}

csPtr<iQuestParameter> celQuestManager::GetParameter (
  	const celQuestParams& params,
	const char* param)
{
  const char* val = ResolveParameter (params, param);
  if (val == 0) return new celQuestConstantParameter ();
  if (*val == '@' && *(val+1) != '@')
  {
    csString fullname = "cel.parameter.";
    fullname += val+1;
    csStringID dynamic_id = pl->FetchStringID (fullname);
    return new celQuestDynamicParameter (object_reg, dynamic_id, val+1);
  }
  else if (*val == '=' && *(val+1) != '=')
  {
    csRef<iCelExpression> expression = GetParser ()->Parse (val+1);
    if (!expression)
    {
      csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
		"cel.questmanager",
		"Can't parse expression '%s'!", val+1);
      return 0;
    }
    // We are looking for 'this' in the parameter block. If we can find it
    // then it indicates the name of the entity. We will find the entity for
    // the expression so that the expression can show things local to the
    // entity (or access properties from the current entity).
    celQuestParams::ConstGlobalIterator def_it = params.GetIterator ();
    csStringBase it_key;
    iCelEntity* entity = 0;
    while (def_it.HasNext ())
    {
      const char* name = def_it.Next (it_key);
      if (it_key == "this")
      {
	entity = pl->FindEntity (name);
	break;
      }
    }
    return new celQuestExpressionParameter (object_reg, entity, expression, val+1);
  }
  return new celQuestConstantParameter (val);
}

const char* celQuestManager::ResolveParameter (
  	const celQuestParams& params,
	const char* param)
{
  if (param == 0) return param;
  if (*param != '$') return param;
  if (*(param+1) == '$') return param+1;	// Double $ means to quote the '$'.
  const char* val = params.Get (param+1, (const char*)0);
  if (!val)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
		"cel.questmanager",
		"Can't resolve parameter %s!", param);
  }
  return val;
}

csPtr<celVariableParameterBlock> celQuestManager::GetParameterBlock (
  	const celQuestParams& params,
	const csArray<celParSpec>& parameters,
	csRefArray<iQuestParameter>& quest_parameters)
{
  celVariableParameterBlock *act_params = new celVariableParameterBlock ();
  size_t i;
  for (i = 0 ; i < parameters.GetSize () ; i++)
  {
    csRef<iQuestParameter> par = GetParameter (params, parameters[i].value);
    quest_parameters.Put (i, par);
    act_params->SetParameterDef (i, parameters[i].id, parameters[i].name);
  }
  return act_params;
}

void celQuestManager::FillParameterBlock (
        iCelParameterBlock* params,
	celVariableParameterBlock* act_params,
	const csArray<celParSpec>& parameters,
	const csRefArray<iQuestParameter>& quest_parameters)
{
  size_t i;
  for (i = 0 ; i < quest_parameters.GetSize () ; i++)
  {
    iQuestParameter* p = quest_parameters[i];
    switch (parameters[i].type)
    {
      case CEL_DATA_STRING:
	act_params->GetParameter (i).Set (p->Get (params));
	break;
      case CEL_DATA_LONG:
	act_params->GetParameter (i).Set (p->GetLong (params));
	break;
      case CEL_DATA_FLOAT:
	act_params->GetParameter (i).Set (ToFloat (p->GetData (params)));
	break;
      case CEL_DATA_BOOL:
	act_params->GetParameter (i).Set (ToBool (p->GetData (params)));
	break;
      case CEL_DATA_VECTOR2:
	act_params->GetParameter (i).Set (ToVector2 (p->GetData (params)));
	break;
      case CEL_DATA_VECTOR3:
	act_params->GetParameter (i).Set (ToVector3 (p->GetData (params)));
	break;
      case CEL_DATA_COLOR:
	act_params->GetParameter (i).Set (ToColor (p->GetData (params)));
	break;
      default:
	//@@@?
	break;
    }
  }
}

bool celQuestManager::Load (iDocumentNode* node)
{
  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    if (strcmp ("quest", value) == 0)
    {
      const char* filename = child->GetAttributeValue ("file");
      csRef<iDocumentNode> questnode;
      csRef<iDocument> doc;
      if (filename)
      {
        // Load quest from a file.
	csRef<iDocumentSystem> xml = 
		csQueryRegistry<iDocumentSystem> (object_reg);
	if (!xml)
	  xml.AttachNew (new csTinyDocumentSystem ());
	doc = xml->CreateDocument ();
	const char* error = doc->Parse (filename, true);
	if (error != 0)
	{
	  csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
		"cel.questmanager",
		"Can't open file '%s': %s!", filename, error);
        }
	questnode = doc->GetRoot ()->GetNode ("quest");
      }
      else
        questnode = child;

      const char* questname = questnode->GetAttributeValue ("name");
      iQuestFactory* questfact = CreateQuestFactory (questname);
      // It is possible that questfact is 0. That means that the factory
      // already existed.
      if (questfact)
        if (!questfact->Load (questnode))
          return false;
    }
    else
    {
      csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
		"cel.questmanager.load",
		"Unknown token '%s' while loading quests!",
		value);
      return false;
    }
  }
  return true;
}

iQuestRewardFactory* celQuestManager::AddNewStateReward (
	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* state_par)
{
  printf("ADD NEW STATE REWARD\n");
  iQuestRewardType* type = GetRewardType ("cel.questreward.newstate");
  csRef<iQuestRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iNewStateQuestRewardFactory> newstate = scfQueryInterface<iNewStateQuestRewardFactory> (rewfact);
  newstate->SetStateParameter (state_par);
  newstate->SetEntityParameter (entity_par);
  response->AddRewardFactory (rewfact);
  return rewfact;
}

iQuestRewardFactory* celQuestManager::AddDebugPrintReward (
	iQuestTriggerResponseFactory* response,
  	const char* msg_par)
{
  iQuestRewardType* type = GetRewardType ("cel.questreward.debugprint");
  csRef<iQuestRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iDebugPrintQuestRewardFactory> newstate = scfQueryInterface<iDebugPrintQuestRewardFactory> (rewfact);
  newstate->SetMessageParameter (msg_par);
  response->AddRewardFactory (rewfact);
  return rewfact;
}

iRewardFactory* celQuestManager::AddDebugPrintReward_NEW (
	iQuestTriggerResponseFactory* response,
  	const char* msg_par)
{
  iRewardType* type = GetRewardType_NEW ("cel.rewards.debugprint");
  csRef<iRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iDebugPrintRewardFactory> newstate = scfQueryInterface<iDebugPrintRewardFactory> (rewfact);
  newstate->SetMessageParameter (msg_par);
  response->AddRewardFactory_NEW (rewfact);
  return rewfact;
}


iQuestRewardFactory* celQuestManager::AddInventoryReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* child_entity_par)
{
  iQuestRewardType* type = GetRewardType ("cel.questreward.inventory");
  csRef<iQuestRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iInventoryQuestRewardFactory> newstate = scfQueryInterface<iInventoryQuestRewardFactory> (rewfact);
  newstate->SetEntityParameter (entity_par);
  newstate->SetChildEntityParameter (child_entity_par);
  response->AddRewardFactory (rewfact);
  return rewfact;
}

iRewardFactory* celQuestManager::AddInventoryReward_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* child_entity_par)
{
  iRewardType* type = GetRewardType_NEW ("cel.rewards.inventory");
  csRef<iRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iInventoryRewardFactory> newstate = scfQueryInterface<iInventoryRewardFactory> (rewfact);
  newstate->SetEntityParameter (entity_par);
  newstate->SetChildEntityParameter (child_entity_par);
  response->AddRewardFactory_NEW (rewfact);
  return rewfact;
}

iQuestRewardFactory* celQuestManager::AddSequenceReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sequence_par,
	const char* delay_par)
{
  iQuestRewardType* type = GetRewardType ("cel.questreward.sequence");
  csRef<iQuestRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iSequenceQuestRewardFactory> newstate = scfQueryInterface<iSequenceQuestRewardFactory> (rewfact);
  newstate->SetEntityParameter (entity_par);
  newstate->SetSequenceParameter (sequence_par);
  newstate->SetDelayParameter (delay_par);
  response->AddRewardFactory (rewfact);
  return rewfact;
}

iQuestRewardFactory* celQuestManager::AddCsSequenceReward (
  	iQuestTriggerResponseFactory* response,
  	const char* sequence_par, const char* delay_par)
{
  iQuestRewardType* type = GetRewardType ("cel.questreward.cssequence");
  csRef<iQuestRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iCsSequenceQuestRewardFactory> newstate = scfQueryInterface<iCsSequenceQuestRewardFactory> (rewfact);
  newstate->SetSequenceParameter (sequence_par);
  newstate->SetDelayParameter (delay_par);
  response->AddRewardFactory (rewfact);
  return rewfact;
}

iQuestRewardFactory* celQuestManager::AddSequenceFinishReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sequence_par)
{
  iQuestRewardType* type = GetRewardType ("cel.questreward.sequencefinish");
  csRef<iQuestRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iSequenceFinishQuestRewardFactory> newstate = 
  	scfQueryInterface<iSequenceFinishQuestRewardFactory> (rewfact);
  newstate->SetEntityParameter (entity_par);
  newstate->SetSequenceParameter (sequence_par);
  response->AddRewardFactory (rewfact);
  return rewfact;
}

iChangePropertyQuestRewardFactory* celQuestManager::AddChangePropertyReward (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* prop_par)
{
  iQuestRewardType* type = GetRewardType ("cel.questreward.changeproperty");
  csRef<iQuestRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iChangePropertyQuestRewardFactory> newstate = 
  	scfQueryInterface<iChangePropertyQuestRewardFactory> (rewfact);
  newstate->SetEntityParameter (entity_par);
  newstate->SetPropertyParameter (prop_par);
  response->AddRewardFactory (rewfact);
  return newstate;
}

iChangePropertyRewardFactory* celQuestManager::AddChangePropertyReward_NEW (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* prop_par)
{
  iRewardType* type = GetRewardType_NEW ("cel.rewards.changeproperty");
  csRef<iRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iChangePropertyRewardFactory> newstate = 
  	scfQueryInterface<iChangePropertyRewardFactory> (rewfact);
  newstate->SetEntityParameter (entity_par);
  newstate->SetPropertyParameter (prop_par);
  response->AddRewardFactory_NEW (rewfact);
  return newstate;
}

iQuestRewardFactory* celQuestManager::AddCreateEntityReward (
  	iQuestTriggerResponseFactory* response,
	const char* template_par,
	const char* name_par,
    const celEntityTemplateParams &tpl_params)
{
  iQuestRewardType* type = GetRewardType ("cel.questreward.createentity");
  csRef<iQuestRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iCreateEntityQuestRewardFactory> newstate = scfQueryInterface<iCreateEntityQuestRewardFactory> (rewfact);
  newstate->SetEntityTemplateParameter (template_par);
  newstate->SetNameParameter (name_par);

  celEntityTemplateParams::ConstGlobalIterator iter = tpl_params.GetIterator();
  while (iter.HasNext())
  {
    csStringFast<12> name;
    // @@@ Support dynamic parameters?
    const char * val = iter.Next (name);
	newstate->AddParameter (name, val);
  }

  response->AddRewardFactory (rewfact);
  return rewfact;
}

iRewardFactory* celQuestManager::AddCreateEntityReward_NEW (
  	iQuestTriggerResponseFactory* response,
	const char* template_par,
	const char* name_par,
    const celEntityTemplateParams &tpl_params)
{
  iRewardType* type = GetRewardType_NEW ("cel.rewards.createentity");
  csRef<iRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iCreateEntityRewardFactory> newstate = scfQueryInterface<iCreateEntityRewardFactory> (rewfact);
  newstate->SetEntityTemplateParameter (template_par);
  newstate->SetNameParameter (name_par);

  celEntityTemplateParams::ConstGlobalIterator iter = tpl_params.GetIterator();
  while (iter.HasNext())
  {
    csStringFast<12> name;
    // @@@ Support dynamic parameters?
    const char * val = iter.Next (name);
	newstate->AddParameter (name, val);
  }

  response->AddRewardFactory_NEW (rewfact);
  return rewfact;
}

iQuestRewardFactory* celQuestManager::AddDestroyEntityReward (
  	iQuestTriggerResponseFactory* response,
	const char* entity_par)
{
  iQuestRewardType* type = GetRewardType ("cel.questreward.destroyentity");
  csRef<iQuestRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iDestroyEntityQuestRewardFactory> newstate = 
  	scfQueryInterface<iDestroyEntityQuestRewardFactory> (rewfact);
  newstate->SetEntityParameter (entity_par);
  response->AddRewardFactory (rewfact);
  return rewfact;
}

iRewardFactory* celQuestManager::AddDestroyEntityReward_NEW (
  	iQuestTriggerResponseFactory* response,
	const char* entity_par)
{
  iRewardType* type = GetRewardType_NEW ("cel.rewards.destroyentity");
  csRef<iRewardFactory> rewfact = type->CreateRewardFactory ();
  csRef<iDestroyEntityRewardFactory> newstate = 
  	scfQueryInterface<iDestroyEntityRewardFactory> (rewfact);
  newstate->SetEntityParameter (entity_par);
  response->AddRewardFactory_NEW (rewfact);
  return rewfact;
}

iQuestTriggerFactory* celQuestManager::SetTimeoutTrigger (
	iQuestTriggerResponseFactory* response,
  	const char* timeout_par)
{
  iQuestTriggerType* type = GetTriggerType ("cel.questtrigger.timeout");
  csRef<iQuestTriggerFactory> trigfact = type->CreateTriggerFactory ();
  csRef<iTimeoutQuestTriggerFactory> newstate = scfQueryInterface<iTimeoutQuestTriggerFactory> (trigfact);
  newstate->SetTimeoutParameter (timeout_par);
  response->SetTriggerFactory (trigfact);
  return trigfact;
}

iQuestTriggerFactory* celQuestManager::SetEnterSectorTrigger (
	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sector_par)
{
  iQuestTriggerType* type = GetTriggerType ("cel.questtrigger.entersector");
  csRef<iQuestTriggerFactory> trigfact = type->CreateTriggerFactory ();
  csRef<iEnterSectorQuestTriggerFactory> newstate = 
  	scfQueryInterface<iEnterSectorQuestTriggerFactory> (trigfact);
  newstate->SetEntityParameter (entity_par);
  newstate->SetSectorParameter (sector_par);
  response->SetTriggerFactory (trigfact);
  return trigfact;
}

iQuestTriggerFactory* celQuestManager::SetMeshEnterSectorTrigger (
	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sector_par)
{
  iQuestTriggerType* type = GetTriggerType ("cel.questtrigger.meshentersector");
  csRef<iQuestTriggerFactory> trigfact = type->CreateTriggerFactory ();
  csRef<iEnterSectorQuestTriggerFactory> newstate = 
  	scfQueryInterface<iEnterSectorQuestTriggerFactory> (trigfact);
  newstate->SetEntityParameter (entity_par);
  newstate->SetSectorParameter (sector_par);
  response->SetTriggerFactory (trigfact);
  return trigfact;
}

iQuestTriggerFactory* celQuestManager::SetSequenceFinishTrigger (
	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* sequence_par)
{
  iQuestTriggerType* type = GetTriggerType ("cel.questtrigger.sequencefinish");
  csRef<iQuestTriggerFactory> trigfact = type->CreateTriggerFactory ();
  csRef<iSequenceFinishQuestTriggerFactory> newstate = 
  	scfQueryInterface<iSequenceFinishQuestTriggerFactory> (trigfact);
  newstate->SetEntityParameter (entity_par);
  newstate->SetSequenceParameter (sequence_par);
  return trigfact;
}

iQuestTriggerFactory* celQuestManager::SetPropertyChangeTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* prop_par, const char* value_par)
{
  iQuestTriggerType* type = GetTriggerType ("cel.questtrigger.propertychange");
  csRef<iQuestTriggerFactory> trigfact = type->CreateTriggerFactory ();
  csRef<iPropertyChangeQuestTriggerFactory> newstate = 
  	scfQueryInterface<iPropertyChangeQuestTriggerFactory> (trigfact);
  newstate->SetEntityParameter (entity_par);
  newstate->SetPropertyParameter (prop_par);
  newstate->SetValueParameter (value_par);
  response->SetTriggerFactory (trigfact);
  return trigfact;
}

iQuestTriggerFactory* celQuestManager::SetTriggerTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, bool do_leave)
{
  iQuestTriggerType* type = GetTriggerType ("cel.questtrigger.trigger");
  csRef<iQuestTriggerFactory> trigfact = type->CreateTriggerFactory ();
  csRef<iTriggerQuestTriggerFactory> newstate = 
  	scfQueryInterface<iTriggerQuestTriggerFactory> (trigfact);
  newstate->SetEntityParameter (entity_par);
  if (do_leave) newstate->EnableLeave ();
  response->SetTriggerFactory (trigfact);
  return trigfact;
}

iQuestTriggerFactory* celQuestManager::SetWatchTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* entity_par, const char* target_entity_par,
	const char* checktime_par,
	const char* radius_par)
{
  iQuestTriggerType* type = GetTriggerType ("cel.questtrigger.watch");
  csRef<iQuestTriggerFactory> trigfact = type->CreateTriggerFactory ();
  csRef<iWatchQuestTriggerFactory> newstate = 
  	scfQueryInterface<iWatchQuestTriggerFactory> (trigfact);
  newstate->SetEntityParameter (entity_par);
  newstate->SetTargetEntityParameter (target_entity_par);
  newstate->SetChecktimeParameter (checktime_par);
  newstate->SetRadiusParameter (radius_par);
  response->SetTriggerFactory (trigfact);
  return trigfact;
}

iQuestTriggerFactory* celQuestManager::SetOperationTrigger (
  	iQuestTriggerResponseFactory* response,
  	const char* operation_par,
	csRefArray<iQuestTriggerFactory> &trigger_factories)
{
  iQuestTriggerType* type = GetTriggerType ("cel.questtrigger.operation");
  csRef<iQuestTriggerFactory> trigfact = type->CreateTriggerFactory ();
  csRef<iOperationQuestTriggerFactory> newstate = 
  	scfQueryInterface<iOperationQuestTriggerFactory> (trigfact);
  newstate->SetOperationParameter (operation_par);
  csRefArray<iQuestTriggerFactory> trigger_factories_list = 
	newstate->GetTriggerFactories();
  csRefArray<iQuestTriggerFactory>::Iterator iter = 
	trigger_factories.GetIterator();
  while (iter.HasNext())
  {
    trigger_factories_list.Push(iter.Next());
  }
  return trigfact;
}

//---------------------------------------------------------------------------

