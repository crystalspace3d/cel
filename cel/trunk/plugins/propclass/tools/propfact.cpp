/*
    Crystal Space Entity Layer
    Copyright (C) 2001-2005 by Jorrit Tyberghein

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
#include "plugins/propclass/tools/propfact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/datatype.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "csutil/util.h"
#include "csutil/scanstr.h"
#include "csutil/debug.h"
#include "iutil/eventq.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/objreg.h"
#include "iutil/virtclk.h"
#include "ivideo/graph2d.h"
#include "ivideo/graph3d.h"
#include "ivideo/fontserv.h"
#include "ivideo/txtmgr.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY (Properties, "pcproperties")

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE_EXT (celPcProperties)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcProperties)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcProperties::PcProperties)
  SCF_IMPLEMENTS_INTERFACE (iPcProperties)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

csStringID celPcProperties::id_index = csInvalidStringID;

celPcProperties::celPcProperties (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcProperties);
  DG_TYPE (this, "celPcProperties()");
  if (id_index == csInvalidStringID)
    id_index = pl->FetchStringID ("cel.parameter.index");
  params = new celOneParameterBlock ();
  params->SetParameterDef (id_index, "index");
  properties_hash_dirty = false;
}

celPcProperties::~celPcProperties ()
{
  Clear ();
  delete params;
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcProperties);
}

size_t celPcProperties::FindProperty (csStringID id)
{
  size_t i;
  for (i = 0 ; i < properties.Length () ; i++)
  {
    property* p = properties[i];
    if (p->id == csInvalidStringID)
    {
      char* buf = new char[30+strlen (p->propName)];
      if (p->type == CEL_DATA_ACTION)
        strcpy (buf, "cel.action.");
      else
        strcpy (buf, "cel.property.");
      strcat (buf, p->propName);
      p->id = pl->FetchStringID (buf);
      delete[] buf;
    }
    if (p->id == id) return i;
  }
  return csArrayItemNotFound;
}

size_t celPcProperties::FindOrNewProperty (csStringID id)
{
  size_t idx = FindProperty (id);
  if (idx != csArrayItemNotFound) return idx;
  const char* propname = pl->FetchString (id);
  return NewProperty (propname+13);
}

celDataType celPcProperties::GetPropertyOrActionType (csStringID id)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return CEL_DATA_NONE;
  return properties[idx]->type;
}

csStringID celPcProperties::GetPropertyOrActionID (int i)
{
  property* p = properties[i];
  if (p->id == csInvalidStringID)
  {
    char* buf = new char[30+strlen (p->propName)];
    if (p->type == CEL_DATA_ACTION)
      strcpy (buf, "cel.action.");
    else
      strcpy (buf, "cel.property.");
    strcat (buf, p->propName);
    p->id = pl->FetchStringID (buf);
    delete[] buf;
  }
  return p->id;
}

const char* celPcProperties::GetPropertyOrActionDescription (csStringID id)
{
  return "none";
}

bool celPcProperties::SetProperty (csStringID id, const char* v)
{
  SetPropertyIndex (FindOrNewProperty (id), v);
  return true;
}

bool celPcProperties::SetProperty (csStringID id, bool v)
{
  SetPropertyIndex (FindOrNewProperty (id), v);
  return true;
}

bool celPcProperties::SetProperty (csStringID id, float v)
{
  SetPropertyIndex (FindOrNewProperty (id), v);
  return true;
}

bool celPcProperties::SetProperty (csStringID id, long v)
{
  SetPropertyIndex (FindOrNewProperty (id), v);
  return true;
}

bool celPcProperties::SetProperty (csStringID id, const csVector2& v)
{
  SetPropertyIndex (FindOrNewProperty (id), v);
  return true;
}

bool celPcProperties::SetProperty (csStringID id, const csVector3& v)
{
  SetPropertyIndex (FindOrNewProperty (id), v);
  return true;
}

bool celPcProperties::SetProperty (csStringID id, const csColor& v)
{
  SetPropertyIndex (FindOrNewProperty (id), v);
  return true;
}

bool celPcProperties::SetProperty (csStringID id, iCelPropertyClass* v)
{
  SetPropertyIndex (FindOrNewProperty (id), v);
  return true;
}

bool celPcProperties::SetProperty (csStringID id, iCelEntity* v)
{
  SetPropertyIndex (FindOrNewProperty (id), v);
  return true;
}

const char* celPcProperties::GetPropertyString (csStringID id)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyStringIndex (idx);
}

bool celPcProperties::GetPropertyBool (csStringID id)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyBoolIndex (idx);
}

long celPcProperties::GetPropertyLong (csStringID id)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyLongIndex (idx);
}

float celPcProperties::GetPropertyFloat (csStringID id)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyFloatIndex (idx);
}

bool celPcProperties::GetPropertyVector (csStringID id, csVector2& v)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyVectorIndex (idx, v);
}

bool celPcProperties::GetPropertyVector (csStringID id, csVector3& v)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyVectorIndex (idx, v);
}

bool celPcProperties::GetPropertyColor (csStringID id, csColor& v)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyColorIndex (idx, v);
}

iCelPropertyClass* celPcProperties::GetPropertyPClass (csStringID id)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyPClassIndex (idx);
}

iCelEntity* celPcProperties::GetPropertyEntity (csStringID id)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyEntityIndex (idx);
}

#define PROPERTIES_SERIAL 1

csPtr<iCelDataBuffer> celPcProperties::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (PROPERTIES_SERIAL);
  size_t i;
  databuf->Add ((uint32)properties.Length ());
  for (i = 0 ; i < properties.Length () ; i++)
  {
    property* p = properties[i];
    databuf->Add (p->propName);
    //databuf->Add ((uint8)p->type);
    switch (p->type)
    {
      case CEL_DATA_FLOAT:
        databuf->Add (p->v.f);
	break;
      case CEL_DATA_LONG:
        databuf->Add ((int32)p->v.l);
	break;
      case CEL_DATA_BOOL:
        databuf->Add (p->v.b);
	break;
      case CEL_DATA_STRING:
        databuf->Add (p->v.s);
	break;
      case CEL_DATA_VECTOR2:
        databuf->Add (csVector2 (p->v.vec.x, p->v.vec.y));
	break;
      case CEL_DATA_VECTOR3:
        databuf->Add (csVector3 (p->v.vec.x, p->v.vec.y,
		p->v.vec.z));
	break;
      case CEL_DATA_COLOR:
        databuf->Add (csVector3 (p->v.col.red, p->v.col.green,
		p->v.col.blue));
	break;
      case CEL_DATA_PCLASS:
        databuf->Add (p->pclass);
	break;
      case CEL_DATA_ENTITY:
        databuf->Add (p->entity);
	break;
      default:
        // @@@ Impossible!
	break;
    }
  }
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcProperties::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != PROPERTIES_SERIAL) return false;
  properties_hash_dirty = true;
  size_t cnt = databuf->GetUInt32 ();
  Clear ();
  celData* cd;
  size_t i;
  for (i = 0 ; i < cnt ; i++)
  {
    size_t idx = NewProperty (databuf->GetString ()->GetData ());
    cd = databuf->GetData (); if (!cd) return false;
    property* p = properties[idx];
    //p->type = (celDataType)cd->value.ub;
    p->type = cd->type;
    switch (p->type)
    {
      case CEL_DATA_FLOAT:
        p->v.f = cd->value.f;
	break;
      case CEL_DATA_LONG:
        p->v.l = cd->value.l;
	break;
      case CEL_DATA_BOOL:
        p->v.b = cd->value.bo;
	break;
      case CEL_DATA_STRING:
        p->v.s = csStrNew (*cd->value.s);
	break;
      case CEL_DATA_VECTOR2:
        p->v.vec.x = cd->value.v.x;
        p->v.vec.y = cd->value.v.y;
	break;
      case CEL_DATA_VECTOR3:
        p->v.vec.x = cd->value.v.x;
        p->v.vec.y = cd->value.v.y;
        p->v.vec.z = cd->value.v.z;
	break;
      case CEL_DATA_COLOR:
        p->v.col.red = cd->value.col.red;
        p->v.col.green = cd->value.col.green;
        p->v.col.blue = cd->value.col.blue;
	break;
      case CEL_DATA_PCLASS:
        p->pclass = cd->value.pc;
	break;
      case CEL_DATA_ENTITY:
        p->entity = cd->value.ent;
	break;
      default:
        return false;
    }
  }

  return true;
}

size_t celPcProperties::NewProperty (const char* name)
{
  property* p = new property ();
  p->id = csInvalidStringID;	// Fetch later if needed.
  p->propName = csStrNew (name);
  p->type = CEL_DATA_NONE;
  size_t idx = properties.Length ();
  properties.Push (p);
  if (!properties_hash_dirty) properties_hash.Put (name, idx+1);	// +1 so that 0 is an error.
  return idx;
}

size_t celPcProperties::FindOrNewProperty (const char* name)
{
  size_t idx = GetPropertyIndex (name);
  if (idx != csArrayItemNotFound) return idx;
  return NewProperty (name);
}

void celPcProperties::ClearPropertyValue (property* p)
{
  if (p->type == CEL_DATA_STRING) delete[] p->v.s;
  p->pclass = 0;
  p->entity = 0;
  p->type = CEL_DATA_NONE;
}

void celPcProperties::SetProperty (const char* name, float value)
{
  SetPropertyIndex (FindOrNewProperty (name), value);
}

void celPcProperties::SetProperty (const char* name, long value)
{
  SetPropertyIndex (FindOrNewProperty (name), value);
}

void celPcProperties::SetProperty (const char* name, bool value)
{
  SetPropertyIndex (FindOrNewProperty (name), value);
}

void celPcProperties::SetProperty (const char* name, const char* value)
{
  SetPropertyIndex (FindOrNewProperty (name), value);
}

void celPcProperties::SetProperty (const char* name, const csVector2& value)
{
  SetPropertyIndex (FindOrNewProperty (name), value);
}

void celPcProperties::SetProperty (const char* name, const csVector3& value)
{
  SetPropertyIndex (FindOrNewProperty (name), value);
}

void celPcProperties::SetProperty (const char* name, const csColor& value)
{
  SetPropertyIndex (FindOrNewProperty (name), value);
}

void celPcProperties::SetProperty (const char* name, iCelPropertyClass* value)
{
  SetPropertyIndex (FindOrNewProperty (name), value);
}

void celPcProperties::SetProperty (const char* name, iCelEntity* value)
{
  SetPropertyIndex (FindOrNewProperty (name), value);
}

size_t celPcProperties::GetPropertyIndex (const char* name)
{
  if (properties_hash_dirty)
  {
    properties_hash_dirty = false;
    // Create the hash.
    properties_hash.DeleteAll ();
    size_t i;
    for (i = 0 ; i < properties.Length () ; i++)
    {
      property* p = properties[i];
      properties_hash.Put (p->propName, i+1);	// +1 so that 0 is an error
    }
  }

  return properties_hash.Get (name,0) - 1;
}

void celPcProperties::SetPropertyIndex (size_t index, float value)
{
  CS_ASSERT ((index != csArrayItemNotFound) && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_FLOAT;
  p->v.f = value;
  FirePropertyListeners (index);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", this, ret, params, index);
  }
}

void celPcProperties::SetPropertyIndex (size_t index, long value)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_LONG;
  p->v.l = value;
  FirePropertyListeners (index);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", this, ret, params, index);
  }
}

void celPcProperties::SetPropertyIndex (size_t index, bool value)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_BOOL;
  p->v.b = value;
  FirePropertyListeners (index);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", this, ret, params, index);
  }
}

void celPcProperties::SetPropertyIndex (size_t index, const csVector2& value)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_VECTOR2;
  p->v.vec.x = value.x;
  p->v.vec.y = value.y;
  FirePropertyListeners (index);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", this, ret, params, index);
  }
}

void celPcProperties::SetPropertyIndex (size_t index, const csVector3& value)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_VECTOR3;
  p->v.vec.x = value.x;
  p->v.vec.y = value.y;
  p->v.vec.z = value.z;
  FirePropertyListeners (index);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", this, ret, params, index);
  }
}

void celPcProperties::SetPropertyIndex (size_t index, const csColor& value)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_COLOR;
  p->v.col.red = value.red;
  p->v.col.green = value.green;
  p->v.col.blue = value.blue;
  FirePropertyListeners (index);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", this, ret, params, index);
  }
}

void celPcProperties::SetPropertyIndex (size_t index, const char* value)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_STRING;
  p->v.s = csStrNew (value);
  FirePropertyListeners (index);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", this, ret, params, index);
  }
}

void celPcProperties::SetPropertyIndex (size_t index, iCelPropertyClass* value)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_PCLASS;
  p->pclass = value;
  FirePropertyListeners (index);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", this, ret, params, index);
  }
}

void celPcProperties::SetPropertyIndex (size_t index, iCelEntity* value)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_ENTITY;
  p->entity = value;
  FirePropertyListeners (index);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", this, ret, params, index);
  }
}

celDataType celPcProperties::GetPropertyType (size_t index) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  return p->type;
}

float celPcProperties::GetPropertyFloatIndex (size_t index) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  if (p->type == CEL_DATA_FLOAT)
    return p->v.f;
  else
    return 0;
}

long celPcProperties::GetPropertyLongIndex (size_t index) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  if (p->type == CEL_DATA_LONG)
    return p->v.l;
  else
    return 0;
}

bool celPcProperties::GetPropertyBoolIndex (size_t index) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  if (p->type == CEL_DATA_BOOL)
    return p->v.b;
  else
    return false;
}

bool celPcProperties::GetPropertyVectorIndex (size_t index, csVector2& v) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  if (p->type == CEL_DATA_VECTOR2)
  {
    v.x = p->v.vec.x;
    v.y = p->v.vec.y;
    return true;
  }
  else
    return false;
}

bool celPcProperties::GetPropertyVectorIndex (size_t index, csVector3& v) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  if (p->type == CEL_DATA_VECTOR3)
  {
    v.x = p->v.vec.x;
    v.y = p->v.vec.y;
    v.z = p->v.vec.z;
    return true;
  }
  else
    return false;
}

bool celPcProperties::GetPropertyColorIndex (size_t index, csColor& v) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  if (p->type == CEL_DATA_COLOR)
  {
    v.red = p->v.col.red;
    v.green = p->v.col.green;
    v.blue = p->v.col.blue;
    return true;
  }
  else
    return false;
}

const char* celPcProperties::GetPropertyStringIndex (size_t index) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  if (p->type == CEL_DATA_STRING)
    return p->v.s;
  else
    return 0;
}

iCelPropertyClass* celPcProperties::GetPropertyPClassIndex (size_t index) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  if (p->type == CEL_DATA_PCLASS)
    return p->pclass;
  else
    return 0;
}

iCelEntity* celPcProperties::GetPropertyEntityIndex (size_t index) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  if (p->type == CEL_DATA_ENTITY)
    return p->entity;
  else
    return 0;
}

void celPcProperties::ClearProperty (size_t index)
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  FirePropertyListeners (index);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    params->GetParameter (0).Set ((int32)index);
    celData ret;
    bh->SendMessage ("pcproperties_clearproperty", this, ret, params, index);
  }
  property* p = properties[index];
  ClearPropertyValue (p);
  properties.DeleteIndex (index);
  // Properties hash is cleared because it is invalid now.
  properties_hash_dirty = true;
}

void celPcProperties::Clear ()
{
  while (properties.Length () > 0)
  {
    ClearProperty (0);
  }
  properties_hash_dirty = false;
  properties_hash.DeleteAll ();
}

size_t celPcProperties::GetPropertyCount () const
{
  return properties.Length ();
}

const char* celPcProperties::GetPropertyName (size_t index) const
{
  CS_ASSERT (index >= 0 && index < properties.Length ());
  property* p = properties[index];
  return p->propName;
}

void celPcProperties::Dump ()
{
}

void celPcProperties::AddPropertyListener (iPcPropertyListener* listener)
{
  listeners.Push (listener);
}

void celPcProperties::RemovePropertyListener (iPcPropertyListener* listener)
{
  listeners.Delete (listener);
}

void celPcProperties::FirePropertyListeners (size_t idx)
{
  size_t i = listeners.Length ();
  while (i > 0)
  {
    i--;
    listeners[i]->PropertyChanged (&scfiPcProperties, idx);
  }
}

//---------------------------------------------------------------------------

