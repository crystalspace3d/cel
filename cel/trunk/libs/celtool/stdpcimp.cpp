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
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propchg.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"

//---------------------------------------------------------------------------

celPcCommon::celPcCommon (iObjectRegistry* object_reg) :
	scfImplementationType (this)
{
  celPcCommon::object_reg = object_reg;
  entity = 0;
  propdata = 0;
  propholder = 0;
  propclasses_dirty = true;
  tag = 0;

  pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
}

celPcCommon::~celPcCommon ()
{
  delete[] tag;
}

void celPcCommon::SetTag (const char* tagname)
{
  delete[] tag;
  tag = csStrNew (tagname);
}

void celPcCommon::SetEntity (iCelEntity* entity)
{
  celPcCommon::entity = entity;
}

void celPcCommon::FirePropertyChangeCallback (int propertyId)
{
  size_t i;
  for (i = 0 ; i < callbacks.Length () ; i++)
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
      csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	    "cel.celpccommon.setproperty",
	    "Property %s from %s is not correctly set up!",
	    pl->FetchString (propertyId), GetName ());
      return false;
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
      csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	    "cel.celpccommon.setproperty",
	    "Property %s from %s is not correctly set up!",
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
      csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	    "cel.celpccommon.getproperty",
	    "Property %s from %s is not correctly set up!",
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
      csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	    "cel.celpccommon.getproperty",
	    "Property %s from %s is not correctly set up!",
	    pl->FetchString (propertyId), GetName ());
      return false;
    }
    v = ((T*)(propdata[i]))[0];
    return true;
  }
  // @@@ Warning?
  return false;
}


long celPcCommon::GetPropertyLong (csStringID propertyId)
{
  return GetPropertyTemplated<long> (propertyId, CEL_DATA_LONG);
}

float celPcCommon::GetPropertyFloat (csStringID propertyId)
{
  return GetPropertyTemplated<float> (propertyId, CEL_DATA_FLOAT);
}

bool celPcCommon::GetPropertyBool (csStringID propertyId)
{
  return GetPropertyTemplated<bool> (propertyId, CEL_DATA_BOOL);
}

const char* celPcCommon::GetPropertyString (csStringID propertyId)
{
  return GetPropertyTemplated<const char*> (propertyId, CEL_DATA_STRING);
}

bool celPcCommon::GetPropertyVector (csStringID propertyId, csVector2& v)
{
  return GetPropertyTemplated<csVector2> (propertyId, CEL_DATA_VECTOR2, v);
}

bool celPcCommon::GetPropertyVector (csStringID propertyId, csVector3& v)
{
  return GetPropertyTemplated<csVector3> (propertyId, CEL_DATA_VECTOR3, v);
}

bool celPcCommon::GetPropertyColor (csStringID propertyId, csColor& v )
{
  return GetPropertyTemplated<csColor> (propertyId, CEL_DATA_COLOR, v);
}

iCelPropertyClass* celPcCommon::GetPropertyPClass (csStringID propertyId)
{
  return GetPropertyTemplated<iCelPropertyClass*> (propertyId,
      CEL_DATA_PCLASS);
}

iCelEntity* celPcCommon::GetPropertyEntity (csStringID propertyId)
{
  return GetPropertyTemplated<iCelEntity*> (propertyId, CEL_DATA_ENTITY);
}

iBase* celPcCommon::GetPropertyIBase (csStringID propertyId)
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


//---------------------------------------------------------------------------

