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

  quest_params.AttachNew (new celVariableParameterBlock ());
}

celPcQuest::~celPcQuest ()
{
  //delete params;
}

class DelayedSwitchState : public scfImplementation1<DelayedSwitchState, iCallable>
{
private:
  iQuest* quest;
  csString state;

public:
  DelayedSwitchState (iQuest* quest, const char* state) :
    scfImplementationType (this), quest (quest), state (state) { }
  virtual ~DelayedSwitchState () { }
  virtual void Call ()
  {
    quest->SwitchState (state);
  }
};

bool celPcQuest::SetPropertyIndexed (int idx, const char* b)
{
  if (idx == propid_state)
  {
    if (quest)
    {
      // We work with a delayed call here because it is possible that
      // the entity hasn't been created fully and we don't want states
      // that depend on things that haven't been made yet to fail.
      csRef<DelayedSwitchState> dss;
      dss.AttachNew (new DelayedSwitchState (quest, b));
      pl->CallLater (dss);

    }
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

bool celPcQuest::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_newquest:
      {
	csString msg;
	if (!Fetch (msg, params, id_name)) return false;
        return NewQuest (msg, params);
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
      Error ("Can't load quest manager plugin!");
      return;
    }
  }
}

bool celPcQuest::NewQuest (const char* name, iCelParameterBlock* params)
{
  GetQuestManager ();
  if (!quest_mgr)
    return Error ("Couldn't find quest manager!");
  iQuestFactory* fact = quest_mgr->GetQuestFactory (name);
  if (!fact)
    return Error ("Couldn't find quest factory '%s'!", name);
  
  quest_params.AttachNew (new celVariableParameterBlock ());
  quest_params->AddParameter (pl->FetchStringID ("this")).Set (entity);
  quest_params->Merge (params);

  quest = fact->CreateQuest (quest_params);
  if (!quest)
    Error ("Couldn't create quest from factory '%s'!", name);
  questname = name;
  return true;
}

void celPcQuest::StopQuest ()
{
  quest = 0;
}

void celPcQuest::Activate ()
{
  if (quest) quest->Activate ();
}

void celPcQuest::Deactivate ()
{
  if (quest) quest->Deactivate ();
}

//---------------------------------------------------------------------------

