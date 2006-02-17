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
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/document.h"
#include "ivaria/reporter.h"

#include "behaviourlayer/behave.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"

#include "plugins/tools/quests/reward_message.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_REWARDTYPE(Message)

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.quests.reward.message",
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

SCF_IMPLEMENT_IBASE (celMessageRewardFactory)
  SCF_IMPLEMENTS_INTERFACE (iQuestRewardFactory)
  SCF_IMPLEMENTS_INTERFACE (iMessageQuestRewardFactory)
SCF_IMPLEMENT_IBASE_END

celMessageRewardFactory::celMessageRewardFactory (
	celMessageRewardType* type)
{
  SCF_CONSTRUCT_IBASE (0);
  celMessageRewardFactory::type = type;
  entity_par = 0;
  id_par = 0;
}

celMessageRewardFactory::~celMessageRewardFactory ()
{
  delete[] entity_par;
  delete[] id_par;

  SCF_DESTRUCT_IBASE ();
}

csPtr<iQuestReward> celMessageRewardFactory::CreateReward (
    iQuest*, const csHash<csStrKey,csStrKey>& params)
{
  celMessageReward* trig = new celMessageReward (type,
  	params, entity_par, id_par);
  return trig;
}

bool celMessageRewardFactory::Load (iDocumentNode* node)
{
  delete[] entity_par; entity_par = 0;
  delete[] id_par; id_par = 0;
  entity_par = csStrNew (node->GetAttributeValue ("entity"));
  id_par = csStrNew (node->GetAttributeValue ("id"));
  if (!entity_par)
    return Report (type->object_reg,
      "'entity' attribute is missing for the message reward!");
  if (!id_par)
    Report (type->object_reg,
      "'id' attribute is missing for the message reward!");
  return true;
}

void celMessageRewardFactory::SetEntityParameter (
	const char* entity)
{
  if (entity_par != entity)
  {
    delete[] entity_par;
    entity_par = csStrNew (entity);
  }
}

void celMessageRewardFactory::SetIDParameter (
	const char* id)
{
  if (id_par == id) return;
  delete[] id_par;
  id_par = csStrNew (id);
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celMessageReward)
  SCF_IMPLEMENTS_INTERFACE (iQuestReward)
SCF_IMPLEMENT_IBASE_END

celMessageReward::celMessageReward (
	celMessageRewardType* type,
  	const csHash<csStrKey,csStrKey>& params,
	const char* entity_par,
	const char* id_par)
{
  SCF_CONSTRUCT_IBASE (0);
  celMessageReward::type = type;
  csRef<iQuestManager> qm = CS_QUERY_REGISTRY (type->object_reg, iQuestManager);
  entity = csStrNew (qm->ResolveParameter (params, entity_par));
  id = csStrNew (qm->ResolveParameter (params, id_par));
}

celMessageReward::~celMessageReward ()
{
  delete[] entity;
  delete[] id;
  SCF_DESTRUCT_IBASE ();
}

void celMessageReward::Reward ()
{
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (type->object_reg, iCelPlLayer);
  if (!ent)
  {
    ent = pl->FindEntity (entity);
    if (!ent) return;
  }

  iCelBehaviour* behave = ent->GetBehaviour ();
  if (behave)
  {
    celData ret;
    behave->SendMessage (id, 0, ret, 0);
  }
}

//---------------------------------------------------------------------------

