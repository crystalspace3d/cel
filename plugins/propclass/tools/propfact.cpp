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

CEL_IMPLEMENT_FACTORY_ALT (Properties, "pctools.properties", "pcproperties")

//---------------------------------------------------------------------------

csStringID celPcProperties::id_index = csInvalidStringID;
csStringID celPcProperties::id_name = csInvalidStringID;
csStringID celPcProperties::id_value = csInvalidStringID;

PropertyHolder celPcProperties::propinfo;

celPcProperties::celPcProperties (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  if (id_index == csInvalidStringID)
  {
    id_index = pl->FetchStringID ("index");
    id_name = pl->FetchStringID ("name");
    id_value = pl->FetchStringID ("value");
  }

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.properties.action.");
    AddAction (action_setproperty, "SetProperty");
  }

  params.AttachNew (new celOneParameterBlock ());
  params->SetParameterDef (id_index);
  properties_hash_dirty = false;
  atBaseline = false;
}

celPcProperties::~celPcProperties ()
{
  // First we clear the listeners so that we will not fire any
  // further events.
  listeners.DeleteAll ();
  Clear ();
}

size_t celPcProperties::FindProperty (csStringID id)
{
  size_t i;
  for (i = 0 ; i < properties.GetSize () ; i++)
  {
    property* p = properties[i];
    if (p->id == csInvalidStringID)
      p->id = pl->FetchStringID (p->propName);
    if (p->id == id) return i;
  }
  return csArrayItemNotFound;
}

size_t celPcProperties::FindOrNewProperty (csStringID id)
{
  size_t idx = FindProperty (id);
  if (idx != csArrayItemNotFound) return idx;
  const char* propname = pl->FetchString (id);
  return NewProperty (propname);
}

celDataType celPcProperties::GetPropertyOrActionType (csStringID id)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return CEL_DATA_NONE;
  return properties[idx]->type;
}

csStringID celPcProperties::GetPropertyOrActionID (size_t i)
{
  property* p = properties[i];
  if (p->id == csInvalidStringID)
    p->id = pl->FetchStringID (p->propName);
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

bool celPcProperties::SetProperty (csStringID id, iBase* v)
{
  SetPropertyIndex (FindOrNewProperty (id), v);
  return true;
}

const char* celPcProperties::GetPropertyStringByID (csStringID id)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyStringIndex (idx);
}

bool celPcProperties::GetPropertyBoolByID (csStringID id)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyBoolIndex (idx);
}

long celPcProperties::GetPropertyLongByID (csStringID id)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyLongIndex (idx);
}

float celPcProperties::GetPropertyFloatByID (csStringID id)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyFloatIndex (idx);
}

bool celPcProperties::GetPropertyVectorByID (csStringID id, csVector2& v)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyVectorIndex (idx, v);
}

bool celPcProperties::GetPropertyVectorByID (csStringID id, csVector3& v)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyVectorIndex (idx, v);
}

bool celPcProperties::GetPropertyColorByID (csStringID id, csColor& v)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyColorIndex (idx, v);
}

iCelPropertyClass* celPcProperties::GetPropertyPClassByID (csStringID id)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyPClassIndex (idx);
}

iCelEntity* celPcProperties::GetPropertyEntityByID (csStringID id)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyEntityIndex (idx);
}

iBase* celPcProperties::GetPropertyIBaseByID (csStringID id)
{
  size_t idx = FindProperty (id);
  if (idx == csArrayItemNotFound) return 0;
  return GetPropertyIBaseIndex (idx);
}

void celPcProperties::SaveModifications (iCelCompactDataBufferWriter* buf,
    iStringSet* strings)
{
  buf->AddUInt32 (properties.GetSize ());
  for (size_t i = 0 ; i < properties.GetSize () ; i++)
  {
    property* prop = properties[i];
    buf->AddID (prop->id);
    buf->AddUInt8 (prop->type);
    switch (prop->type)
    {
      case CEL_DATA_FLOAT: buf->AddFloat (prop->v.f); break;
      case CEL_DATA_LONG: buf->AddInt32 (prop->v.l); break;
      case CEL_DATA_BOOL: buf->AddBool (prop->v.b); break;
      case CEL_DATA_STRING: buf->AddID (strings->Request (prop->v.s)); break;
      case CEL_DATA_VECTOR2:
        buf->AddFloat (prop->v.vec.x);
        buf->AddFloat (prop->v.vec.y);
        break;
      case CEL_DATA_VECTOR3:
        buf->AddFloat (prop->v.vec.x);
        buf->AddFloat (prop->v.vec.y);
        buf->AddFloat (prop->v.vec.z);
        break;
      case CEL_DATA_COLOR:
        buf->AddFloat (prop->v.col.red);
        buf->AddFloat (prop->v.col.green);
        buf->AddFloat (prop->v.col.blue);
        break;
      case CEL_DATA_PCLASS:
        {
          if (prop->pclass)
          {
            buf->AddUInt32 (prop->pclass->GetEntity ()->GetID ());
            buf->AddID (strings->Request (prop->pclass->GetName ()));
            if (prop->pclass->GetTag ())
              buf->AddID (strings->Request (prop->pclass->GetTag ()));
            else
              buf->AddID (csInvalidStringID);
          }
          else
          {
            buf->AddUInt32 (csArrayItemNotFound);
          }
        }
        break;
      case CEL_DATA_ENTITY:
        {
          if (prop->entity)
            buf->AddUInt32 (prop->entity->GetID ());
          else
            buf->AddUInt32 (csArrayItemNotFound);
        }
        break;
      case CEL_DATA_IBASE:
        printf ("Saving of general iBase can't be done for property %s in entity %s!\n",
            prop->propName, entity->GetName ());
        break;
      default:
        printf ("Unknown datatype %d for property %s in entity %s!\n",
            prop->type, prop->propName, entity->GetName ());
        break;

    }
  }
}

void celPcProperties::RestoreModifications (iCelCompactDataBufferReader* buf,
    const csHash<csString,csStringID>& strings)
{
  Clear ();
  size_t size = buf->GetUInt32 ();
  for (size_t i = 0 ; i < size ; i++)
  {
    csStringID id = buf->GetID ();
    celDataType type = (celDataType) buf->GetUInt8 ();
    switch (type)
    {
      case CEL_DATA_FLOAT: SetProperty (id, buf->GetFloat ()); break;
      case CEL_DATA_LONG: SetProperty (id, (long)buf->GetInt32 ()); break;
      case CEL_DATA_BOOL: SetProperty (id, buf->GetBool ()); break;
      case CEL_DATA_STRING:
        SetProperty (id, strings.Get (buf->GetID (), (const char*)0));
        break;
      case CEL_DATA_VECTOR2:
        SetProperty (id, csVector2 (buf->GetFloat (), buf->GetFloat ()));
        break;
      case CEL_DATA_VECTOR3:
        SetProperty (id, csVector3 (buf->GetFloat (), buf->GetFloat (),
              buf->GetFloat ()));
        break;
      case CEL_DATA_COLOR:
        SetProperty (id, csColor (buf->GetFloat (), buf->GetFloat (),
              buf->GetFloat ()));
        break;
      case CEL_DATA_PCLASS:
        {
          iCelPropertyClass* pc = 0;
          uint entid = (uint)buf->GetUInt32 ();
          if (entid != (uint)csArrayItemNotFound)
          {
            iCelEntity* ent = pl->GetEntity (entid);
            if (!ent)
            {
              //@@@ Error reporting
              printf ("Can't find entity '%d'!\n", entid);
              return;
            }
            const char* pcname = strings.Get (buf->GetID (), (const char*)0);
            csStringID tagId = buf->GetID ();
            if (tagId == csInvalidStringID)
              pc = ent->GetPropertyClassList ()->FindByName (pcname);
            else
              pc = ent->GetPropertyClassList ()->FindByNameAndTag (pcname,
                    strings.Get (tagId, (const char*)0));
            if (!pc)
            {
              // @@@ Error reporting
              printf ("Can't find property class '%s' for entity '%s'!\n",
                    pcname, ent->GetName ());
              return;
            }
          }
          SetProperty (id, pc);
        }
        break;
      case CEL_DATA_ENTITY:
        {
          iCelEntity* ent = 0;
          uint entid = (uint)buf->GetUInt32 ();
          if (entid != (uint)csArrayItemNotFound)
          {
            ent = pl->GetEntity (entid);
            if (!ent)
            {
              //@@@ Error reporting
              printf ("Can't find entity '%d'!\n", entid);
              return;
            }
          }
          SetProperty (id, ent);
        }
        break;
      case CEL_DATA_IBASE:
        break;
      default:
        break;
    }
  }

  atBaseline = false;
}

bool celPcProperties::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_setproperty:
      {
	csString name, value_s;
	if (!Fetch (name, params, id_name)) return false;
	if (!Fetch (value_s, params, id_value, true, "")) return false;
	if (!value_s.IsEmpty ())
        {
          SetProperty (name, (const char*)value_s);
          return true;
        }
        CEL_FETCH_BOOL_PAR (value_b,params,id_value);
        if (p_value_b)
        {
          SetProperty (name, (bool)value_b);
          return true;
        }
	if (ParExists (CEL_DATA_FLOAT, params, id_value))
        {
	  float value_f;
	  if (!Fetch (value_f, params, id_value)) return false;
          SetProperty (name, value_f);
          return true;
        }
	if (ParExists (CEL_DATA_LONG, params, id_value))
	{
	  long value_l;
	  if (!Fetch (value_l, params, id_value)) return false;
          SetProperty (name, value_l);
          return true;
        }
        CEL_FETCH_VECTOR3_PAR (value_v3,params,id_value);
        if (p_value_v3)
        {
          SetProperty (name, (csVector3&)value_v3);
          return true;
        }
        return false;
      }
    default:
      return false;
  }
}

size_t celPcProperties::NewProperty (const char* name)
{
  property* p = new property ();
  p->id = pl->FetchStringID (name);
  p->propName = csStrNew (name);
  p->type = CEL_DATA_NONE;
  size_t idx = properties.GetSize ();
  properties.Push (p);
  if (!properties_hash_dirty) properties_hash.Put (name, idx+1);	// +1 so that 0 is an error.
  atBaseline = false;
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
  atBaseline = false;
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

void celPcProperties::SetProperty (const char* name, iBase* value)
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
    for (i = 0 ; i < properties.GetSize () ; i++)
    {
      property* p = properties[i];
      properties_hash.Put (p->propName, i+1);	// +1 so that 0 is an error
    }
  }

  return properties_hash.Get (name,0) - 1;
}

void celPcProperties::HandlePropertyChange (size_t index)
{
  atBaseline = false;
  FirePropertyListeners (index);
  params->GetParameter (0).Set ((int32)index);
  iCelBehaviour* bh = entity->GetBehaviour ();
  if (bh)
  {
    celData ret;
    bh->SendMessage ("pcproperties_setproperty", this, ret, params, index);
  }
  if (!dispatcher_set)
  {
    dispatcher_set = entity->QueryMessageChannel ()->
      CreateMessageDispatcher (this, pl->FetchStringID ("cel.properties.set"));
    if (!dispatcher_set) return;
  }
  dispatcher_set->SendMessage (params);
}

void celPcProperties::SetPropertyIndex (size_t index, float value)
{
  CS_ASSERT ((index != csArrayItemNotFound) && index < properties.GetSize ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_FLOAT;
  p->v.f = value;
  HandlePropertyChange (index);
}

void celPcProperties::SetPropertyIndex (size_t index, long value)
{
  CS_ASSERT (index >= 0 && index < properties.GetSize ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_LONG;
  p->v.l = value;
  HandlePropertyChange (index);
}

void celPcProperties::SetPropertyIndex (size_t index, bool value)
{
  CS_ASSERT (index >= 0 && index < properties.GetSize ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_BOOL;
  p->v.b = value;
  HandlePropertyChange (index);
}

void celPcProperties::SetPropertyIndex (size_t index, const csVector2& value)
{
  CS_ASSERT (index >= 0 && index < properties.GetSize ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_VECTOR2;
  p->v.vec.x = value.x;
  p->v.vec.y = value.y;
  HandlePropertyChange (index);
}

void celPcProperties::SetPropertyIndex (size_t index, const csVector3& value)
{
  CS_ASSERT (index >= 0 && index < properties.GetSize ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_VECTOR3;
  p->v.vec.x = value.x;
  p->v.vec.y = value.y;
  p->v.vec.z = value.z;
  HandlePropertyChange (index);
}

void celPcProperties::SetPropertyIndex (size_t index, const csColor& value)
{
  CS_ASSERT (index >= 0 && index < properties.GetSize ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_COLOR;
  p->v.col.red = value.red;
  p->v.col.green = value.green;
  p->v.col.blue = value.blue;
  HandlePropertyChange (index);
}

void celPcProperties::SetPropertyIndex (size_t index, const char* value)
{
  CS_ASSERT (index >= 0 && index < properties.GetSize ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_STRING;
  p->v.s = csStrNew (value);
  HandlePropertyChange (index);
}

void celPcProperties::SetPropertyIndex (size_t index, iCelPropertyClass* value)
{
  CS_ASSERT (index >= 0 && index < properties.GetSize ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_PCLASS;
  p->pclass = value;
  HandlePropertyChange (index);
}

void celPcProperties::SetPropertyIndex (size_t index, iCelEntity* value)
{
  CS_ASSERT (index >= 0 && index < properties.GetSize ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_ENTITY;
  p->entity = value;
  HandlePropertyChange (index);
}

void celPcProperties::SetPropertyIndex (size_t index, iBase* value)
{
  CS_ASSERT (index >= 0 && index < properties.GetSize ());
  property* p = properties[index];
  ClearPropertyValue (p);
  p->type = CEL_DATA_IBASE;
  p->ref = value;
  HandlePropertyChange (index);
}

celDataType celPcProperties::GetPropertyType (size_t index) const
{
  CS_ASSERT (index >= 0 && index < properties.GetSize ());
  property* p = properties[index];
  return p->type;
}

float celPcProperties::GetPropertyFloatIndex (size_t index) const
{
  CS_ASSERT (index >= 0 && index < properties.GetSize ());
  property* p = properties[index];
  if (p->type == CEL_DATA_FLOAT)
    return p->v.f;
  else
    return 0;
}

long celPcProperties::GetPropertyLongIndex (size_t index) const
{
  CS_ASSERT (index >= 0 && index < properties.GetSize ());
  property* p = properties[index];
  if (p->type == CEL_DATA_LONG)
    return p->v.l;
  else
    return 0;
}

bool celPcProperties::GetPropertyBoolIndex (size_t index) const
{
  CS_ASSERT (index >= 0 && index < properties.GetSize ());
  property* p = properties[index];
  if (p->type == CEL_DATA_BOOL)
    return p->v.b;
  else
    return false;
}

bool celPcProperties::GetPropertyVectorIndex (size_t index, csVector2& v) const
{
  CS_ASSERT (index >= 0 && index < properties.GetSize ());
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
  CS_ASSERT (index >= 0 && index < properties.GetSize ());
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
  CS_ASSERT (index >= 0 && index < properties.GetSize ());
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
  CS_ASSERT (index >= 0 && index < properties.GetSize ());
  property* p = properties[index];
  if (p->type == CEL_DATA_STRING)
    return p->v.s;
  else
    return 0;
}

iCelPropertyClass* celPcProperties::GetPropertyPClassIndex (size_t index) const
{
  CS_ASSERT (index >= 0 && index < properties.GetSize ());
  property* p = properties[index];
  if (p->type == CEL_DATA_PCLASS)
    return p->pclass;
  else
    return 0;
}

iCelEntity* celPcProperties::GetPropertyEntityIndex (size_t index) const
{
  CS_ASSERT (index >= 0 && index < properties.GetSize ());
  property* p = properties[index];
  if (p->type == CEL_DATA_ENTITY)
    return p->entity;
  else
    return 0;
}

iBase* celPcProperties::GetPropertyIBaseIndex (size_t index) const
{
  CS_ASSERT (index >= 0 && index < properties.GetSize ());
  property* p = properties[index];
  if (p->type == CEL_DATA_IBASE)
    return p->ref;
  else
    return 0;
}

void celPcProperties::ClearProperty (size_t index)
{
  CS_ASSERT (index >= 0 && index < properties.GetSize ());
  FirePropertyListeners (index);
  params->GetParameter (0).Set ((int32)index);
  if (entity) 
  {
    iCelBehaviour* bh = entity->GetBehaviour ();
    if (bh)
    {
      celData ret;
      bh->SendMessage ("pcproperties_clearproperty", this, ret, params, index);
    }
    if (!dispatcher_clear)
    {
      dispatcher_clear = entity->QueryMessageChannel ()->
        CreateMessageDispatcher (this, pl->FetchStringID ("cel.properties.clear"));
      if (!dispatcher_clear) return;
    }
    dispatcher_clear->SendMessage (params);
  }
  property* p = properties[index];
  ClearPropertyValue (p);
  properties.DeleteIndex (index);
  // Properties hash is cleared because it is invalid now.
  properties_hash_dirty = true;
  atBaseline = false;
}

void celPcProperties::Clear ()
{
  while (properties.GetSize () > 0)
  {
    ClearProperty (0);
  }
  properties_hash_dirty = false;
  properties_hash.DeleteAll ();
}

size_t celPcProperties::GetPropertyCount () const
{
  return properties.GetSize ();
}

const char* celPcProperties::GetPropertyName (size_t index) const
{
  CS_ASSERT (index >= 0 && index < properties.GetSize ());
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
  size_t i = listeners.GetSize ();
  while (i > 0)
  {
    i--;
    listeners[i]->PropertyChanged (this, idx);
  }
}

//---------------------------------------------------------------------------

