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

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"

#include "plugins/tools/quests/reward_inventory.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_REWARDTYPE(Inventory)

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celInventoryRewardFactory)
  SCF_IMPLEMENTS_INTERFACE (iQuestRewardFactory)
  SCF_IMPLEMENTS_INTERFACE (iInventoryQuestRewardFactory)
SCF_IMPLEMENT_IBASE_END

celInventoryRewardFactory::celInventoryRewardFactory (
	celInventoryRewardType* type)
{
  SCF_CONSTRUCT_IBASE (0);
  celInventoryRewardFactory::type = type;
  entity_name_par = 0;
  child_entity_name_par = 0;
}

celInventoryRewardFactory::~celInventoryRewardFactory ()
{
  delete[] entity_name_par;
  delete[] child_entity_name_par;

  SCF_DESTRUCT_IBASE ();
}

csPtr<iQuestReward> celInventoryRewardFactory::CreateReward (
    const csHash<csStrKey,csStrKey>& params)
{
  celInventoryReward* trig = new celInventoryReward (type,
  	params, entity_name_par, child_entity_name_par);
  return trig;
}

bool celInventoryRewardFactory::Load (iDocumentNode* node)
{
  delete[] entity_name_par; entity_name_par = 0;
  delete[] child_entity_name_par; child_entity_name_par = 0;
  entity_name_par = csStrNew (node->GetAttributeValue ("entity_name"));
  child_entity_name_par = csStrNew (node->GetAttributeValue (
  	"child_entity_name"));
  if (!entity_name_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questreward.inventory",
      "'entity_name' attribute is missing for the inventory reward!");
    return false;
  }
  if (!child_entity_name_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questreward.inventory",
      "'child_entity_name' attribute is missing for the inventory reward!");
    return false;
  }
  return true;
}

void celInventoryRewardFactory::SetEntityNameParameter (
	const char* entity_name)
{
  if (entity_name_par == entity_name) return;
  delete[] entity_name_par;
  entity_name_par = csStrNew (entity_name);
}

void celInventoryRewardFactory::SetChildEntityNameParameter (
	const char* entity_name)
{
  if (child_entity_name_par == entity_name) return;
  delete[] child_entity_name_par;
  child_entity_name_par = csStrNew (entity_name);
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celInventoryReward)
  SCF_IMPLEMENTS_INTERFACE (iQuestReward)
SCF_IMPLEMENT_IBASE_END

celInventoryReward::celInventoryReward (
	celInventoryRewardType* type,
  	const csHash<csStrKey,csStrKey>& params,
	const char* entity_name_par,
	const char* child_entity_name_par)
{
  SCF_CONSTRUCT_IBASE (0);
  celInventoryReward::type = type;
  csRef<iQuestManager> qm = CS_QUERY_REGISTRY (type->object_reg, iQuestManager);
  entity_name = csStrNew (qm->ResolveParameter (params, entity_name_par));
  child_entity_name = csStrNew (qm->ResolveParameter (params,
  	child_entity_name_par));
}

celInventoryReward::~celInventoryReward ()
{
  delete[] entity_name;
  delete[] child_entity_name;
  SCF_DESTRUCT_IBASE ();
}

void celInventoryReward::Reward ()
{
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (type->object_reg, iCelPlLayer);
  if (!inventory)
  {
    if (!entity)
    {
      entity = pl->FindEntity (entity_name);
      if (!entity) return;
    }
    inventory = CEL_QUERY_PROPCLASS_ENT (entity, iPcInventory);
    if (!inventory) return;
  }

  iCelEntity* child_entity = pl->FindEntity (child_entity_name);
  if (!child_entity) return;	// @@@ Report error!

  if (!inventory->AddEntity (child_entity))
  {
    // @@@ Report error!
    return;
  }

  printf ("New item in inventory!\n");
  fflush (stdout);
}

//---------------------------------------------------------------------------

