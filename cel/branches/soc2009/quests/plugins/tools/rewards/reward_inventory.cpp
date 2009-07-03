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
#include "iutil/plugin.h"
#include "ivaria/reporter.h"
#include "iengine/mesh.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "propclass/mesh.h"

#include "plugins/tools/rewards/reward_inventory.h"

//---------------------------------------------------------------------------
SCF_IMPLEMENT_FACTORY (celInventoryRewardType)
CEL_IMPLEMENT_REWARDTYPE_NEW (Inventory)

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.rewards.inventory",
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

celInventoryRewardFactory::celInventoryRewardFactory (
	celInventoryRewardType* type) : scfImplementationType (this)
{
  celInventoryRewardFactory::type = type;
}

celInventoryRewardFactory::~celInventoryRewardFactory ()
{
}

csPtr<iReward> celInventoryRewardFactory::CreateReward (
    const celParams& params)
{
  celInventoryReward* trig = new celInventoryReward (type,
  	params, entity_par, tag_par, child_entity_par, child_tag_par);
  return trig;
}

bool celInventoryRewardFactory::Load (iDocumentNode* node)
{
  entity_par = node->GetAttributeValue ("entity");
  child_entity_par = node->GetAttributeValue ("child_entity");
  tag_par = node->GetAttributeValue ("entity_tag");
  child_tag_par = node->GetAttributeValue ("child_entity_tag");
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
  entity_par = entity;
  tag_par = tag;
}

void celInventoryRewardFactory::SetChildEntityParameter (
	const char* entity, const char* tag)
{
  child_entity_par = entity;
  child_tag_par = tag;
}

//---------------------------------------------------------------------------

celInventoryReward::celInventoryReward (
	celInventoryRewardType* type,
  	const celParams& params,
	const char* entity_par, const char* tag_par,
	const char* child_entity_par, const char* child_tag_par) :
	scfImplementationType (this)
{
  celInventoryReward::type = type;
  //NEW
  csRef<iPluginManager> plugin_mgr = 
   csQueryRegistry<iPluginManager> (type->object_reg);
  pm = csLoadPlugin<iParameterManager> (plugin_mgr,
    "cel.parameters.manager");

  entity = pm->GetParameter (params, entity_par);
  tag = pm->GetParameter (params, tag_par);
  child_entity = pm->GetParameter (params, child_entity_par);
  child_tag = pm->GetParameter (params, child_tag_par);
}

celInventoryReward::~celInventoryReward ()
{
}

void celInventoryReward::Reward (iCelParameterBlock* params)
{
  iCelPlLayer* pl = type->pl;
  bool changed;
  const char* e = entity->Get (params, changed);
  if (changed) { ent = 0; inventory = 0; }
  const char* t = tag->Get (params, changed);
  if (changed) { inventory = 0; }

  if (!inventory)
  {
    if (!ent)
    {
      ent = pl->FindEntity (e);
      if (!ent) return;
    }
    inventory = CEL_QUERY_PROPCLASS_TAG_ENT (ent, iPcInventory, t);
    if (!inventory) return;
  }

  const char* ce = child_entity->Get (params);
  if (!ce) return;
  iCelEntity* child_ent = pl->FindEntity (ce);
  if (!child_ent)
  {
    Report (type->object_reg, "Can't find entity '%s' in inventory reward!", ce);
    return;
  }

  if (!inventory->AddEntity (child_ent))
  {
    Report (type->object_reg, "Can't add entity '%s' in inventory reward!", ce);
    return;
  }

  // Make the mesh invisible if the entity has one.
  const char* cet = child_tag->Get (params);
  csRef<iPcMesh> pcmesh = CEL_QUERY_PROPCLASS_TAG_ENT (child_ent, iPcMesh, cet);
  if (pcmesh)
    pcmesh->GetMesh ()->GetFlags ().Set (CS_ENTITY_INVISIBLE);

  printf ("New item in inventory!\n");
  fflush (stdout);
}

//---------------------------------------------------------------------------

