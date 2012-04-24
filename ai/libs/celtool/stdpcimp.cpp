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
#include "csutil/util.h"
#include "iutil/objreg.h"
#include "ivaria/reporter.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propchg.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"

//---------------------------------------------------------------------------

csStringID celPcCommon::id_tag = csInvalidStringID;
csStringID celPcCommon::id_name = csInvalidStringID;
csStringID celPcCommon::id_value = csInvalidStringID;


celPcCommon::celPcCommon (iObjectRegistry* object_reg) :
	scfImplementationType (this)
{
  celPcCommon::object_reg = object_reg;
  entity = 0;
  propdata = 0;
  propholder = 0;
  propclasses_dirty = true;

  pl = csQueryRegistry<iCelPlLayer> (object_reg);

  if (id_tag == csInvalidStringID)
  {
    id_tag = pl->FetchStringID ("tag");
    id_name = pl->FetchStringID ("name");
    id_value = pl->FetchStringID ("value");
  }
}

celPcCommon::~celPcCommon ()
{
}

bool celPcCommon::Error (char const *desc, ...) const
{
  va_list args;
  va_start(args, desc);
  csReportV (object_reg,
	CS_REPORTER_SEVERITY_ERROR, GetName (), desc, args);
  va_end(args);
  return false;
}

bool celPcCommon::Warning (char const *desc, ...) const
{
  va_list args;
  va_start(args, desc);
  csReportV (object_reg,
	CS_REPORTER_SEVERITY_WARNING, GetName (), desc, args);
  va_end(args);
  return false;
}

bool celPcCommon::Bug (char const *desc, ...) const
{
  va_list args;
  va_start (args, desc);
  csReportV(object_reg,
	CS_REPORTER_SEVERITY_BUG, GetName (), desc, args);
  va_end(args);
  return false;
}

bool celPcCommon::Notify (char const *desc, ...) const
{
  va_list args;
  va_start (args, desc);
  csReportV(object_reg,
	CS_REPORTER_SEVERITY_NOTIFY, GetName (), desc, args);
  va_end(args);
  return false;
}

void celPcCommon::SetTag (const char* tagname)
{
  tag = tagname;
}

const char* celPcCommon::GetName () const
{
  return csobj.GetName ();
}
// @@@ (ge) Please note that after the alternate names are deprecated
//  that this can turn into a simple assignment and the macro factory
//  definition should pass in a hard reference for the name.
void celPcCommon::SetName (const char* pcname)
{
  csobj.SetName (pcname);
}
iObject *celPcCommon::QueryObject ()
{
  return &csobj;
}

void celPcCommon::SetEntity (iCelEntity* entity)
{
  if (celPcCommon::entity)
    celPcCommon::entity->QueryMessageChannel ()->Unsubscribe (this);
  celPcCommon::entity = entity;
  if (entity && propholder && propholder->mask.Length () > 0)
    entity->QueryMessageChannel ()->Subscribe (this, propholder->mask);
}

void celPcCommon::FirePropertyChangeCallback (int propertyId)
{
  size_t i;
  for (i = 0 ; i < callbacks.GetSize () ; i++)
  {
    iCelPropertyChangeCallback* cb = callbacks[i];
    cb->PropertyChanged (propertyId, this);
  }
}

bool celPcCommon::AddPropertyChangeCallback (iCelPropertyChangeCallback* cb)
{
  size_t idx = callbacks.Find (cb);
  if (idx != csArrayItemNotFound) return false;
  callbacks.Push (cb);
  return true;
}

bool celPcCommon::RemovePropertyChangeCallback (
  	iCelPropertyChangeCallback* cb)
{
  size_t idx = callbacks.Find (cb);
  if (idx == csArrayItemNotFound) return false;
  callbacks.DeleteIndex (idx);
  return true;
}


template <class T>
bool celPcCommon::SetPropertyTemplated (csStringID propertyId, T l,
    celDataType type)
{
  if (!propdata) return false;
  int i = propholder->constants.Get (propertyId, -1);
  if (i == -1)
  {
    // @@@ Warning?
    return false;
  }

  if (SetPropertyIndexed (i, l))
    return true;

  Property* props = propholder->properties;
  CS_ASSERT (props[i].id == propertyId);
  if (props[i].datatype == type) 
  {
    if (!propdata[i])
    {
      return Warning (
	    "Property %s from %s is not correctly set up!",
	    pl->FetchString (propertyId), GetName ());
    }
    ((T*)(propdata[i]))[0] = l;
    return true;
  }
  // @@@ Warning?
  return false; 
}

bool celPcCommon::SetProperty (csStringID propertyId, long l)
{
  return SetPropertyTemplated<long> (propertyId, l, CEL_DATA_LONG);
}

bool celPcCommon::SetProperty (csStringID propertyId, float f)
{
  return SetPropertyTemplated<float> (propertyId, f, CEL_DATA_FLOAT);
}

bool celPcCommon::SetProperty (csStringID propertyId, bool b)
{
  return SetPropertyTemplated<bool> (propertyId, b, CEL_DATA_BOOL);
}

bool celPcCommon::SetProperty (csStringID propertyId, const csVector2& v)
{
  return SetPropertyTemplated<csVector2> (propertyId, v, CEL_DATA_VECTOR2);
}

bool celPcCommon::SetProperty (csStringID propertyId, const csVector3& v)
{
  return SetPropertyTemplated<csVector3> (propertyId, v, CEL_DATA_VECTOR3);
}

bool celPcCommon::SetProperty (csStringID propertyId, const csColor& v)
{
  return SetPropertyTemplated<csColor> (propertyId, v, CEL_DATA_COLOR);
}

bool celPcCommon::SetProperty (csStringID propertyId, iCelPropertyClass* v)
{
  return SetPropertyTemplated<iCelPropertyClass*> (propertyId, v,
      CEL_DATA_PCLASS);
}

bool celPcCommon::SetProperty (csStringID propertyId, iCelEntity* v)
{
  return SetPropertyTemplated<iCelEntity*> (propertyId, v, CEL_DATA_ENTITY);
}

bool celPcCommon::SetProperty (csStringID propertyId, iBase* v)
{
  return SetPropertyTemplated<iBase*> (propertyId, v, CEL_DATA_IBASE);
}

bool celPcCommon::SetProperty (csStringID propertyId, const char* s)
{
  if (!propdata) return false;
  int i = propholder->constants.Get (propertyId, -1);
  if (i == -1)
  {
    // @@@ Warning?
    return false;
  }

  if (SetPropertyIndexed (i, s))
    return true;

  Property* props = propholder->properties;
  CS_ASSERT (props[i].id == propertyId);
  if (props[i].datatype == CEL_DATA_STRING)
  {
    if (!propdata[i])
    {
      return Warning ("Property %s from %s is not correctly set up!",
	    pl->FetchString (propertyId), GetName ());
      return false;
    }
    char** ptr = (char**) propdata[i];
    if  (*ptr != s)
    {
      delete[] (*ptr);
      *ptr = csStrNew (s);
    }
    return true;
  }
  // @@@ Warning?
  return false; 
}

celDataType celPcCommon::GetPropertyOrActionType (csStringID propertyId)
{
  if (!propdata) return CEL_DATA_NONE;
  int i = propholder->constants.Get (propertyId, -1);
  if (i == -1)
  {
    // @@@ Warning?
    return CEL_DATA_NONE;
  }

  Property* props = propholder->properties;
  CS_ASSERT (props[i].id == propertyId);
  return props[i].datatype;
}

bool celPcCommon::IsPropertyReadOnly (csStringID propertyId)
{
  if (!propdata) return 0;
  int i = propholder->constants.Get (propertyId, -1);
  if (i == -1)
  {
    // @@@ Warning?
    return true;
  }

  Property* props = propholder->properties;
  CS_ASSERT (props[i].id == propertyId);
  return props[i].readonly;
}

template <class T>
T celPcCommon::GetPropertyTemplated (csStringID propertyId,
    celDataType type)
{
  if (!propdata) return 0;
  int i = propholder->constants.Get (propertyId, -1);
  if (i == -1)
  {
    // @@@ Warning?
    return 0;
  }

  T v;
  if (GetPropertyIndexed (i, v))
    return v;

  Property* props = propholder->properties;
  CS_ASSERT (props[i].id == propertyId);

  if (props[i].datatype == type)
  {
    if (!propdata[i])
    {
      Warning ("Property %s from %s is not correctly set up!",
	    pl->FetchString (propertyId), GetName ());
      return 0;
    }
    return ((T*)(propdata[i]))[0];
  }
  // @@@ Warning?
  return 0;
}

template <class T>
bool celPcCommon::GetPropertyTemplated (csStringID propertyId, celDataType type,
    T& v)
{
  if (!propdata) return false;
  int i = propholder->constants.Get (propertyId, -1);
  if (i == -1)
  {
    // @@@ Warning?
    return false;
  }

  if (GetPropertyIndexed (i, v))
    return true;

  Property* props = propholder->properties;
  CS_ASSERT (props[i].id == propertyId);

  if (props[i].datatype == type)
  {
    if (!propdata[i])
    {
      return Warning ("Property %s from %s is not correctly set up!",
	    pl->FetchString (propertyId), GetName ());
      return false;
    }
    v = ((T*)(propdata[i]))[0];
    return true;
  }
  // @@@ Warning?
  return false;
}


long celPcCommon::GetPropertyLongByID (csStringID propertyId)
{
  return GetPropertyTemplated<long> (propertyId, CEL_DATA_LONG);
}

float celPcCommon::GetPropertyFloatByID (csStringID propertyId)
{
  return GetPropertyTemplated<float> (propertyId, CEL_DATA_FLOAT);
}

bool celPcCommon::GetPropertyBoolByID (csStringID propertyId)
{
  return GetPropertyTemplated<bool> (propertyId, CEL_DATA_BOOL);
}

const char* celPcCommon::GetPropertyStringByID (csStringID propertyId)
{
  return GetPropertyTemplated<const char*> (propertyId, CEL_DATA_STRING);
}

bool celPcCommon::GetPropertyVectorByID (csStringID propertyId, csVector2& v)
{
  return GetPropertyTemplated<csVector2> (propertyId, CEL_DATA_VECTOR2, v);
}

bool celPcCommon::GetPropertyVectorByID (csStringID propertyId, csVector3& v)
{
  return GetPropertyTemplated<csVector3> (propertyId, CEL_DATA_VECTOR3, v);
}

bool celPcCommon::GetPropertyColorByID (csStringID propertyId, csColor& v )
{
  return GetPropertyTemplated<csColor> (propertyId, CEL_DATA_COLOR, v);
}

iCelPropertyClass* celPcCommon::GetPropertyPClassByID (csStringID propertyId)
{
  return GetPropertyTemplated<iCelPropertyClass*> (propertyId,
      CEL_DATA_PCLASS);
}

iCelEntity* celPcCommon::GetPropertyEntityByID (csStringID propertyId)
{
  return GetPropertyTemplated<iCelEntity*> (propertyId, CEL_DATA_ENTITY);
}

iBase* celPcCommon::GetPropertyIBaseByID (csStringID propertyId)
{
  return GetPropertyTemplated<iBase*> (propertyId, CEL_DATA_IBASE);
}

const char* celPcCommon::GetPropertyOrActionDescription (csStringID propertyId)
{
  if (!propdata) return 0;
  int i = propholder->constants.Get (propertyId, -1);
  if (i == -1)
  {
    // @@@ Warning?
    return 0;
  }

  Property* props = propholder->properties;
  CS_ASSERT (props[i].id == propertyId);
  return props[i].desc;
}

size_t celPcCommon::GetPropertyAndActionCount()
{
  if (!propholder) return 0;

  return propholder->propertycount;
}

csStringID celPcCommon::GetPropertyOrActionID (size_t i)
{ 
  if (!propholder) return csInvalidStringID;
  Property* props = propholder->properties;
  if (!props) return csInvalidStringID;
  
  return props[i].id; 
}

bool celPcCommon::PerformAction (csStringID actionId,
    iCelParameterBlock* params, celData& ret)
{
  if (!propholder) return false;
  int i = propholder->constants.Get (actionId, -1);
  if (i == -1)
  {
    // @@@ Warning?
    return false;
  }
  return PerformActionIndexed (i, params, ret);
}

#define IDX_SETPROPERTY 1000000000

void celPcCommon::SetActionMask (const char* mask)
{
  propholder->mask = mask;
  csString newid = mask;
  newid += "SetProperty";
  propholder->new_constants.Put (pl->FetchStringID (newid), IDX_SETPROPERTY);
}

bool celPcCommon::ReceiveMessage (csStringID msg_id, iMessageSender* sender,
      celData& ret, iCelParameterBlock* params)
{
  if (!propholder) return false;
  int i = propholder->new_constants.Get (msg_id, -1);
  if (i == -1)
    return false;

  // Check if this message is for this tag first.
  csString msgtag;
  if (!Fetch (msgtag, params, id_tag, true, "")) return false;
  if (!msgtag.IsEmpty ())
  {
    if (tag != msgtag) return false;
  }

  if (i == IDX_SETPROPERTY)
  {
    csString property;
    if (!Fetch (property, params, id_name)) return false;
    csStringID propertyID = pl->FetchStringID (property);
    const celData* p_value = params->GetParameter (id_value);
    if (!p_value) return false;
    switch (p_value->type)
    {
      case CEL_DATA_STRING: SetProperty (propertyID, p_value->value.s->GetData ()); break;
      case CEL_DATA_BOOL: SetProperty (propertyID, p_value->value.bo); break;
      case CEL_DATA_BYTE: SetProperty (propertyID, long (p_value->value.b)); break;
      case CEL_DATA_UBYTE: SetProperty (propertyID, long (p_value->value.ub)); break;
      case CEL_DATA_WORD: SetProperty (propertyID, long (p_value->value.w)); break;
      case CEL_DATA_UWORD: SetProperty (propertyID, long (p_value->value.uw)); break;
      case CEL_DATA_LONG: SetProperty (propertyID, long (p_value->value.l)); break;
      case CEL_DATA_ULONG: SetProperty (propertyID, long (p_value->value.ul)); break;
      case CEL_DATA_FLOAT: SetProperty (propertyID, p_value->value.f); break;
      case CEL_DATA_VECTOR2: SetProperty (propertyID,
		csVector2 (p_value->value.v.x, p_value->value.v.y));
		break;
      case CEL_DATA_VECTOR3: SetProperty (propertyID,
		csVector3 (p_value->value.v.x, p_value->value.v.y, p_value->value.v.z));
		break;
      case CEL_DATA_COLOR: SetProperty (propertyID,
		csColor (p_value->value.col.red, p_value->value.col.green, p_value->value.col.blue));
		break;
      case CEL_DATA_PCLASS: SetProperty (propertyID, p_value->value.pc); break;
      case CEL_DATA_ENTITY: SetProperty (propertyID, p_value->value.ent); break;
      case CEL_DATA_IBASE: SetProperty (propertyID, p_value->value.ibase); break;
      default: break;	// @@@ Error?
    }
    return true;
  }
  else return PerformActionIndexed (i, params, ret);
}

//---------------------------------------------------------------------------

