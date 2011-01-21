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
#include "cstool/initapp.h"
#include "ivaria/reporter.h"
#include "plugins/propclass/quest/questfact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY_ALT (Quest, "pclogic.quest", "pcquest")

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.propclass.quest",
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

csStringID celPcQuest::id_name = csInvalidStringID;

PropertyHolder celPcQuest::propinfo;

celPcQuest::celPcQuest (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  // For SendMessage parameters.
  //params = new celOneParameterBlock ();
  //params->SetParameterDef (id_message);

  // For actions.
  if (id_name == csInvalidStringID)
  {
    id_name = pl->FetchStringID ("name");
  }

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.quest.action.");
    AddAction (action_newquest, "NewQuest");
    AddAction (action_stopquest, "StopQuest");
  }

  // For properties.
  propinfo.SetCount (2);
  AddProperty (propid_name, "name",
	CEL_DATA_STRING, true, "Quest Factory Name.", 0);
  AddProperty (propid_state, "state",
	CEL_DATA_STRING, false, "Current State.", 0);

  GetQuestManager ();
}

celPcQuest::~celPcQuest ()
{
  //delete params;
}

bool celPcQuest::SetPropertyIndexed (int idx, const char* b)
{
  if (idx == propid_state)
  {
    if (quest) quest->SwitchState (b);
    return true;
  }
  return false;
}

bool celPcQuest::GetPropertyIndexed (int idx, const char*& b)
{
  switch (idx)
  {
    case propid_state:
      b = quest ? quest->GetCurrentState () : 0;
      return true;
    case propid_name:
      b = questname;
      return true;
    default:
      return false;
  }
}

#define QUEST_SERIAL 1

csPtr<iCelDataBuffer> celPcQuest::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (QUEST_SERIAL);

  databuf->Add (questname);

  databuf->Add ((uint32)quest_params.GetSize ());
  celParams::GlobalIterator it = quest_params.GetIterator ();
  while (it.HasNext ())
  {
    csStringBase key;
    csStringBase value = it.Next (key);
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
  if (serialnr != QUEST_SERIAL)
    return Report (object_reg, "Couldn't load pcquest!");

  questname = databuf->GetString ()->GetData ();

  celParams qp;
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

bool celPcQuest::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_newquest:
      {
        CEL_FETCH_STRING_PAR (msg,params,id_name);
        if (!p_msg)
          return Report (object_reg,
      	    "Missing parameter 'name' for action NewQuest!");
        celParams par;
        size_t i;
        for (i = 0 ; i < params->GetParameterCount () ; i++)
        {
          celDataType t;
          csStringID id = params->GetParameterDef (i, t);
	  const char* n = pl->FetchString (id);
          if (t == CEL_DATA_STRING && strcmp ("name", n) != 0)
          {
            const celData* cd = params->GetParameter (id);
            par.Put (n, cd->value.s->GetData ());
          }
        }
        return NewQuest (msg, par);
      }
    case action_stopquest:
      StopQuest ();
      return true;
    default:
      return false;
  }
}

void celPcQuest::GetQuestManager ()
{
  if (!quest_mgr)
  {
    quest_mgr = csQueryRegistryOrLoad<iQuestManager> (object_reg,
    	"cel.manager.quests");
    if (!quest_mgr)
    {
      Report (object_reg, "Can't load quest manager plugin!");
      return;
    }
  }
}

bool celPcQuest::NewQuest (const char* name, celParams& params)
{
  GetQuestManager ();
  if (!quest_mgr)
    return Report (object_reg, "Couldn't find quest manager!");
  iQuestFactory* fact = quest_mgr->GetQuestFactory (name);
  if (!fact)
    return Report (object_reg, "Couldn't find quest factory '%s'!", name);
  
  quest_params = params;
  quest_params.Put ("this", entity->GetName ());

  quest = fact->CreateQuest (quest_params);
  quest_params.Delete ("this", entity->GetName ());
  if (!quest)
    Report (object_reg, "Couldn't create quest from factory '%s'!", name);
  questname = name;
  return true;
}

void celPcQuest::StopQuest ()
{
  quest = 0;
}

//---------------------------------------------------------------------------

