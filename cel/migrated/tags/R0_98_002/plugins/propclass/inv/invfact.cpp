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
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.persistance",
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

SCF_IMPLEMENT_IBASE_EXT (celPcInventory)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcInventory)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcInventory::PcInventory)
  SCF_IMPLEMENTS_INTERFACE (iPcInventory)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

csStringID celPcInventory::id_entity = csInvalidStringID;

celPcInventory::celPcInventory (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcInventory);
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
  databuf->SetDataCount (
  	1+constraints.Length ()*5 +
  	1+contents.Length ());
  size_t i, j = 0;
  databuf->GetData (j++)->Set ((uint16)constraints.Length ());
  for (i = 0 ; i < (size_t)constraints.Length () ; i++)
  {
    constraint* c = constraints[i];
    databuf->GetData (j++)->Set (c->charName);
    databuf->GetData (j++)->Set (c->minValue);
    databuf->GetData (j++)->Set (c->maxValue);
    databuf->GetData (j++)->Set (c->totalMaxValue);
    databuf->GetData (j++)->Set (c->strict);
  }
  databuf->GetData (j++)->Set ((uint16)contents.Length ());
  for (i = 0 ; i < (size_t)contents.Length () ; i++)
  {
    iCelEntity* ent = (iCelEntity*)contents[i];
    databuf->GetData (j++)->Set (ent);
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
#ifdef CS_DEBUG
  int cnt_total = databuf->GetDataCount ();
#endif

  RemoveAllConstraints ();
  RemoveAll ();

  celData* cd;
  int i, j = 0;
  cd = databuf->GetData (j++);
  if (!cd)
  {
    Report (object_reg,"cnt_constraints is not specified.  Cannot load.");
    return false;
  }
  int cnt_constraints = cd->value.uw;
  for (i = 0 ; i < cnt_constraints ; i++)
  {
    cd = databuf->GetData (j++);
    if (!cd)
    {
      Report (object_reg,"constraint name not specified for record %d.  Cannot load.",i);
      return false;
    }
    constraint* c = NewConstraint (*cd->value.s);
    if (!c)
    {
      Report (object_reg,"constraint name is 0 for record %d.  Cannot load.",i);
      return false;
    }
    cd = databuf->GetData (j++);
    if (!cd)
    {
      Report (object_reg,"Min value not specified for record %d.  Cannot load.",i);
      return false;
    }
    c->minValue = cd->value.f;
    cd = databuf->GetData (j++);
    if (!cd)
    {
      Report (object_reg,"maxValue not specified for record %d.  Cannot load.",i);
      return false;
    }
    c->maxValue = cd->value.f;
    cd = databuf->GetData (j++);
    if (!cd)
    {
      Report (object_reg,"totalMaxValue not specified for record %d.  Cannot load.",i);
      return false;
    }
    c->totalMaxValue = cd->value.f;
    cd = databuf->GetData (j++);
    if (!cd)
    {
      Report (object_reg,"Strict flag not specified for record %d.  Cannot load.",i);
      return false;
    }
    c->strict = cd->value.bo;
    c->dirty = true;
  }

  cd = databuf->GetData (j++); 
  if (!cd)
  {
    Report (object_reg,"cnt_contents not specified.  Cannot load.");
    return false;
  }
  int cnt_contents = cd->value.uw;
  CS_ASSERT (cnt_total == 1+cnt_constraints*5 + 1+cnt_contents);
  for (i = 0 ; i < cnt_contents ; i++)
  {
    cd = databuf->GetData (j++);
    if (!cd)
    {
      Report (object_reg,"Contents entity not specified for record %d.  Cannot load.",i);
      return false;
    }
    contents.Push (cd->value.ent);
    DG_LINK (this, cd->value.ent->QueryObject ());
    csRef<iPcCharacteristics> pcchar (CEL_QUERY_PROPCLASS (
  	cd->value.ent->GetPropertyClassList (), iPcCharacteristics));
    if (pcchar)
      pcchar->AddToInventory (&scfiPcInventory);
  }

  return true;
}

bool celPcInventory::AddEntity (iCelEntity* child)
{
  if ((size_t)contents.Find (child) != csArrayItemNotFound) return true;

  // Add our child. We will later test if this is valid and if
  // not undo this change.
  int idx = contents.Push (child);
  DG_LINK (this, child->QueryObject ());
  csRef<iPcCharacteristics> pcchar (CEL_QUERY_PROPCLASS (
  	child->GetPropertyClassList (), iPcCharacteristics));
  if (pcchar)
    pcchar->AddToInventory (&scfiPcInventory);

  // First try if everything is ok.
  MarkDirty (0);
  if (!TestConstraints (0))
  {
    // Constraints are not ok. Undo our change.
    MarkDirty (0);
    contents.DeleteIndex (idx);
    DG_UNLINK (this, child->QueryObject ());
    if (pcchar)
      pcchar->RemoveFromInventory (&scfiPcInventory);
    return false;
  }

  // Send messages.
  iCelBehaviour* bh;
  if (entity)
  {
    bh = entity->GetBehaviour ();
    if (bh)
    {
      params->GetParameter (0).Set (child);
      celData ret;
      bh->SendMessage ("pcinventory_addchild", ret, params);
    }
  }
  bh = child->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set (entity);
    celData ret;
    bh->SendMessage ("pcinventory_added", ret, params);
  }

  return true;
}

bool celPcInventory::RemoveEntity (iCelEntity* child)
{
  int idx = contents.Find (child);
  if (idx == -1) return true;

  // Remove our child. We will later test if this is valid and if
  // not undo this change.
  // make sure the entity isn't deleted too early
  csRef<iCelEntity> childref = child;
  contents.DeleteIndex (idx);
  DG_UNLINK (this, child->QueryObject ());
  csRef<iPcCharacteristics> pcchar (CEL_QUERY_PROPCLASS (
  	child->GetPropertyClassList (), iPcCharacteristics));
  if (pcchar)
    pcchar->RemoveFromInventory (&scfiPcInventory);

  // First try if everything is ok.
  MarkDirty (0);
  if (!TestConstraints (0))
  {
    // Constraints are not ok. Undo our change.
    MarkDirty (0);
    contents.Push (child);
    DG_LINK (this, child->QueryObject ());
    if (pcchar)
      pcchar->AddToInventory (&scfiPcInventory);
    return false;
  }

  // Send messages.
  iCelBehaviour* bh;
  if (entity)
  {
    bh = entity->GetBehaviour ();
    if (bh)
    {
      params->GetParameter (0).Set (child);
      celData ret;
      bh->SendMessage ("pcinventory_removechild", ret, params);
    }
  }
  bh = child->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set (entity);
    celData ret;
    bh->SendMessage ("pcinventory_removed", ret, params);
  }

  return true;
}

bool celPcInventory::RemoveAll ()
{
  while (contents.Length () > 0)
    if (!RemoveEntity ((iCelEntity*)contents[0])) return false;
  return true;
}

iCelEntity* celPcInventory::GetEntity (size_t idx) const
{
  CS_ASSERT ((idx != csArrayItemNotFound) && idx < (size_t)contents.Length ());
  iCelEntity* ent = (iCelEntity*)contents[idx];
  return ent;
}

celPcInventory::constraint* celPcInventory::FindConstraint (
	const char* name) const
{
  size_t i;
  for (i = 0 ; i < (size_t)constraints.Length () ; i++)
  {
    constraint* c = constraints[i];
    if (!strcmp (name, c->charName)) return c;
  }
  return 0;
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
  for (i = 0 ; i < (size_t)constraints.Length () ; i++)
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
    for (i = 0 ; i < (size_t)contents.Length () ; i++)
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
    for (i = 0 ; i < (size_t)contents.Length () ; i++)
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
    for (i = 0 ; i < (size_t)constraints.Length () ; i++)
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
    for (i = 0 ; i < (size_t)constraints.Length () ; i++)
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
  for (i = 0 ; i < (size_t)constraints.Length () ; i++)
  {
    constraint* c = constraints[i];
    printf ("  '%s' min=%g max=%g totMax=%g current=%g strict=%d\n",
		    c->charName, c->minValue, c->maxValue, c->totalMaxValue,
		    GetCurrentCharacteristic (c->charName), c->strict);
  }
  printf ("Entities:\n");
  for (i = 0 ; i < (size_t)contents.Length () ; i++)
  {
    iCelEntity* ent = (iCelEntity*)contents[i];
    printf ("  '%s'\n", ent->GetName ());
  }
  fflush (stdout);
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE_EXT (celPcCharacteristics)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcCharacteristics)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcCharacteristics::PcCharacteristics)
  SCF_IMPLEMENTS_INTERFACE (iPcCharacteristics)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcCharacteristics::celPcCharacteristics (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcCharacteristics);
  DG_TYPE (this, "celPcCharacteristics()");
}

celPcCharacteristics::~celPcCharacteristics ()
{
}

#define CHARACTERISTICS_SERIAL 1

csPtr<iCelDataBuffer> celPcCharacteristics::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (CHARACTERISTICS_SERIAL);
  databuf->SetDataCount (
  	1+chars.Length ()*4);
  size_t i, j = 0;
  databuf->GetData (j++)->Set ((uint16)chars.Length ());
  for (i = 0 ; i < (size_t)chars.Length () ; i++)
  {
    charact* c = chars[i];
    databuf->GetData (j++)->Set (c->name);
    databuf->GetData (j++)->Set (c->value);
    databuf->GetData (j++)->Set (c->factor);
    databuf->GetData (j++)->Set (c->add);
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
#ifdef CS_DEBUG
  int cnt_total = databuf->GetDataCount ();
#endif

  ClearAll ();

  celData* cd;
  int i, j = 0;
  cd = databuf->GetData (j++);
  if (!cd)
  {
    Report (object_reg,"cnt_chars not specified.  Cannot load.");
    return false;
  }

  int cnt_chars = cd->value.uw;
  CS_ASSERT (cnt_total == 1+cnt_chars*4);
  for (i = 0 ; i < cnt_chars ; i++)
  {
    cd = databuf->GetData (j++);
    if (!cd)
    {
      Report (object_reg,"character name not specified for record %d.  Cannot load.",i);
      return false;
    }
    charact* c = new charact (); chars.Push (c);
    c->name = csStrNew (*cd->value.s);
    if (!c)
    {
      Report (object_reg,"Character name string not specified for record %d.  Cannot load.",i);
      return false;
    }
    cd = databuf->GetData (j++);
    if (!cd)
    {
      Report (object_reg,"Value not specified for record %d.  Cannot load.",i);
      return false;
    }
    c->value = cd->value.f;
    cd = databuf->GetData (j++);
    if (!cd)
    {
      Report (object_reg,"Factor not specified for record %d.  Cannot load.",i);
      return false;
    }
    c->factor = cd->value.f;
    cd = databuf->GetData (j++);
    if (!cd)
    {
      Report (object_reg,"Add not specified for record %d.  Cannot load.",i);
      return false;
    }
    c->add = cd->value.f;
  }

  return true;
}

celPcCharacteristics::charact* celPcCharacteristics::FindCharact (
	const char* name) const
{
  size_t i;
  for (i = 0 ; i < (size_t)chars.Length () ; i++)
  {
    charact* c = chars[i];
    if (!strcmp (name, c->name)) return c;
  }
  return 0;
}

bool celPcCharacteristics::TestConstraints (const char* charName)
{
  size_t i;
  for (i = 0 ; i < (size_t)inventories.Length () ; i++)
  {
    iPcInventory* inv = inventories[i];
    if (!inv->TestConstraints (charName)) return false;
  }
  return true;
}

void celPcCharacteristics::MarkDirty (const char* charName)
{
  size_t i;
  for (i = 0 ; i < (size_t)inventories.Length () ; i++)
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
  for (i = 0 ; i < (size_t)chars.Length () ; i++)
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
  if ((size_t)inventories.Find (inv) != csArrayItemNotFound) return;
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
  for (i = 0 ; i < (size_t)chars.Length () ; i++)
  {
    charact* c = chars[i];
    printf ("  '%s' value=%g, local value=%g factor=%g add=%g\n", c->name,
		    GetCharacteristic (c->name), c->value, c->factor, c->add);
  }
  printf ("Inventories:\n");
  for (i = 0 ; i < (size_t)inventories.Length () ; i++)
  {
    iPcInventory* inv = inventories[i];
    csRef<iCelPropertyClass> pc (
    	SCF_QUERY_INTERFACE (inv, iCelPropertyClass));
    if (pc)
      printf ("  '%s'\n", pc->GetEntity ()->GetName ());
  }
}

//---------------------------------------------------------------------------
