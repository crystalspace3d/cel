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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "cssysdef.h"
#include "pf/inv/invfact.h"
#include "pl/entity.h"
#include "bl/entity.h"
#include "csutil/util.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celPfInventory)

SCF_EXPORT_CLASS_TABLE (pfinv)
  SCF_EXPORT_CLASS (celPfInventory, "cel.pcfactory.inventory",
  	"CEL Inventory Property Class Factory")
SCF_EXPORT_CLASS_TABLE_END

SCF_IMPLEMENT_IBASE (celPfInventory)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClassFactory)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPfInventory::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPfInventory::celPfInventory (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
}

celPfInventory::~celPfInventory ()
{
}

bool celPfInventory::Initialize (iObjectRegistry* /*object_reg*/)
{
  return true;
}

iCelPropertyClass* celPfInventory::CreatePropertyClass (const char* type)
{
  if (!strcmp (type, "pcinventory")) return new celPcInventory ();
  else if (!strcmp (type, "pccharacteristics")) return new celPcCharacteristics ();
  return NULL;
}

const char* celPfInventory::GetTypeName (int idx) const
{
  switch (idx)
  {
    case 0: return "pcinventory";
    case 1: return "pccharacteristics";
    default: return NULL;
  }
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celPcInventory)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClass)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcInventory)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcInventory::PcInventory)
  SCF_IMPLEMENTS_INTERFACE (iPcInventory)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcInventory::celPcInventory ()
{
  SCF_CONSTRUCT_IBASE (NULL);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcInventory);
}

celPcInventory::~celPcInventory ()
{
  RemoveAll ();
  while (constraints.Length () > 0)
  {
    constraint* c = (constraint*)constraints[0];
    delete[] c->charName;
    delete c;
    constraints.Delete (0);
  }
}

void celPcInventory::SetEntity (iCelEntity* entity)
{
  celPcInventory::entity = entity;
}

bool celPcInventory::AddEntity (iCelEntity* entity)
{
  if (contents.Find (entity) != -1) return true;
  if (TestAddEntity (entity) != NULL) return false;
  UpdateConstraints (entity, true);
  contents.Push (entity);
  entity->IncRef ();
  return true;
}

void celPcInventory::RemoveEntity (iCelEntity* entity)
{
  int idx = contents.Find (entity);
  if (idx == -1) return;
  UpdateConstraints (entity, false);
  contents.Delete (idx);
  entity->DecRef ();
}

void celPcInventory::RemoveAll ()
{
  while (contents.Length () > 0)
    RemoveEntity (0);
}

iCelEntity* celPcInventory::GetEntity (int idx) const
{
  CS_ASSERT (idx >= 0 && idx < contents.Length ());
  iCelEntity* ent = (iCelEntity*)contents[idx];
  return ent;
}

celPcInventory::constraint* celPcInventory::FindConstraint (const char* name) const
{
  int i;
  for (i = 0 ; i < constraints.Length () ; i++)
  {
    constraint* c = (constraint*)constraints[i];
    if (!strcmp (name, c->charName)) return c;
  }
  return NULL;
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
  return c;
}

void celPcInventory::SetStrictCharacteristics (const char* charName, bool strict)
{
  constraint* c = FindConstraint (charName);
  if (!c) c = NewConstraint (charName);
  c->strict = strict;
}

bool celPcInventory::HasStrictCharacteristics (const char* charName) const
{
  constraint* c = FindConstraint (charName);
  if (c) return c->strict;
  else return false;
}

void celPcInventory::SetConstraints (const char* charName, float minValue, float maxValue,
		  float totalMaxValue)
{
  constraint* c = FindConstraint (charName);
  if (!c) c = NewConstraint (charName);
  c->minValue = minValue;
  c->maxValue = maxValue;
  c->totalMaxValue = totalMaxValue;
}

bool celPcInventory::GetConstraints (const char* charName, float& minValue, float& maxValue,
		  float& totalMaxValue) const
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
  int i;
  for (i = 0 ; i < constraints.Length () ; i++)
  {
    constraint* c = (constraint*)constraints[i];
    if (!strcmp (charName, c->charName))
    {
      delete[] c->charName;
      delete c;
      constraints.Delete (i);
      return;
    }
  }
}

float celPcInventory::GetCurrentCharacteristic (const char* charName) const
{
  constraint* c = FindConstraint (charName);
  if (!c) return 0.;
  return c->currentValue;
}

void celPcInventory::UpdateConstraints (iCelEntity* entity, bool add)
{
  // This routine assumes the constraints are valid!!!
  if (constraints.Length () <= 0) return;
  iCelPropertyClass* pc = entity->GetPropertyClassList ()->FindByName ("pccharacteristics");
  if (!pc) return;
  iPcCharacteristics* pcchar = SCF_QUERY_INTERFACE (pc, iPcCharacteristics);
  CS_ASSERT (pcchar != NULL);
  int i;
  for (i = 0 ; i < constraints.Length () ; i++)
  {
    constraint* c = (constraint*)constraints[i];
    float value = pcchar->GetCharProperty (c->charName);
    if (add) c->currentValue += value;
    else c->currentValue -= value;
  }
  pcchar->DecRef ();
  return;
}

const char* celPcInventory::TestAddEntity (iCelEntity* entity)
{
  if (constraints.Length () <= 0) return NULL;
  iCelPropertyClass* pc = entity->GetPropertyClassList ()->FindByName ("pccharacteristics");
  iPcCharacteristics* pcchar = NULL;
  if (pc)
  {
    pcchar = SCF_QUERY_INTERFACE (pc, iPcCharacteristics);
    CS_ASSERT (pcchar != NULL);
  }
  int i;
  for (i = 0 ; i < constraints.Length () ; i++)
  {
    constraint* c = (constraint*)constraints[i];
    // If entity has no characteristics and the constraint is strict we fail.
    if (!pcchar && c->strict) return c->charName;
    if (pcchar)
    {
      bool hp = pcchar->HasProperty (c->charName);
      // If entity doesn't have property and constraint is strict we fail.
      if (!hp && c->strict) { pcchar->DecRef (); return c->charName; }
      if (hp)
      {
	float value = pcchar->GetCharProperty (c->charName);
	if (value < c->minValue || value > c->maxValue) { pcchar->DecRef (); return c->charName; }
	if (c->currentValue + value > c->totalMaxValue) { pcchar->DecRef (); return c->charName; }
      }
    }
  }
  if (pcchar) pcchar->DecRef ();
  return NULL;
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celPcCharacteristics)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClass)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcCharacteristics)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcCharacteristics::PcCharacteristics)
  SCF_IMPLEMENTS_INTERFACE (iPcCharacteristics)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcCharacteristics::celPcCharacteristics ()
{
  SCF_CONSTRUCT_IBASE (NULL);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcCharacteristics);
}

celPcCharacteristics::~celPcCharacteristics ()
{
  ClearAll ();
}

void celPcCharacteristics::SetEntity (iCelEntity* entity)
{
  celPcCharacteristics::entity = entity;
}

celPcCharacteristics::charact* celPcCharacteristics::FindCharact (const char* name) const
{
  int i;
  for (i = 0 ; i < chars.Length () ; i++)
  {
    charact* c = (charact*)chars[i];
    if (!strcmp (name, c->name)) return c;
  }
  return NULL;
}

void celPcCharacteristics::SetCharProperty (const char* name, float value)
{
  charact* c = FindCharact (name);
  if (!c) { c = new charact (); chars.Push (c); c->name = csStrNew (name); }
  c->value = value;
}

float celPcCharacteristics::GetCharProperty (const char* name) const
{
  charact* c = FindCharact (name);
  if (c) return c->value;
  return 0;
}

bool celPcCharacteristics::HasProperty (const char* name) const
{
  charact* c = FindCharact (name);
  return c != NULL;
}

void celPcCharacteristics::ClearProperty (const char* name)
{
  int i;
  for (i = 0 ; i < chars.Length () ; i++)
  {
    charact* c = (charact*)chars[i];
    if (!strcmp (name, c->name))
    {
      chars.Delete (i);
      delete[] c->name;
      delete c;
      return;
    }
  }
}

void celPcCharacteristics::ClearAll ()
{
  while (chars.Length () > 0)
  {
    charact* c = (charact*)chars[0];
    delete[] c->name;
    delete c;
    chars.Delete (0);
  }
}

//---------------------------------------------------------------------------

