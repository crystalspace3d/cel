/*
    Crystal Space Entity Layer
    Copyright (C) 2001 by Jorrit Tyberghein

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
#include "csutil/util.h"
#include "iutil/objreg.h"
#include "iutil/plugin.h"
#include "plugins/propclass/inv/invfact.h"
#include "physicallayer/pl.h"
#include "physicallayer/datatype.h"
#include "physicallayer/persist.h"
#include "physicallayer/entity.h"
#include "behaviourlayer/behave.h"
#include "ivaria/reporter.h"
#include "tools/loot.h"

//---------------------------------------------------------------------------

#define DEF 0

CEL_IMPLEMENT_FACTORY_ALT (Inventory, "pctools.inventory", "pcinventory")
CEL_IMPLEMENT_FACTORY_ALT (Characteristics, "pctools.inventory.characteristics", "pccharacteristics")

void Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.pctools.inventory",
    	msg, arg);
  else
  {
    csPrintfV (msg, arg);
    csPrintf ("\n");
    fflush (stdout);
  }

  va_end (arg);
}

//---------------------------------------------------------------------------

csStringID celPcInventory::id_entity = csInvalidStringID;
csStringID celPcInventory::id_amount = csInvalidStringID;
csStringID celPcInventory::id_name = csInvalidStringID;

PropertyHolder celPcInventory::propinfo;

celPcInventory::celPcInventory (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  if (id_entity == csInvalidStringID)
  {
    id_entity = pl->FetchStringID ("entity");
    id_amount = pl->FetchStringID ("amount");
    id_name = pl->FetchStringID ("name");
  }
  params.AttachNew (new celVariableParameterBlock ());
  params->SetParameterDef (0, id_entity);
  params->SetParameterDef (1, id_amount);

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.inventory.action.");
    AddAction (action_addtemplate, "AddTemplate");
    AddAction (action_removetemplate, "RemoveTemplate");
    AddAction (action_setlootgenerator, "SetLootGenerator");
    AddAction (action_generateloot, "GenerateLoot");
  }
  generatorActive = false;
  atBaseline = false;
}

celPcInventory::~celPcInventory ()
{
  RemoveAllConstraints ();
}

bool celPcInventory::GenerateLoot ()
{
  if (!generatorActive) return true;
  generatorActive = false;
  if (!generator) return true;

  atBaseline = false;

  csRef<celOneParameterBlock> params;
  params.AttachNew (new celOneParameterBlock ());
  params->SetParameterDef (pl->FetchStringID ("this"));
  params->GetParameter (0).Set (entity);
  return generator->GenerateLoot (this, params);
}

bool celPcInventory::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  if (idx == action_addtemplate)
  {
    CEL_FETCH_STRING_PAR (name,params,id_name);
    CEL_FETCH_LONG_PAR (amount,params,id_amount);
    if (!p_name) return false;
    iCelEntityTemplate* tpl = pl->FindEntityTemplate (name);
    if (!tpl)
    {
      // @@@ Proper error.
      printf ("Can't find entity template '%s'!\n", name);
      return false;
    }
    bool rc = AddEntityTemplate (tpl, amount);
    ret.Set (rc);
    return true;
  }
  else if (idx == action_removetemplate)
  {
    CEL_FETCH_STRING_PAR (name,params,id_name);
    CEL_FETCH_LONG_PAR (amount,params,id_amount);
    if (!p_name) return false;
    iCelEntityTemplate* tpl = pl->FindEntityTemplate (name);
    if (!tpl)
    {
      // @@@ Proper error.
      printf ("Can't find entity template '%s'!\n", name);
      return false;
    }
    bool rc = RemoveEntityTemplate (tpl, amount);
    ret.Set (rc);
    return true;
  }
  else if (idx == action_setlootgenerator)
  {
    csRef<iLootManager> lootmgr = csLoadPluginCheck<iLootManager> (object_reg, "cel.tools.lootmanager");
    if (!lootmgr)
    {
      // @@@ Proper error.
      printf ("Can't find loot manager!\n");
      return false;
    }
    CEL_FETCH_STRING_PAR (name,params,id_name);
    if (!p_name) return false;
    iLootGenerator* gen = lootmgr->FindLootGenerator (name);
    if (!gen)
    {
      // @@@ Proper error.
      printf ("Can't find loot generator'%s'!\n", name);
      return false;
    }
    SetLootGenerator (gen);
    return true;
  }
  else if (idx == action_generateloot)
  {
    ret.Set (GenerateLoot ());
    return true;
  }
  return false;
}

void celPcInventory::SaveModifications (iCelCompactDataBufferWriter* buf, iStringSet* strings)
{
  buf->AddBool (generatorActive);
  if (generator)
  {
    csStringID generatorID = strings->Request (generator->GetName ());
    buf->AddID (generatorID);
  }
  else
  {
    buf->AddID (csInvalidStringID);
  }
  buf->AddUInt16 (contents.GetSize ());
  for (size_t i = 0 ; i < contents.GetSize () ; i++)
  {
    // We only save ID here. We assume the contents of the entities
    // will be saved elsewhere.
    buf->AddUInt32 (contents[i]->GetID ());
  }
  buf->AddUInt16 (templatedContents.GetSize ());
  for (size_t i = 0 ; i < templatedContents.GetSize () ; i++)
  {
    csStringID tplID = strings->Request (templatedContents[i].tpl->GetName ());
    buf->AddID (tplID); 
    buf->AddInt32 (templatedContents[i].amount);
  }
}

void celPcInventory::RestoreModifications (iCelCompactDataBufferReader* buf,
    const csHash<csString,csStringID>& strings)
{
  RemoveAll ();

  generatorActive = buf->GetBool ();
  csStringID generatorID = buf->GetID ();
  if (generatorID == csInvalidStringID)
    generator = 0;
  else
  {
    csRef<iLootManager> lootmgr = csLoadPluginCheck<iLootManager> (object_reg, "cel.tools.lootmanager");
    if (!lootmgr)
    {
      printf ("Error! Couldn't find loot manager!\n");
      return;
    }
    const char* lootname = strings.Get (generatorID, (const char*)0);
    //printf ("this=%p generatorID=%d loot=%s\n", this, (uint)generatorID, lootname);
    if (lootname)
      generator = lootmgr->FindLootGenerator (lootname);
    else
      generator = 0;
  }
  size_t coSize = buf->GetUInt16 ();
  for (size_t i = 0 ; i < coSize ; i++)
  {
    uint entID = (uint)buf->GetUInt32 ();
    csRef<iCelEntity> ent = pl->GetEntity (entID);
    if (!ent)
    {
      // If we can't find the entity we will create it here. We will then
      // assume that later code will complete the loading of this entity.
      printf ("Inv: Couldn't find entity '%d', creating dummy!\n", entID);
      ent = pl->CreateEntity (entID);
    }
    contents.Push (ent);
  }
  size_t tcSize = buf->GetUInt16 ();
  for (size_t i = 0 ; i < tcSize ; i++)
  {
    csStringID tplID = buf->GetID ();
    TemplateStack ts;
    const char* tplName = strings.Get (tplID, (const char*)0);
    ts.tpl = pl->FindEntityTemplate (tplName);
    if (!ts.tpl)
    {
      printf ("Error! Couldn't find template '%s'!\n", tplName);
      return;
    }
    ts.amount = buf->GetInt32 ();
    templatedContents.Push (ts);
  }
  atBaseline = false;
}

bool celPcInventory::AddEntityTemplate (iCelEntityTemplate* child, int amount)
{
  if ((!allowedClasses.IsEmpty ()) && !allowedClasses.TestIntersect (
        child->GetClasses ()))
    return false;

  if (amount == 0) return true; // This works always.

  //if (space)
  //{
    //bool ret = space->AddEntity (child, pparams);
    //if (!ret) return false;
  //}

  // Add our child. We will later test if this is valid and if
  // not undo this change.
  bool newlyadded = false;
  size_t idx = FindEntityTemplate (child);
  if (idx == csArrayItemNotFound)
  {
    newlyadded = true;
    TemplateStack ts;
    ts.tpl = child;
    ts.amount = amount;
    idx = templatedContents.Push (ts);
  }
  else
  {
    templatedContents[idx].amount += amount;
  }

  // First try if everything is ok.
  MarkDirty (0);
  if (!TestConstraints (0))
  {
    // Constraints are not ok. Undo our change.
    MarkDirty (0);
    if (newlyadded)
      templatedContents.DeleteIndex (idx);
    else
      templatedContents[idx].amount -= amount;

    //if(space)
      //space->RemoveEntity(child);
    return false;
  }

  // Send messages.
  atBaseline = false;
  FireInventoryListenersAdd (child, amount);
  if (entity)
  {
    params->GetParameter (0).Set (child->GetName ());
    params->GetParameter (1).Set (amount);
    if (!dispatcher_add)
      dispatcher_add = entity->QueryMessageChannel ()->CreateMessageDispatcher (
	    this, "cel.entity.add.template");
    if (dispatcher_add)
      dispatcher_add->SendMessage (params);
  }

  return true;
}

bool celPcInventory::AddEntity (iCelEntity* child, iCelParameterBlock* pparams)
{
  if (contents.Find (child) != csArrayItemNotFound) return true;

  if ((!allowedClasses.IsEmpty ()) && !allowedClasses.TestIntersect (
        child->GetClasses ()))
    return false;

  if (space)
  {
    bool ret = space->AddEntity (child, pparams);
    if (!ret) return false;
  }

  // Add our child. We will later test if this is valid and if
  // not undo this change.
  size_t idx = contents.Push (child);
  csRef<iPcCharacteristics> pcchar = celQueryPropertyClassEntity<iPcCharacteristics> (child);
  if (pcchar)
    pcchar->AddToInventory ((iPcInventory*)this);

  // First try if everything is ok.
  MarkDirty (0);
  if (!TestConstraints (0))
  {
    // Constraints are not ok. Undo our change.
    MarkDirty (0);
    contents.DeleteIndex (idx);
    if (pcchar)
      pcchar->RemoveFromInventory ((iPcInventory*)this);

    if(space)
      space->RemoveEntity(child);
    return false;
  }

  // Send messages.
  atBaseline = false;
  FireInventoryListenersAdd (child);
  iCelBehaviour* bh;
  if (entity)
  {
    params->GetParameter (0).Set (child);
    params->GetParameter (1).Set (1);
    bh = entity->GetBehaviour ();
    if (bh)
    {
      celData ret;
      bh->SendMessage ("pcinventory_addchild", this, ret, params);
    }
    if (!dispatcher_add)
      dispatcher_add = entity->QueryMessageChannel ()->CreateMessageDispatcher (
	    this, "cel.entity.add");
    if (dispatcher_add)
      dispatcher_add->SendMessage (params);
  }

  params->GetParameter (0).Set (entity);
  params->GetParameter (1).Set (1);
  bh = child->GetBehaviour ();
  if (bh)
  {
    celData ret;
    bh->SendMessage ("pcinventory_added", this, ret, params);
  }
  // Direct message since the child is always different so we can't
  // easily cache the dispatcher.
  child->QueryMessageChannel ()->SendMessage ("cel.entity.add.this", this,
      params);

  return true;
}

bool celPcInventory::RemoveEntity (iCelEntity* child)
{
  size_t idx = contents.Find (child);
  if (idx == csArrayItemNotFound) return true;

  if(space)
  {
    bool ret = space->RemoveEntity(child);
    if(!ret)
      return false;
  }

  // Remove our child. We will later test if this is valid and if
  // not undo this change.
  // make sure the entity isn't deleted too early
  csRef<iCelEntity> childref = child;
  contents.DeleteIndex (idx);
  csRef<iPcCharacteristics> pcchar = celQueryPropertyClassEntity<iPcCharacteristics> (child);
  if (pcchar)
    pcchar->RemoveFromInventory ((iPcInventory*)this);

  // First try if everything is ok.
  MarkDirty (0);
  if (!TestConstraints (0))
  {
    // Constraints are not ok. Undo our change.
    MarkDirty (0);
    contents.Push (child);
    if (pcchar)
      pcchar->AddToInventory ((iPcInventory*)this);
    if (space)
      space->AddEntity(child);
    return false;
  }

  // Send messages.
  atBaseline = false;
  FireInventoryListenersRemove (child);
  iCelBehaviour* bh;
  if (entity)
  {
    params->GetParameter (0).Set (child);
    params->GetParameter (1).Set (1);
    bh = entity->GetBehaviour ();
    if (bh)
    {
      celData ret;
      bh->SendMessage ("pcinventory_removechild", this, ret, params);
    }
    if (!dispatcher_remove)
      dispatcher_remove = entity->QueryMessageChannel ()
	->CreateMessageDispatcher (this, "cel.entity.remove");
    if (dispatcher_remove)
      dispatcher_remove->SendMessage (params);
  }
  params->GetParameter (0).Set (entity);
  params->GetParameter (1).Set (1);
  bh = child->GetBehaviour ();
  if (bh)
  {
    celData ret;
    bh->SendMessage ("pcinventory_removed", this, ret, params);
  }
  // Direct message since the child is always different so we can't
  // easily cache the dispatcher.
  child->QueryMessageChannel ()->SendMessage ("cel.entity.remove.this",
      this, params);

  return true;
}

bool celPcInventory::RemoveEntityTemplate (iCelEntityTemplate* child, int amount)
{
  size_t idx = FindEntityTemplate (child);
  if (idx == csArrayItemNotFound) return false;

  if (amount == 0) return true; // This works allways.

  //if (space)
  //{
    //bool ret = space->RemoveEntity(child);
    //if(!ret)
      //return false;
  //}

  // Remove our child. We will later test if this is valid and if
  // not undo this change.
  // make sure the entity isn't deleted too early
  csRef<iCelEntityTemplate> childref = child;
  int oldAmount = templatedContents[idx].amount;

  templatedContents[idx].amount -= amount;
  if (templatedContents[idx].amount < 0) templatedContents[idx].amount = 0;

  // First try if everything is ok.
  MarkDirty (0);
  if (!TestConstraints (0))
  {
    // Constraints are not ok. Undo our change.
    MarkDirty (0);
    templatedContents[idx].amount = oldAmount;
    //if (space)
      //space->AddEntity(child);
    return false;
  }
  else
  {
    if (templatedContents[idx].amount == 0)
      templatedContents.DeleteIndex (idx);
  }

  // Send messages.
  atBaseline = false;
  FireInventoryListenersRemove (child, amount);
  if (entity)
  {
    params->GetParameter (0).Set (child);
    params->GetParameter (1).Set (amount);
    if (!dispatcher_remove)
      dispatcher_remove = entity->QueryMessageChannel ()
	->CreateMessageDispatcher (this, "cel.entity.remove.template");
    if (dispatcher_remove)
      dispatcher_remove->SendMessage (params);
  }

  return true;
}

bool celPcInventory::RemoveEntity (iCelParameterBlock* pparams)
{
  csRef<iCelEntity> child;
  if (space)
    child = GetEntitySlot (pparams);
  else
    return false;

  if (!child)
    return false;

  bool ret = space->RemoveEntity(child);
  if (!ret)
    return false;

  size_t idx = contents.Find (child);
  if (idx == csArrayItemNotFound) return true;

  // Remove our child. We will later test if this is valid and if
  // not undo this change.
  // make sure the entity isn't deleted too early
  csRef<iCelEntity> childref = child;
  contents.DeleteIndex (idx);
  csRef<iPcCharacteristics> pcchar = celQueryPropertyClassEntity<iPcCharacteristics> (child);
  if (pcchar)
    pcchar->RemoveFromInventory ((iPcInventory*)this);

  // First try if everything is ok.
  MarkDirty (0);
  if (!TestConstraints (0))
  {
    // Constraints are not ok. Undo our change.
    MarkDirty (0);
    contents.Push (child);
    if (pcchar)
      pcchar->AddToInventory ((iPcInventory*)this);

    space->AddEntity(child);
    return false;
  }

  // Send messages.
  atBaseline = false;
  FireInventoryListenersRemove (child);
  iCelBehaviour* bh;
  if (entity)
  {
    params->GetParameter (0).Set (child);
    params->GetParameter (1).Set (1);
    bh = entity->GetBehaviour ();
    if (bh)
    {
      celData ret;
      bh->SendMessage ("pcinventory_removechild", this, ret, params);
    }
    if (!dispatcher_remove)
      dispatcher_remove = entity->QueryMessageChannel ()
	->CreateMessageDispatcher (this, "cel.entity.remove");
    if (dispatcher_remove)
      dispatcher_remove->SendMessage (params);
  }
  params->GetParameter (0).Set (entity);
  params->GetParameter (1).Set (1);
  bh = child->GetBehaviour ();
  if (bh)
  {
    celData ret;
    bh->SendMessage ("pcinventory_removed", this, ret, params);
  }
  // Direct message since the child is always different so we can't
  // easily cache the dispatcher.
  child->QueryMessageChannel ()->SendMessage ("cel.entity.remove.this",
      this, params);

  return true;
}

bool celPcInventory::RemoveAll ()
{
  while (contents.GetSize () > 0)
  {
    if (!RemoveEntity (contents[0])) return false;
  }
  while (templatedContents.GetSize () > 0)
  {
    TemplateStack& ts = templatedContents[0];
    if (!RemoveEntityTemplate (ts.tpl, ts.amount)) return false;
  }
  generatorActive = false;
  generator = 0;

  if (space) space->RemoveAll();
  atBaseline = false;

  return true;
}

iCelEntity* celPcInventory::GetEntity (size_t idx) const
{
  // Loot doesn't affect entities (only templates).
  CS_ASSERT ((idx != csArrayItemNotFound) && idx < contents.GetSize ());
  iCelEntity* ent = (iCelEntity*)contents[idx];
  return ent;
}

bool celPcInventory::In (iCelEntity* entity) const
{
  // Loot doesn't affect entities (only templates).
  return contents.Contains (entity) != csArrayItemNotFound;
}

size_t celPcInventory::FindEntity (iCelEntity* entity) const
{
  return contents.Contains (entity);
}

bool celPcInventory::In (const char* name) const
{
  size_t idx;
  idx = FindEntity (name);
  if (idx != csArrayItemNotFound) return true;

  idx = FindEntityTemplate (name);
  return idx != csArrayItemNotFound;
}

size_t celPcInventory::FindEntity (const char* name) const
{
  for (size_t i = 0 ; i < contents.GetSize () ; i++)
    if (contents[i]->GetName () && !strcmp (name, contents[i]->GetName ()))
      return i;
  return csArrayItemNotFound;
}

size_t celPcInventory::FindEntity (csStringID classid) const
{
  for (size_t i = 0 ; i < contents.GetSize () ; i++)
    if (contents[i]->HasClass(classid))
      return i;
  return csArrayItemNotFound;
}

size_t celPcInventory::GetEntityTemplateCount () const
{
  return templatedContents.GetSize ();
}

iCelEntityTemplate* celPcInventory::GetEntityTemplate (size_t idx) const
{
  return templatedContents[idx].tpl;
}

int celPcInventory::GetEntityTemplateAmount (size_t idx) const
{
  return templatedContents[idx].amount;
}

bool celPcInventory::In (iCelEntityTemplate* tpl) const
{
  return FindEntityTemplate (tpl) != csArrayItemNotFound;
}

size_t celPcInventory::FindEntityTemplate (iCelEntityTemplate* tpl) const
{
  for (size_t i = 0 ; i < templatedContents.GetSize () ; i++)
    if (templatedContents[i].tpl == tpl)
      return i;
  return csArrayItemNotFound;
}

size_t celPcInventory::FindEntityTemplate (const char* name) const
{
  for (size_t i = 0 ; i < templatedContents.GetSize () ; i++)
    if (!strcmp (name, templatedContents[i].tpl->GetName ()))
      return i;
  return csArrayItemNotFound;
}

size_t celPcInventory::FindEntityTemplate (csStringID classid) const
{
  for (size_t i = 0 ; i < templatedContents.GetSize () ; i++)
    if (templatedContents[i].tpl->HasClass (classid))
      return i;
  return csArrayItemNotFound;
}

iCelEntity* celPcInventory::GetEntitySlot (iCelParameterBlock* pparams) const
{
  return space->GetEntity (pparams);
}

celPcInventory::constraint* celPcInventory::FindConstraint (
	const char* name) const
{
  size_t i;
  for (i = 0 ; i < constraints.GetSize () ; i++)
  {
    constraint* c = constraints[i];
    if (c->charName == name) return c;
  }
  return 0;
}

void celPcInventory::SetSpace (iCelInventorySpace* space)
{
  this->space = space;
}

iCelInventorySpace* celPcInventory::GetSpace ()
{
  return space;
}

celPcInventory::constraint* celPcInventory::NewConstraint (const char* name)
{
  constraint* c = new constraint ();
  constraints.Push (c);
  c->charName = name;
  c->strict = false;
  c->totalMaxValue = 1000000000.;
  c->minValue = -1000000000.;
  c->maxValue = 1000000000.;
  c->currentValue = 0.;
  c->dirty = true;
  return c;
}

bool celPcInventory::SetStrictCharacteristics (const char* charName,
	bool strict)
{
  constraint* c = FindConstraint (charName);
  if (!c) c = NewConstraint (charName);
  bool old_strict = c->strict;
  c->strict = strict;
  if (!strict) return true;
  if (old_strict) return true;
  // Else we need to check if all entities satisfy the new strict condition.
  if (!TestConstraints (charName))
  {
    // Undo.
    c->strict = old_strict;
    return false;
  }
  return true;
}

bool celPcInventory::HasStrictCharacteristics (const char* charName) const
{
  constraint* c = FindConstraint (charName);
  if (c) return c->strict;
  else return false;
}

bool celPcInventory::SetConstraints (const char* charName,
	float minValue, float maxValue, float totalMaxValue)
{
  constraint* c = FindConstraint (charName);
  if (!c) c = NewConstraint (charName);
  float old_minValue = c->minValue;
  float old_maxValue = c->maxValue;
  float old_totalMaxValue = c->totalMaxValue;
  c->minValue = minValue;
  c->maxValue = maxValue;
  c->totalMaxValue = totalMaxValue;

  // Check if valid.
  if (!TestConstraints (charName))
  {
    // Undo.
    c->minValue = old_minValue;
    c->maxValue = old_maxValue;
    c->totalMaxValue = old_totalMaxValue;
    return false;
  }
  return true;
}

bool celPcInventory::GetConstraints (const char* charName,
	float& minValue, float& maxValue, float& totalMaxValue) const
{
  constraint* c = FindConstraint (charName);
  if (!c) return false;
  minValue = c->minValue;
  maxValue = c->maxValue;
  totalMaxValue = c->totalMaxValue;
  return true;
}

void celPcInventory::RemoveConstraints (const char* charName)
{
  size_t i;
  for (i = 0 ; i < constraints.GetSize () ; i++)
  {
    constraint* c = constraints[i];
    if (c->charName == charName)
    {
      constraints.DeleteIndex (i);
      return;
    }
  }
}

void celPcInventory::RemoveAllConstraints ()
{
  constraints.DeleteAll ();
}

float celPcInventory::GetCurrentCharacteristic (const char* charName) const
{
  constraint* c = FindConstraint (charName);
  if (!c) return 0.;
  if (c->dirty)
  {
    size_t i;
    c->currentValue = 0;
    for (i = 0 ; i < contents.GetSize () ; i++)
    {
      iCelEntity* child = contents[i];
      csRef<iPcCharacteristics> pcchar = celQueryPropertyClassEntity<iPcCharacteristics> (child);
      if (pcchar)
        c->currentValue += pcchar->GetCharacteristic (charName);
      else
        c->currentValue += DEF;
    }
    for (i = 0 ; i < templatedContents.GetSize () ; i++)
    {
      iCelEntityTemplate* child = templatedContents[i].tpl;
      int amount = templatedContents[i].amount;
      iTemplateCharacteristics* pcchar = child->GetCharacteristics ();
      c->currentValue += pcchar->GetCharacteristic (charName) * amount;
    }
    c->dirty = false;
  }
  return c->currentValue;
}

bool celPcInventory::TestLocalConstraints (const char* charName)
{
  if (charName && *charName)
  {
    //========
    // This case is for when a characteristic is given.
    //========
    constraint* c = 0;
    c = FindConstraint (charName);
    float minValue, maxValue, totalMaxValue;
    bool strict;
    if (c)
    {
      minValue = c->minValue;
      maxValue = c->maxValue;
      totalMaxValue = c->totalMaxValue;
      strict = c->strict;
    }
    else
    {
      minValue = -10000000000.0f;
      maxValue = 10000000000.0f;
      totalMaxValue = 100000000000.0f;
      strict = false;
    }
    float curValue = 0;
    size_t i;
    for (i = 0 ; i < contents.GetSize () ; i++)
    {
      iCelEntity* child = (iCelEntity*)contents[i];
      csRef<iPcCharacteristics> pcchar = celQueryPropertyClassEntity<iPcCharacteristics> (child);
      float child_val = DEF;
      if (pcchar && pcchar->HasCharacteristic (charName))
      {
        child_val = pcchar->GetCharacteristic (charName);
      }
      else if (strict)
      {
        // If this constraint is strict we fail here because this
        // child doesn't have this characteristic.
        return false;
      }

      if (child_val < minValue || child_val > maxValue) return false;
      curValue += child_val;
      if (child_val > totalMaxValue) return false;
    }
    for (i = 0 ; i < templatedContents.GetSize () ; i++)
    {
      iCelEntityTemplate* child = templatedContents[i].tpl;
      int amount = templatedContents[i].amount;
      iTemplateCharacteristics* pcchar = child->GetCharacteristics ();
      float child_val = DEF;
      if (pcchar->HasCharacteristic (charName))
      {
        child_val = amount * pcchar->GetCharacteristic (charName);
      }
      else if (strict)
      {
        // If this constraint is strict we fail here because this
        // child doesn't have this characteristic.
        return false;
      }

      if (child_val < minValue || child_val > maxValue) return false;
      curValue += child_val;
      if (child_val > totalMaxValue) return false;
    }
  }
  else
  {
    //========
    // This case is for when no characteristic is given.
    //========
    size_t i;
    for (i = 0 ; i < constraints.GetSize () ; i++)
    {
      constraint* c = constraints[i];
      if (!TestLocalConstraints (c->charName)) return false;
    }
  }

  return true;
}

bool celPcInventory::TestConstraints (const char* charName)
{
  if (!TestLocalConstraints (charName)) return false;
  if (!entity) return true;

  // Local contents seems to be ok. No check if this entity
  // also has characteristics and in that case check constraints
  // for that too.
  csRef<iPcCharacteristics> pcchar = celQueryPropertyClassEntity<iPcCharacteristics> (entity);
  if (pcchar)
  {
    bool rc = pcchar->TestConstraints (charName);
    return rc;
  }

  return true;
}

void celPcInventory::MarkDirty (const char* name)
{
  constraint* c = 0;
  if (name)
  {
    c = FindConstraint (name);
    if (c) c->dirty = true;
  }
  else
  {
    size_t i;
    for (i = 0 ; i < constraints.GetSize () ; i++)
    {
      constraint* c = constraints[i];
      c->dirty = true;
    }
  }
  if (!entity) return;
  csRef<iPcCharacteristics> pcchar = celQueryPropertyClassEntity<iPcCharacteristics> (entity);
  if (pcchar)
    pcchar->MarkDirty (name);
}

void celPcInventory::Dump ()
{
  size_t i;
  printf ("Inventory for entity '%s'\n", entity->GetName ());
  printf ("Constraints:\n");
  for (i = 0 ; i < constraints.GetSize () ; i++)
  {
    constraint* c = constraints[i];
    printf ("  '%s' min=%g max=%g totMax=%g current=%g strict=%d\n",
    	    (const char*)(c->charName), c->minValue, c->maxValue, c->totalMaxValue,
    	    GetCurrentCharacteristic (c->charName), c->strict);
  }
  printf ("Entities:\n");
  for (i = 0 ; i < contents.GetSize () ; i++)
  {
    iCelEntity* ent = (iCelEntity*)contents[i];
    printf ("  '%s'\n", ent->GetName ());
  }
  for (i = 0 ; i < templatedContents.GetSize () ; i++)
  {
    iCelEntityTemplate* tpl = templatedContents[i].tpl;
    printf ("  '%s'/%d\n", tpl->GetName (), templatedContents[i].amount);
  }
  fflush (stdout);
}

void celPcInventory::AddInventoryListener (iPcInventoryListener* listener)
{
  listeners.Push (listener);
}

void celPcInventory::RemoveInventoryListener (iPcInventoryListener* listener)
{
  listeners.Delete (listener);
}

void celPcInventory::FireInventoryListenersAdd (iCelEntityTemplate* tpl, int amount)
{
  size_t i = listeners.GetSize ();
  while (i > 0)
  {
    i--;
    listeners[i]->AddChildTemplate ((iPcInventory*)this, tpl, amount);
  }
}

void celPcInventory::FireInventoryListenersRemove (iCelEntityTemplate* tpl, int amount)
{
  size_t i = listeners.GetSize ();
  while (i > 0)
  {
    i--;
    listeners[i]->RemoveChildTemplate ((iPcInventory*)this, tpl, amount);
  }
}

void celPcInventory::FireInventoryListenersAdd (iCelEntity* entity)
{
  size_t i = listeners.GetSize ();
  while (i > 0)
  {
    i--;
    listeners[i]->AddChild ((iPcInventory*)this, entity);
  }
}

void celPcInventory::FireInventoryListenersRemove (iCelEntity* entity)
{
  size_t i = listeners.GetSize ();
  while (i > 0)
  {
    i--;
    listeners[i]->RemoveChild ((iPcInventory*)this, entity);
  }
}

void celPcInventory::AddAllowedClass (csStringID cls)
{
  allowedClasses.Add (cls);
}

void celPcInventory::ClearAllowedClasses ()
{
  allowedClasses.Empty ();
}

bool celPcInventory::IsClassAllowed (csStringID cls) const
{
  if (allowedClasses.IsEmpty ()) return true;
  return allowedClasses.Contains (cls);
}

void celPcInventory::SetLootGenerator (iLootGenerator* generator)
{
  celPcInventory::generator = generator;
  generatorActive = true;
  atBaseline = false;
}

//---------------------------------------------------------------------------

csStringID celPcCharacteristics::id_name = csInvalidStringID;
PropertyHolder celPcCharacteristics::propinfo;

celPcCharacteristics::celPcCharacteristics (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.characteristics.action.");
    AddAction (action_hascharacteristic, "HasCharacteristic");
  }

  if (id_name == csInvalidStringID)
  {
    id_name = pl->FetchStringID ("name");
  }
}

celPcCharacteristics::~celPcCharacteristics ()
{
}

bool celPcCharacteristics::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  if (idx == action_hascharacteristic)
  {
    CEL_FETCH_STRING_PAR (name,params,id_name);
    if (!p_name) return false;
    bool rc = HasCharacteristic (name);
    ret.Set (rc);
    return true;
  }
  return false;
}

bool celPcCharacteristics::SetProperty (csStringID propertyId, float b)
{
  const char* property = pl->FetchString (propertyId);
  if (property)
  {
    return SetCharacteristic (property, b);
  }

  return celPcCommon::SetProperty (propertyId, b);
}

float celPcCharacteristics::GetPropertyFloat (csStringID propertyId)
{
  const char* property = pl->FetchString (propertyId);
  if (property)
  {
    return GetLocalCharacteristic (property);
  }
  return celPcCommon::GetPropertyFloatByID (propertyId);
}

celPcCharacteristics::charact* celPcCharacteristics::FindCharact (
	const char* name) const
{
  size_t i;
  for (i = 0 ; i < chars.GetSize () ; i++)
  {
    charact* c = chars[i];
    if (!strcmp (name, c->name)) return c;
  }
  return 0;
}

bool celPcCharacteristics::TestConstraints (const char* charName)
{
  size_t i;
  for (i = 0 ; i < inventories.GetSize () ; i++)
  {
    iPcInventory* inv = inventories[i];
    if (!inv->TestConstraints (charName)) return false;
  }
  return true;
}

void celPcCharacteristics::MarkDirty (const char* charName)
{
  size_t i;
  for (i = 0 ; i < inventories.GetSize () ; i++)
  {
    iPcInventory* inv = inventories[i];
    inv->MarkDirty (charName);
  }
}

bool celPcCharacteristics::SetCharacteristic (const char* name, float value)
{
  charact* c = FindCharact (name);
  if (!c) { c = new charact (); chars.Push (c); c->name = csStrNew (name); }

  // Remember the old value and then set the new value. After
  // that we will test if inventories are ok.
  float old_value = c->value;
  c->value = value;

  // First we mark all inventories that are a direct or indirect parent
  // of this entity as dirty.
  MarkDirty (name);

  // If there is a violation we must undo our operation.
  // Otherwise we can remain as we are.
  if (!TestConstraints (name))
  {
    c->value = old_value;
    // Mark parents as dirty again.
    MarkDirty (name);
    return false;
  }

  return true;
}

bool celPcCharacteristics::SetInheritedCharacteristic (const char* name,
	float factor, float add)
{
  charact* c = FindCharact (name);
  if (!c) { c = new charact (); chars.Push (c); c->name = csStrNew (name); }
  float old_factor = factor;
  float old_add = add;
  c->factor = factor;
  c->add = add;

  MarkDirty (name);
  if (!TestConstraints (name))
  {
    MarkDirty (name);
    c->factor = old_factor;
    c->add = old_add;
    return false;
  }
  return true;
}

float celPcCharacteristics::GetCharacteristic (const char* name) const
{
  return GetLocalCharacteristic (name) + GetInheritedCharacteristic (name);
}

float celPcCharacteristics::GetLocalCharacteristic (const char* name) const
{
  charact* c = FindCharact (name);
  if (c) return c->value;
  return 0;
}

float celPcCharacteristics::GetInheritedCharacteristic (const char* name) const
{
  charact* c = FindCharact (name);
  float factor = 0, add = 0;
  if (c) { factor = c->factor; add = c->add; }

  if (ABS (factor) < SMALL_EPSILON) return add;

  csRef<iPcInventory> pcinv = celQueryPropertyClassEntity<iPcInventory> (entity);
  if (pcinv)
  {
    float invval = pcinv->GetCurrentCharacteristic (name);
    return invval * factor + add;
  }
  return add;
}

bool celPcCharacteristics::HasCharacteristic (const char* name) const
{
  charact* c = FindCharact (name);
  return c != 0;
}

bool celPcCharacteristics::ClearCharacteristic (const char* name)
{
  size_t i;
  for (i = 0 ; i < chars.GetSize () ; i++)
  {
    charact* c = chars[i];
    if (!strcmp (name, c->name))
    {
      chars.Extract (i);
      // First test if this doesn't invalidate constraints.
      MarkDirty (name);
      if (!TestConstraints (name))
      {
        // Undo our change.
        MarkDirty (name);
        chars.Push (c);
        return false;
      }

      delete[] c->name;
      delete c;
      return true;
    }
  }

  return true;
}

bool celPcCharacteristics::ClearAll ()
{
  while (chars.GetSize () > 0)
  {
    charact* c = chars[0];
    if (!ClearCharacteristic (c->name)) return false;
  }
  return true;
}

void celPcCharacteristics::AddToInventory (iPcInventory* inv)
{
  if (inventories.Find (inv) != csArrayItemNotFound) return;
  inventories.Push (inv);
}

void celPcCharacteristics::RemoveFromInventory (iPcInventory* inv)
{
  inventories.Delete (inv);
}

void celPcCharacteristics::Dump ()
{
  printf ("Characteristics for entity '%s'\n", entity->GetName ());
  printf ("Characteristics:\n");
  size_t i;
  for (i = 0 ; i < chars.GetSize () ; i++)
  {
    charact* c = chars[i];
    printf ("  '%s' value=%g, local value=%g factor=%g add=%g\n", c->name,
    	    GetCharacteristic (c->name), c->value, c->factor, c->add);
  }
  printf ("Inventories:\n");
  for (i = 0 ; i < inventories.GetSize () ; i++)
  {
    iPcInventory* inv = inventories[i];
    csRef<iCelPropertyClass> pc (
    	scfQueryInterface<iCelPropertyClass> (inv));
    if (pc)
      printf ("  '%s'\n", pc->GetEntity ()->GetName ());
  }
}
