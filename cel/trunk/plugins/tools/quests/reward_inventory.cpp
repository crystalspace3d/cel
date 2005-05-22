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
#include "csutil/flags.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/document.h"
#include "ivaria/reporter.h"
#include "iengine/mesh.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "propclass/mesh.h"

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
  entity_par = 0;
  child_entity_par = 0;
}

celInventoryRewardFactory::~celInventoryRewardFactory ()
{
  delete[] entity_par;
  delete[] child_entity_par;

  SCF_DESTRUCT_IBASE ();
}

csPtr<iQuestReward> celInventoryRewardFactory::CreateReward (
    const csHash<csStrKey,csStrKey>& params)
{
  celInventoryReward* trig = new celInventoryReward (type,
  	params, entity_par, child_entity_par);
  return trig;
}

bool celInventoryRewardFactory::Load (iDocumentNode* node)
{
  delete[] entity_par; entity_par = 0;
  delete[] child_entity_par; child_entity_par = 0;
  entity_par = csStrNew (node->GetAttributeValue ("entity"));
  child_entity_par = csStrNew (node->GetAttributeValue (
  	"child_entity"));
  if (!entity_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questreward.inventory",
      "'entity' attribute is missing for the inventory reward!");
    return false;
  }
  if (!child_entity_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questreward.inventory",
      "'child_entity' attribute is missing for the inventory reward!");
    return false;
  }
  return true;
}

void celInventoryRewardFactory::SetEntityParameter (
	const char* entity)
{
  if (entity_par == entity) return;
  delete[] entity_par;
  entity_par = csStrNew (entity);
}

void celInventoryRewardFactory::SetChildEntityParameter (
	const char* entity)
{
  if (child_entity_par == entity) return;
  delete[] child_entity_par;
  child_entity_par = csStrNew (entity);
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celInventoryReward)
  SCF_IMPLEMENTS_INTERFACE (iQuestReward)
SCF_IMPLEMENT_IBASE_END

celInventoryReward::celInventoryReward (
	celInventoryRewardType* type,
  	const csHash<csStrKey,csStrKey>& params,
	const char* entity_par,
	const char* child_entity_par)
{
  SCF_CONSTRUCT_IBASE (0);
  celInventoryReward::type = type;
  csRef<iQuestManager> qm = CS_QUERY_REGISTRY (type->object_reg, iQuestManager);
  entity = csStrNew (qm->ResolveParameter (params, entity_par));
  child_entity = csStrNew (qm->ResolveParameter (params,
  	child_entity_par));
}

celInventoryReward::~celInventoryReward ()
{
  delete[] entity;
  delete[] child_entity;
  SCF_DESTRUCT_IBASE ();
}

void celInventoryReward::Reward ()
{
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (type->object_reg, iCelPlLayer);
  if (!inventory)
  {
    if (!ent)
    {
      ent = pl->FindEntity (entity);
      if (!ent) return;
    }
    inventory = CEL_QUERY_PROPCLASS_ENT (ent, iPcInventory);
    if (!inventory) return;
  }

  iCelEntity* child_ent = pl->FindEntity (child_entity);
  if (!child_ent) return;	// @@@ Report error!

  if (!inventory->AddEntity (child_ent))
  {
    // @@@ Report error!
    return;
  }

  // Make the mesh invisible if the entity has one.
  csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_ENT (child_ent, iPcMesh);
  if (pcmesh)
    pcmesh->GetMesh ()->GetFlags ().Set (CS_ENTITY_INVISIBLE);

  printf ("New item in inventory!\n");
  fflush (stdout);
}

//---------------------------------------------------------------------------

