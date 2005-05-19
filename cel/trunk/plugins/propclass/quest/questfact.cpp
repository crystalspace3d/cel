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
#include "iutil/objreg.h"
#include "iutil/plugin.h"
#include "csutil/debug.h"
#include "plugins/propclass/quest/questfact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (Quest, "pcquest")

//---------------------------------------------------------------------------

csStringID celPcQuest::action_newquest = csInvalidStringID;
csStringID celPcQuest::action_stopquest = csInvalidStringID;
csStringID celPcQuest::id_name = csInvalidStringID;

SCF_IMPLEMENT_IBASE_EXT (celPcQuest)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcQuest)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcQuest::PcQuest)
  SCF_IMPLEMENTS_INTERFACE (iPcQuest)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcQuest::celPcQuest (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcQuest);

  // For SendMessage parameters.
  //params = new celOneParameterBlock ();
  //params->SetParameterDef (id_message, "message");

  // For PerformAction.
  if (action_newquest == csInvalidStringID)
  {
    action_newquest = pl->FetchStringID ("cel.action.NewQuest");
    action_stopquest = pl->FetchStringID ("cel.action.StopQuest");
    id_name = pl->FetchStringID ("cel.parameter.name");
  }

  // For properties.
  UpdateProperties (object_reg);
  propdata = new void* [propertycount];
  props = properties;
  propcount = &propertycount;
  propdata[propid_name] = 0;		// Handled in this class.
  propdata[propid_state] = 0;		// Handled in this class.

  GetQuestManager ();
}

celPcQuest::~celPcQuest ()
{
  //delete params;
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcQuest);
}

Property* celPcQuest::properties = 0;
size_t celPcQuest::propertycount = 0;

void celPcQuest::UpdateProperties (iObjectRegistry* object_reg)
{
  if (propertycount == 0)
  {
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    CS_ASSERT (pl != 0);

    propertycount = 2;
    properties = new Property[propertycount];

    properties[propid_name].id = pl->FetchStringID (
    	"cel.property.name");
    properties[propid_name].datatype = CEL_DATA_STRING;
    properties[propid_name].readonly = true;
    properties[propid_name].desc = "Quest Factory Name.";

    properties[propid_state].id = pl->FetchStringID (
    	"cel.property.state");
    properties[propid_state].datatype = CEL_DATA_STRING;
    properties[propid_state].readonly = false;
    properties[propid_state].desc = "Current State.";
  }
}

bool celPcQuest::SetProperty (csStringID propertyId, const char* b)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_state].id)
  {
    if (quest) quest->SwitchState (b);
    return true;
  }
  else
  {
    return celPcCommon::SetProperty (propertyId, b);
  }
}

const char* celPcQuest::GetPropertyString (csStringID propertyId)
{
  UpdateProperties (object_reg);
  if (propertyId == properties[propid_state].id)
  {
    return quest ? quest->GetCurrentState () : 0;
  }
  else if (propertyId == properties[propid_name].id)
  {
    return questname;
  }
  else
  {
    return celPcCommon::GetPropertyString (propertyId);
  }
}

#define QUEST_SERIAL 1

csPtr<iCelDataBuffer> celPcQuest::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (QUEST_SERIAL);

  databuf->Add (questname);

  databuf->Add ((uint32)quest_params.GetSize ());
  celQuestParams::GlobalIterator it = quest_params.GetIterator ();
  while (it.HasNext ())
  {
    csStrKey key;
    csStrKey value = it.Next (key);
    databuf->Add (key);
    databuf->Add (value);
  }

  if (quest)
  {
    databuf->Add (true);
    databuf->Add (quest->GetCurrentState ());
    quest->SaveState (databuf);
  }
  else
  {
    databuf->Add (false);
  }
  
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcQuest::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != QUEST_SERIAL) return false;

  questname = databuf->GetString ()->GetData ();

  celQuestParams qp;
  uint32 count = databuf->GetUInt32 ();
  size_t i;
  for (i = 0 ; i < count ; i++)
  {
    const char* key = databuf->GetString ()->GetData ();
    const char* value = databuf->GetString ()->GetData ();
    qp.Put (key, value);
  }

  bool has_quest = databuf->GetBool ();
  if (has_quest)
  {
    if (!NewQuest (questname, qp))
      return false;
    if (!quest->LoadState (databuf->GetString ()->GetData (), databuf))
      return false;
  }
  else
  {
    quest_params = qp;
  }

  return true;
}

bool celPcQuest::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == action_newquest)
  {
    CEL_FETCH_STRING_PAR (msg,params,id_name);
    if (!p_msg) return false;
    celQuestParams par;
    size_t i;
    for (i = 0 ; i < params->GetParameterCount () ; i++)
    {
      csStringID id;
      celDataType t;
      const char* n = params->GetParameter (i, id, t);
      if (t == CEL_DATA_STRING && strcmp ("name", n) != 0)
      {
        const celData* cd = params->GetParameter (id);
        par.Put (n, cd->value.s->GetData ());
      }
    }
    return NewQuest (msg, par);
  }
  else if (actionId == action_stopquest)
  {
    StopQuest ();
    return true;
  }
  return false;
}

void celPcQuest::GetQuestManager ()
{
  if (!quest_mgr)
  {
    quest_mgr = CS_QUERY_REGISTRY (object_reg, iQuestManager);
    if (!quest_mgr)
    {
      csRef<iPluginManager> plugin_mgr = CS_QUERY_REGISTRY (object_reg,
      	iPluginManager);
      quest_mgr = CS_LOAD_PLUGIN (plugin_mgr, "cel.manager.quests",
      	iQuestManager);
      if (!quest_mgr)
      {
	csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	  "cel.propclass.quest",
	  "Can't find quest manager plugin!");
        return;
      }
      if (!object_reg->Register (quest_mgr, "iQuestManager"))
      {
	csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	  "cel.propclass.quest",
	  "Couldn't register quest manager plugin!");
        return;
      }
    }
  }
}

bool celPcQuest::NewQuest (const char* name, celQuestParams& params)
{
  // @@@ Report error on reporter here?
  GetQuestManager ();
  if (!quest_mgr)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	  "cel.propclass.quest",
	  "Couldn't find quest manager!");
    return false;
  }
  iQuestFactory* fact = quest_mgr->GetQuestFactory (name);
  if (!fact)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	  "cel.propclass.quest",
	  "Couldn't find quest factory '%s'!", name);
    return false;
  }
  quest = fact->CreateQuest (params);
  if (!quest)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	  "cel.propclass.quest",
	  "Couldn't create quest from factory '%s'!", name);
    return false;
  }
  quest_params = params;
  questname = name;
  return true;
}

void celPcQuest::StopQuest ()
{
  quest = 0;
}

//---------------------------------------------------------------------------

