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
#include "csutil/debug.h"
#include "iutil/objreg.h"
#include "plugins/propclass/inv/invfact.h"
#include "physicallayer/pl.h"
#include "physicallayer/datatype.h"
#include "physicallayer/persist.h"
#include "physicallayer/entity.h"
#include "behaviourlayer/behave.h"
#include "ivaria/reporter.h"

//---------------------------------------------------------------------------

#define DEF 0

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (Inventory, "pcinventory")
CEL_IMPLEMENT_FACTORY (Characteristics, "pccharacteristics")

void Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.persistence",
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

celPcInventory::celPcInventory (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  DG_TYPE (this, "celPcInventory()");

  if (id_entity == csInvalidStringID)
    id_entity = pl->FetchStringID ("cel.parameter.entity");
  params = new celOneParameterBlock ();
  params->SetParameterDef (id_entity, "entity");
}

celPcInventory::~celPcInventory ()
{
  RemoveAllConstraints ();
  delete params;
}

#define INVENTORY_SERIAL 1

csPtr<iCelDataBuffer> celPcInventory::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (INVENTORY_SERIAL);
  size_t i;
  databuf->Add ((uint16)constraints.Length ());
  for (i = 0 ; i < constraints.Length () ; i++)
  {
    constraint* c = constraints[i];
    databuf->Add (c->charName);
    databuf->Add (c->minValue);
    databuf->Add (c->maxValue);
    databuf->Add (c->totalMaxValue);
    databuf->Add (c->strict);
  }
  databuf->Add ((uint16)contents.Length ());
  for (i = 0 ; i < contents.Length () ; i++)
  {
    iCelEntity* ent = (iCelEntity*)contents[i];
    databuf->Add (ent);
  }
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcInventory::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != INVENTORY_SERIAL)
  {
    Report (object_reg,"serialnr != INVENTORY_SERIAL.  Cannot load.");
    return false;
  }

  RemoveAllConstraints ();
  RemoveAll ();

  int i;
  int cnt_constraints = databuf->GetUInt16 ();
  for (i = 0 ; i < cnt_constraints ; i++)
  {
    const char* cname = databuf->GetString ()->GetData ();
    if (!cname)
    {
      Report (object_reg, "Constraint name not specified for record %d!", i);
      return false;
    }
    constraint* c = NewConstraint (cname);
    if (!c)
    {
      Report (object_reg, "Constraint name is 0 for record %d!", i);
      return false;
    }
    c->minValue = databuf->GetFloat ();
    c->maxValue = databuf->GetFloat ();
    c->totalMaxValue = databuf->GetFloat ();
    c->strict = databuf->GetBool ();
    c->dirty = true;
  }

  int cnt_contents = databuf->GetUInt16 ();
  for (i = 0 ; i < cnt_contents ; i++)
  {
    iCelEntity* ent = databuf->GetEntity ();
    contents.Push (ent);
    csRef<iPcCharacteristics> pcchar = CEL_QUERY_PROPCLASS_ENT (
  	ent, iPcCharacteristics);
    if (pcchar)
      pcchar->AddToInventory ((iPcInventory*)this);
  }

  return true;
}

bool celPcInventory::AddEntity (iCelEntity* child)
{
  if (contents.Find (child) != csArrayItemNotFound) return true;

  if(space)
  {
    bool ret = space->AddEntity(child);
    if(!ret)
      return false;
  }

  // Add our child. We will later test if this is valid and if
  // not undo this change.
  size_t idx = contents.Push (child);
  DG_LINK (this, child->QueryObject ());
  csRef<iPcCharacteristics> pcchar (CEL_QUERY_PROPCLASS (
  	child->GetPropertyClassList (), iPcCharacteristics));
  if (pcchar)
    pcchar->AddToInventory ((iPcInventory*)this);

  // First try if everything is ok.
  MarkDirty (0);
  if (!TestConstraints (0))
  {
    // Constraints are not ok. Undo our change.
    MarkDirty (0);
    contents.DeleteIndex (idx);
    DG_UNLINK (this, child->QueryObject ());
    if (pcchar)
      pcchar->RemoveFromInventory ((iPcInventory*)this);

    if(space)
      space->RemoveEntity(child);
    return false;
  }

  // Send messages.
  FireInventoryListenersAdd (child);
  iCelBehaviour* bh;
  if (entity)
  {
    bh = entity->GetBehaviour ();
    if (bh)
    {
      params->GetParameter (0).Set (child);
      celData ret;
      bh->SendMessage ("pcinventory_addchild", this, ret, params);
    }
  }
  bh = child->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set (entity);
    celData ret;
    bh->SendMessage ("pcinventory_added", this, ret, params);
  }

  return true;
}

bool celPcInventory::AddEntity (iCelEntity* child, iCelParameterBlock* pparams)
{

  if (contents.Find (child) != csArrayItemNotFound) return true;

  if(space)
  {
    bool ret = space->AddEntity(child, pparams);
    if(!ret)
      return false;
  }

  // Add our child. We will later test if this is valid and if
  // not undo this change.
  size_t idx = contents.Push (child);
  DG_LINK (this, child->QueryObject ());
  csRef<iPcCharacteristics> pcchar (CEL_QUERY_PROPCLASS (
    child->GetPropertyClassList (), iPcCharacteristics));
  if (pcchar)
    pcchar->AddToInventory ((iPcInventory*)this);

  // First try if everything is ok.
  MarkDirty (0);
  if (!TestConstraints (0))
  {
    // Constraints are not ok. Undo our change.
    MarkDirty (0);
    contents.DeleteIndex (idx);
    DG_UNLINK (this, child->QueryObject ());
    if (pcchar)
      pcchar->RemoveFromInventory ((iPcInventory*)this);

    if(space)
      space->RemoveEntity(child);
    return false;
  }

  // Send messages.
  FireInventoryListenersAdd (child);
  iCelBehaviour* bh;
  if (entity)
  {
    bh = entity->GetBehaviour ();
    if (bh)
    {
      params->GetParameter (0).Set (child);
      celData ret;
      bh->SendMessage ("pcinventory_addchild", this, ret, params);
    }
  }
  bh = child->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set (entity);
    celData ret;
    bh->SendMessage ("pcinventory_added", this, ret, params);
  }

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
  DG_UNLINK (this, child->QueryObject ());
  csRef<iPcCharacteristics> pcchar (CEL_QUERY_PROPCLASS (
  	child->GetPropertyClassList (), iPcCharacteristics));
  if (pcchar)
    pcchar->RemoveFromInventory ((iPcInventory*)this);

  // First try if everything is ok.
  MarkDirty (0);
  if (!TestConstraints (0))
  {
    // Constraints are not ok. Undo our change.
    MarkDirty (0);
    contents.Push (child);
    DG_LINK (this, child->QueryObject ());
    if (pcchar)
      pcchar->AddToInventory ((iPcInventory*)this);
    if (space)
      space->AddEntity(child);
    return false;
  }

  // Send messages.
  FireInventoryListenersRemove (child);
  iCelBehaviour* bh;
  if (entity)
  {
    bh = entity->GetBehaviour ();
    if (bh)
    {
      params->GetParameter (0).Set (child);
      celData ret;
      bh->SendMessage ("pcinventory_removechild", this, ret, params);
    }
  }
  bh = child->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set (entity);
    celData ret;
    bh->SendMessage ("pcinventory_removed", this, ret, params);
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
  DG_UNLINK (this, child->QueryObject ());
  csRef<iPcCharacteristics> pcchar (CEL_QUERY_PROPCLASS (
    child->GetPropertyClassList (), iPcCharacteristics));
  if (pcchar)
    pcchar->RemoveFromInventory ((iPcInventory*)this);

  // First try if everything is ok.
  MarkDirty (0);
  if (!TestConstraints (0))
  {
    // Constraints are not ok. Undo our change.
    MarkDirty (0);
    contents.Push (child);
    DG_LINK (this, child->QueryObject ());
    if (pcchar)
      pcchar->AddToInventory ((iPcInventory*)this);

    space->AddEntity(child);
    return false;
  }

  // Send messages.
  FireInventoryListenersRemove (child);
  iCelBehaviour* bh;
  if (entity)
  {
    bh = entity->GetBehaviour ();
    if (bh)
    {
      params->GetParameter (0).Set (child);
      celData ret;
      bh->SendMessage ("pcinventory_removechild", this, ret, params);
    }
  }
  bh = child->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set (entity);
    celData ret;
    bh->SendMessage ("pcinventory_removed", this, ret, params);
  }

  return true;
}

bool celPcInventory::RemoveAll ()
{
  while (contents.Length () > 0)
  {
    if (!RemoveEntity ((iCelEntity*)contents[0])) return false;
  }

  if(space) space->RemoveAll();

  return true;
}

iCelEntity* celPcInventory::GetEntity (size_t idx) const
{
  CS_ASSERT ((idx != csArrayItemNotFound) && idx < contents.Length ());
  iCelEntity* ent = (iCelEntity*)contents[idx];
  return ent;
}

iCelEntity* celPcInventory::GetEntitySlot (iCelParameterBlock* pparams) const
{
  return space->GetEntity (pparams);
}

celPcInventory::constraint* celPcInventory::FindConstraint (
	const char* name) const
{
  size_t i;
  for (i = 0 ; i < constraints.Length () ; i++)
  {
    constraint* c = constraints[i];
    if (!strcmp (name, c->charName)) return c;
  }
  return 0;
}

void celPcInventory::SetSpace(iCelInventorySpace* space)
{
  this->space = space;
}

iCelInventorySpace* celPcInventory::GetSpace()
{
  return space;
}

celPcInventory::constraint* celPcInventory::NewConstraint (const char* name)
{
  constraint* c = new constraint ();
  constraints.Push (c);
  c->charName = csStrNew (name);
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
  for (i = 0 ; i < constraints.Length () ; i++)
  {
    constraint* c = constraints[i];
    if (!strcmp (charName, c->charName))
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
    for (i = 0 ; i < contents.Length () ; i++)
    {
      iCelEntity* child = (iCelEntity*)contents[i];
      csRef<iPcCharacteristics> pcchar (CEL_QUERY_PROPCLASS (
      	child->GetPropertyClassList (), iPcCharacteristics));
      if (pcchar)
	c->currentValue += pcchar->GetCharacteristic (charName);
      else
	c->currentValue += DEF;
    }
    c->dirty = false;
  }
  return c->currentValue;
}

bool celPcInventory::TestLocalConstraints (const char* charName)
{
  if (charName)
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
    for (i = 0 ; i < contents.Length () ; i++)
    {
      iCelEntity* child = (iCelEntity*)contents[i];
      csRef<iPcCharacteristics> pcchar (CEL_QUERY_PROPCLASS (
      	child->GetPropertyClassList (), iPcCharacteristics));
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
  }
  else
  {
    //========
    // This case is for when no characteristic is given.
    //========
    size_t i;
    for (i = 0 ; i < constraints.Length () ; i++)
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
  csRef<iPcCharacteristics> pcchar (CEL_QUERY_PROPCLASS (
  	entity->GetPropertyClassList (), iPcCharacteristics));
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
    for (i = 0 ; i < constraints.Length () ; i++)
    {
      constraint* c = constraints[i];
      c->dirty = true;
    }
  }
  if (!entity) return;
  csRef<iPcCharacteristics> pcchar (CEL_QUERY_PROPCLASS (
  	entity->GetPropertyClassList (), iPcCharacteristics));
  if (pcchar)
    pcchar->MarkDirty (name);
}

void celPcInventory::Dump ()
{
  size_t i;
  printf ("Inventory for entity '%s'\n", entity->GetName ());
  printf ("Constraints:\n");
  for (i = 0 ; i < constraints.Length () ; i++)
  {
    constraint* c = constraints[i];
    printf ("  '%s' min=%g max=%g totMax=%g current=%g strict=%d\n",
		    c->charName, c->minValue, c->maxValue, c->totalMaxValue,
		    GetCurrentCharacteristic (c->charName), c->strict);
  }
  printf ("Entities:\n");
  for (i = 0 ; i < contents.Length () ; i++)
  {
    iCelEntity* ent = (iCelEntity*)contents[i];
    printf ("  '%s'\n", ent->GetName ());
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

void celPcInventory::FireInventoryListenersAdd (iCelEntity* entity)
{
  size_t i = listeners.Length ();
  while (i > 0)
  {
    i--;
    listeners[i]->AddChild ((iPcInventory*)this, entity);
  }
}

void celPcInventory::FireInventoryListenersRemove (iCelEntity* entity)
{
  size_t i = listeners.Length ();
  while (i > 0)
  {
    i--;
    listeners[i]->RemoveChild ((iPcInventory*)this, entity);
  }
}

//---------------------------------------------------------------------------

celPcCharacteristics::celPcCharacteristics (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  DG_TYPE (this, "celPcCharacteristics()");
}

celPcCharacteristics::~celPcCharacteristics ()
{
}

#define CHARACTERISTICS_SERIAL 1

csPtr<iCelDataBuffer> celPcCharacteristics::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (CHARACTERISTICS_SERIAL);
  size_t i;
  databuf->Add ((uint16)chars.Length ());
  for (i = 0 ; i < chars.Length () ; i++)
  {
    charact* c = chars[i];
    databuf->Add (c->name);
    databuf->Add (c->value);
    databuf->Add (c->factor);
    databuf->Add (c->add);
  }
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcCharacteristics::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != CHARACTERISTICS_SERIAL)
  {
    Report (object_reg,"serialnr != CHARACTERISTICS_SERIAL.  Cannot load.");
    return false;
  }

  ClearAll ();

  int i;
  int cnt_chars = databuf->GetUInt16 ();
  for (i = 0 ; i < cnt_chars ; i++)
  {
    charact* c = new charact (); chars.Push (c);
    c->name = csStrNew (databuf->GetString ()->GetData ());
    c->value = databuf->GetFloat ();
    c->factor = databuf->GetFloat ();
    c->add = databuf->GetFloat ();
  }

  return true;
}

celPcCharacteristics::charact* celPcCharacteristics::FindCharact (
	const char* name) const
{
  size_t i;
  for (i = 0 ; i < chars.Length () ; i++)
  {
    charact* c = chars[i];
    if (!strcmp (name, c->name)) return c;
  }
  return 0;
}

bool celPcCharacteristics::TestConstraints (const char* charName)
{
  size_t i;
  for (i = 0 ; i < inventories.Length () ; i++)
  {
    iPcInventory* inv = inventories[i];
    if (!inv->TestConstraints (charName)) return false;
  }
  return true;
}

void celPcCharacteristics::MarkDirty (const char* charName)
{
  size_t i;
  for (i = 0 ; i < inventories.Length () ; i++)
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

  csRef<iPcInventory> pcinv (
  	CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (),
	iPcInventory));
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
  for (i = 0 ; i < chars.Length () ; i++)
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
  while (chars.Length () > 0)
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
  for (i = 0 ; i < chars.Length () ; i++)
  {
    charact* c = chars[i];
    printf ("  '%s' value=%g, local value=%g factor=%g add=%g\n", c->name,
		    GetCharacteristic (c->name), c->value, c->factor, c->add);
  }
  printf ("Inventories:\n");
  for (i = 0 ; i < inventories.Length () ; i++)
  {
    iPcInventory* inv = inventories[i];
    csRef<iCelPropertyClass> pc (
    	SCF_QUERY_INTERFACE (inv, iCelPropertyClass));
    if (pc)
      printf ("  '%s'\n", pc->GetEntity ()->GetName ());
  }
}
