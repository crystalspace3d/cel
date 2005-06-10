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

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.quests.reward.inventory",
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
  tag_par = 0;
  child_tag_par = 0;
}

celInventoryRewardFactory::~celInventoryRewardFactory ()
{
  delete[] entity_par;
  delete[] child_entity_par;
  delete[] tag_par;
  delete[] child_tag_par;

  SCF_DESTRUCT_IBASE ();
}

csPtr<iQuestReward> celInventoryRewardFactory::CreateReward (
    iQuest*, const csHash<csStrKey,csStrKey>& params)
{
  celInventoryReward* trig = new celInventoryReward (type,
  	params, entity_par, tag_par, child_entity_par, child_tag_par);
  return trig;
}

bool celInventoryRewardFactory::Load (iDocumentNode* node)
{
  delete[] entity_par; entity_par = 0;
  delete[] child_entity_par; child_entity_par = 0;
  delete[] tag_par; tag_par = 0;
  delete[] child_tag_par; child_tag_par = 0;
  entity_par = csStrNew (node->GetAttributeValue ("entity"));
  child_entity_par = csStrNew (node->GetAttributeValue (
  	"child_entity"));
  tag_par = csStrNew (node->GetAttributeValue ("entity_tag"));
  child_tag_par = csStrNew (node->GetAttributeValue (
  	"child_entity_tag"));
  if (!entity_par)
    return Report (type->object_reg,
      "'entity' attribute is missing for the inventory reward!");
  if (!child_entity_par)
    return Report (type->object_reg,
      "'child_entity' attribute is missing for the inventory reward!");
  return true;
}

void celInventoryRewardFactory::SetEntityParameter (
	const char* entity, const char* tag)
{
  if (entity_par != entity)
  {
    delete[] entity_par;
    entity_par = csStrNew (entity);
  }
  if (tag_par != tag)
  {
    delete[] tag_par;
    tag_par = csStrNew (tag);
  }
}

void celInventoryRewardFactory::SetChildEntityParameter (
	const char* entity, const char* tag)
{
  if (child_entity_par != entity)
  {
    delete[] child_entity_par;
    child_entity_par = csStrNew (entity);
  }
  if (child_tag_par != tag)
  {
    delete[] child_tag_par;
    child_tag_par = csStrNew (tag);
  }
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celInventoryReward)
  SCF_IMPLEMENTS_INTERFACE (iQuestReward)
SCF_IMPLEMENT_IBASE_END

celInventoryReward::celInventoryReward (
	celInventoryRewardType* type,
  	const csHash<csStrKey,csStrKey>& params,
	const char* entity_par, const char* tag_par,
	const char* child_entity_par, const char* child_tag_par)
{
  SCF_CONSTRUCT_IBASE (0);
  celInventoryReward::type = type;
  csRef<iQuestManager> qm = CS_QUERY_REGISTRY (type->object_reg, iQuestManager);
  entity = csStrNew (qm->ResolveParameter (params, entity_par));
  tag = csStrNew (qm->ResolveParameter (params, tag_par));
  child_entity = csStrNew (qm->ResolveParameter (params, child_entity_par));
  child_tag = csStrNew (qm->ResolveParameter (params, child_tag_par));
}

celInventoryReward::~celInventoryReward ()
{
  delete[] entity;
  delete[] child_entity;
  delete[] tag;
  delete[] child_tag;
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
    inventory = CEL_QUERY_PROPCLASS_TAG_ENT (ent, iPcInventory, tag);
    if (!inventory) return;
  }

  iCelEntity* child_ent = pl->FindEntity (child_entity);
  if (!child_ent)
  {
    Report (type->object_reg, "Can't create entity '%s' in inventory reward!",
    	(const char*)child_entity);
    return;
  }

  if (!inventory->AddEntity (child_ent))
  {
    Report (type->object_reg, "Can't add entity '%s' in inventory reward!",
    	(const char*)child_entity);
    return;
  }

  // Make the mesh invisible if the entity has one.
  csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_TAG_ENT (child_ent, iPcMesh,
  	child_tag);
  if (pcmesh)
    pcmesh->GetMesh ()->GetFlags ().Set (CS_ENTITY_INVISIBLE);

  printf ("New item in inventory!\n");
  fflush (stdout);
}

//---------------------------------------------------------------------------

