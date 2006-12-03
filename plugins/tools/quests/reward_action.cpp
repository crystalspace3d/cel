/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein

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
#include "csutil/scanstr.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/document.h"
#include "ivaria/reporter.h"

#include "celtool/stdparams.h"
#include "behaviourlayer/behave.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"

#include "plugins/tools/quests/quests.h"
#include "plugins/tools/quests/reward_action.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_REWARDTYPE(Action)

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.quests.reward.action",
    	msg, arg);
  else
  {
    csPrintfV (msg, arg);
    csPrintf ("\n");
    fflush (stdout);
  }

  va_end (arg);
  return false;
}

//---------------------------------------------------------------------------

celActionRewardFactory::celActionRewardFactory (
	celActionRewardType* type) : scfImplementationType (this)
{
  celActionRewardFactory::type = type;
  entity_par = 0;
  pcclass_par = 0;
  tag_par = 0;
  id_par = 0;
}

celActionRewardFactory::~celActionRewardFactory ()
{
  delete[] entity_par;
  delete[] id_par;
}

csPtr<iQuestReward> celActionRewardFactory::CreateReward (
    iQuest*, const celQuestParams& params)
{
  celActionReward* trig = new celActionReward (type,
  	params, entity_par, id_par, pcclass_par, tag_par, parameters);
  return trig;
}

bool celActionRewardFactory::Load (iDocumentNode* node)
{
  delete[] entity_par; entity_par = 0;
  delete[] id_par; id_par = 0;
  delete[] pcclass_par; entity_par = 0;
  delete[] tag_par; id_par = 0;
  entity_par = csStrNew (node->GetAttributeValue ("entity"));
  pcclass_par = csStrNew (node->GetAttributeValue ("pc"));
  tag_par = csStrNew (node->GetAttributeValue ("tag"));
  id_par = csStrNew (node->GetAttributeValue ("id"));
  if (!entity_par)
    return Report (type->object_reg,
      "'entity' attribute is missing for the action reward!");
  if (!id_par)
    Report (type->object_reg,
      "'id' attribute is missing for the action reward!");
  if (!pcclass_par)
    Report (type->object_reg,
      "'pc' attribute is missing for the action reward!");
  iCelPlLayer* pl = type->pl;
  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    if (!strcmp ("par", value))
    {
      const char* name = child->GetAttributeValue ("name");
      if (!name)
        return Report (type->object_reg,
          "Missing name attribute in a parameter for the action reward!");
      csString fullname = "cel.parameter.";
      fullname += name;
      csStringID id = pl->FetchStringID (fullname);
      const char* str_value = child->GetAttributeValue ("string");
      if (str_value)
      {
	AddParameter (CEL_DATA_STRING, id, name, str_value);
	continue;
      }
      const char* vec3_value = child->GetAttributeValue ("vector3");
      if (vec3_value)
      {
	AddParameter (CEL_DATA_VECTOR3, id, name, vec3_value);
	continue;
      }
      const char* vec2_value = child->GetAttributeValue ("vector2");
      if (vec2_value)
      {
	AddParameter (CEL_DATA_VECTOR2, id, name, vec2_value);
	continue;
      }
      const char* float_value = child->GetAttributeValue ("float");
      if (float_value)
      {
	AddParameter (CEL_DATA_FLOAT, id, name, float_value);
	continue;
      }
      const char* long_value = child->GetAttributeValue ("long");
      if (long_value)
      {
	AddParameter (CEL_DATA_LONG, id, name, long_value);
	continue;
      }
      const char* bool_value = child->GetAttributeValue ("bool");
      if (bool_value)
      {
	AddParameter (CEL_DATA_BOOL, id, name, bool_value);
	continue;
      }
      return Report (type->object_reg,
        "Unknown parameter type for action reward!");
    }
    else
    {
      return Report (type->object_reg,
        "Unexpected child '%s' in the action reward!", value);
    }
  }
  return true;
}

void celActionRewardFactory::SetEntityParameter (
	const char* entity)
{
  if (entity_par != entity)
  {
    delete[] entity_par;
    entity_par = csStrNew (entity);
  }
}

void celActionRewardFactory::SetIDParameter (
	const char* id)
{
  if (id_par == id) return;
  delete[] id_par;
  id_par = csStrNew (id);
}

void celActionRewardFactory::SetPropertyClassParameter (
	const char* propertyclass)
{
  if (pcclass_par != propertyclass)
  {
    delete[] pcclass_par;
    pcclass_par = csStrNew (propertyclass);
  }
}

void celActionRewardFactory::SetTagParameter (
	const char* pctag)
{
  if (tag_par != pctag)
  {
    delete[] tag_par;
    tag_par = csStrNew (pctag);
  }
}

void celActionRewardFactory::AddParameter (celDataType type,
    csStringID id, const char* name, const char* value)
{
  size_t idx = parameters.Push (parSpec ());
  parameters[idx].type = type;
  parameters[idx].id = id;
  parameters[idx].name = name;
  parameters[idx].value = value;
}

//---------------------------------------------------------------------------

celActionReward::celActionReward (
	celActionRewardType* type,
  	const celQuestParams& params,
	const char* entity_par,
	const char* id_par,
	const char* pcclass_par,
	const char* tag_par,
	const csArray<parSpec>& parameters) : scfImplementationType (this)
{
  celActionReward::type = type;
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  pcclass = csStrNew (qm->ResolveParameter (params, pcclass_par));
  tag = csStrNew (qm->ResolveParameter (params, tag_par));
  entity = csStrNew (qm->ResolveParameter (params, entity_par));
  id = csStrNew (qm->ResolveParameter (params, id_par));
  act_params = new celVariableParameterBlock ();
  size_t i;
  for (i = 0 ; i < parameters.Length () ; i++)
  {
    csString v = qm->ResolveParameter (params, parameters[i].value);
    act_params->SetParameterDef (i, parameters[i].id, parameters[i].name);
    switch (parameters[i].type)
    {
      case CEL_DATA_STRING:
	act_params->GetParameter (i).Set ((const char*)v);
	break;
      case CEL_DATA_LONG:
	{
	  long f;
	  csScanStr (v, "%d", &f);
	  act_params->GetParameter (i).Set ((int32)f);
	}
	break;
      case CEL_DATA_FLOAT:
	{
	  float f;
	  csScanStr (v, "%f", &f);
	  act_params->GetParameter (i).Set (f);
	}
	break;
      case CEL_DATA_BOOL:
	{
	  bool f;
	  csScanStr (v, "%b", &f);
	  act_params->GetParameter (i).Set (f);
	}
	break;
      case CEL_DATA_VECTOR2:
	{
	  csVector2 vec;
	  csScanStr (v, "%f,%f", &vec.x, &vec.y);
	  act_params->GetParameter (i).Set (vec);
	}
	break;
      case CEL_DATA_VECTOR3:
	{
	  csVector3 vec;
	  csScanStr (v, "%f,%f,%f", &vec.x, &vec.y, &vec.z);
	  act_params->GetParameter (i).Set (vec);
	}
	break;
      case CEL_DATA_COLOR:
	{
	  csColor vec;
	  csScanStr (v, "%f,%f,%f", &vec.red, &vec.green, &vec.blue);
	  act_params->GetParameter (i).Set (vec);
	}
	break;
      default:
	//@@@?
	break;
    }
  }
}

celActionReward::~celActionReward ()
{
  delete[] pcclass;
  delete[] tag;
  delete[] entity;
  delete[] id;
  delete act_params;
}

void celActionReward::Reward ()
{
  iCelPlLayer* pl = type->pl;
  if (!ent)
  {
    ent = pl->FindEntity (entity);
    if (!ent) return;
  }
  csRef<iCelPropertyClass> propertyclass;
  propertyclass = ent->GetPropertyClassList()->FindByNameAndTag(pcclass,tag);
  if (propertyclass)
  {
    csString fullname = "cel.action.";
    fullname += id;
    csStringID actionID = pl->FetchStringID(fullname);
    if (actionID)
    {
      celData ret;
      propertyclass->PerformAction(actionID, act_params, ret);
    }
    else
      Report (type->object_reg,
		"No action  'cel.action.%s' in the specified pc!", id);
  }
  else
    Report (type->object_reg,
            "No propertyclass  '%s' in the specified entity!", pcclass);
}

//---------------------------------------------------------------------------

