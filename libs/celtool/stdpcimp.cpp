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
  propcount = 0;
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


bool celPcCommon::SetProperty (csStringID propertyId, long l)
{
  UpdateProperties ();

  if (!propcount) return false;

  for (size_t i=0; i<(*propcount); i++)
  {
    if (props[i].id == propertyId)
      if (props[i].datatype == CEL_DATA_LONG) 
      {
        if (!propdata[i])
	{
	  csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	    "cel.celpccommon.setproperty",
	    "Property %s from %s is not correctly set up!",
	    pl->FetchString (propertyId), GetName ());
	  return false;
	}
	((long*)(propdata[i]))[0] = l;
	return true;
      }
      else return false;
  }
  return false; 
}

bool celPcCommon::SetProperty (csStringID propertyId, float f)
{
  UpdateProperties ();

  if (!propcount) return false;

  for (size_t i=0; i<(*propcount); i++)
  {
    if (props[i].id == propertyId)
      if (props[i].datatype == CEL_DATA_FLOAT)
      {
        if (!propdata[i])
	{
	  csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	    "cel.celpccommon.setproperty",
	    "Property %s from %s is not correctly set up!",
	    pl->FetchString (propertyId), GetName ());
	  return false;
	}
	((float*)(propdata[i]))[0] = f;
	return true;
      }
      else return false;
  }
  return false; 
}

bool celPcCommon::SetProperty (csStringID propertyId, bool b)
{
  UpdateProperties ();

  if (!propcount) return false;

  for (size_t i=0; i<(*propcount); i++)
  {
    if (props[i].id == propertyId)
      if (props[i].datatype == CEL_DATA_BOOL)
      {
        if (!propdata[i])
	{
	  csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	    "cel.celpccommon.setproperty",
	    "Property %s from %s is not correctly set up!",
	    pl->FetchString (propertyId), GetName ());
	  return false;
	}
	((bool*)(propdata[i]))[0] = b;
	return true;
      }
      else return false;
  }
  return false; 
}

bool celPcCommon::SetProperty (csStringID propertyId, const char* s)
{
  UpdateProperties ();

  if (!propcount) return false;

  for (size_t i=0; i<(*propcount); i++)
  {
    if (props[i].id == propertyId)
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
	  delete[] (*ptr);
	*ptr = csStrNew (s);
	return true;
      }
      else return false;
  }
  return false; 
}

bool celPcCommon::SetProperty (csStringID propertyId, const csVector2& v)
{
  UpdateProperties ();

  if (!propcount) return false;

  for (size_t i=0; i<(*propcount); i++)
  {
    if (props[i].id == propertyId)
      if (props[i].datatype == CEL_DATA_VECTOR2)
      {
        if (!propdata[i])
	{
	  csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	    "cel.celpccommon.setproperty",
	    "Property %s from %s is not correctly set up!",
	    pl->FetchString (propertyId), GetName ());
	  return false;
	}
	((csVector2*)(propdata[i]))[0] = v;
	return true;
      }
      else return false;
  }
  return false; 
}

bool celPcCommon::SetProperty (csStringID propertyId, const csVector3& v)
{
  UpdateProperties ();

  if (!propcount) return false;

  for (size_t i=0; i<(*propcount); i++)
  {
    if (props[i].id == propertyId)
      if (props[i].datatype == CEL_DATA_VECTOR3)
      {
        if (!propdata[i])
	{
	  csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	    "cel.celpccommon.setproperty",
	    "Property %s from %s is not correctly set up!",
	    pl->FetchString (propertyId), GetName ());
	  return false;
	}
	((csVector3*)(propdata[i]))[0] = v;
	return true;
      }
      else return false;
  }
  return false; 
}

bool celPcCommon::SetProperty (csStringID propertyId, const csColor& v)
{
  UpdateProperties ();

  if (!propcount) return false;

  for (size_t i=0; i<(*propcount); i++)
  {
    if (props[i].id == propertyId)
      if (props[i].datatype == CEL_DATA_COLOR)
      {
        if (!propdata[i])
	{
	  csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	    "cel.celpccommon.setproperty",
	    "Property %s from %s is not correctly set up!",
	    pl->FetchString (propertyId), GetName ());
	  return false;
	}
	((csColor*)(propdata[i]))[0] = v;
	return true;
      }
      else return false;
  }
  return false; 
}

bool celPcCommon::SetProperty (csStringID propertyId, iCelPropertyClass* v)
{
  UpdateProperties ();

  if (!propcount) return false;

  for (size_t i=0; i<(*propcount); i++)
  {
    if (props[i].id == propertyId)
      if (props[i].datatype == CEL_DATA_PCLASS)
      {
        if (!propdata[i])
	{
	  csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	    "cel.celpccommon.setproperty",
	    "Property %s from %s is not correctly set up!",
	    pl->FetchString (propertyId), GetName ());
	  return false;
	}
	((iCelPropertyClass**)(propdata[i]))[0] = v;
	return true;
      }
      else return false;
  }
  return false; 
}

bool celPcCommon::SetProperty (csStringID propertyId, iCelEntity* v)
{
  UpdateProperties ();

  if (!propcount) return false;

  for (size_t i=0; i<(*propcount); i++)
  {
    if (props[i].id == propertyId)
      if (props[i].datatype == CEL_DATA_ENTITY)
      {
        if (!propdata[i])
	{
	  csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	    "cel.celpccommon.setproperty",
	    "Property %s from %s is not correctly set up!",
	    pl->FetchString (propertyId), GetName ());
	  return false;
	}
	((iCelEntity**)(propdata[i]))[0] = v;
	return true;
      }
      else return false;
  }
  return false; 
}

bool celPcCommon::SetProperty (csStringID propertyId, iBase* v)
{
  UpdateProperties ();

  if (!propcount) return false;

  for (size_t i=0; i<(*propcount); i++)
  {
    if (props[i].id == propertyId)
      if (props[i].datatype == CEL_DATA_IBASE)
      {
        if (!propdata[i])
	{
	  csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	    "cel.celpccommon.setproperty",
	    "Property %s from %s is not correctly set up!",
	    pl->FetchString (propertyId), GetName ());
	  return false;
	}
	((iBase**)(propdata[i]))[0] = v;
	return true;
      }
      else return false;
  }
  return false; 
}

celDataType celPcCommon::GetPropertyOrActionType (csStringID propertyId)
{
  UpdateProperties ();

  if (!propcount) return CEL_DATA_NONE;

  for (size_t i=0; i<(*propcount); i++)
    if (props[i].id == propertyId)
      return props[i].datatype;
  return CEL_DATA_NONE;
}

bool celPcCommon::IsPropertyReadOnly (csStringID propertyId)
{
  UpdateProperties ();

  for (size_t i=0; i<(*propcount); i++)
    if (props[i].id == propertyId)
      return props[i].readonly;
  return true;
}

long celPcCommon::GetPropertyLong (csStringID propertyId)
{
  UpdateProperties ();

  if (!propcount) return 0;

  for (size_t i=0; i<(*propcount); i++)
  {
    if (props[i].id == propertyId)
      if (props[i].datatype == CEL_DATA_LONG)
      {
        if (!propdata[i])
	{
	  csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	    "cel.celpccommon.getproperty",
	    "Property %s from %s is not correctly set up!",
	    pl->FetchString (propertyId), GetName ());
	  return 0;
	}
	return ((long*)(propdata[i]))[0];
      }
      else return 0;
  }
  return 0;
}

float celPcCommon::GetPropertyFloat (csStringID propertyId)
{
  UpdateProperties ();

  if (!propcount) return 0;

  for (size_t i=0; i<(*propcount); i++)
  {
    if (props[i].id == propertyId)
      if (props[i].datatype == CEL_DATA_FLOAT)
      {
        if (!propdata[i])
	{
	  csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	    "cel.celpccommon.getproperty",
	    "Property %s from %s is not correctly set up!",
	    pl->FetchString (propertyId), GetName ());
	  return 0;
	}
	return ((float*)(propdata[i]))[0];
      }
      else return 0;
  }
  return 0; 
}

bool celPcCommon::GetPropertyBool (csStringID propertyId)
{
  UpdateProperties ();

  if (!propcount) return false;

  for (size_t i=0; i<(*propcount); i++)
  {
    if (props[i].id == propertyId)
      if (props[i].datatype == CEL_DATA_BOOL)
      {
        if (!propdata[i])
	{
	  csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	    "cel.celpccommon.getproperty",
	    "Property %s from %s is not correctly set up!",
	    pl->FetchString (propertyId), GetName ());
	  return false;
	}
        return ((((long*)(propdata[i]))[0])? true:false);
      }
      else return false;
  }
  return false; 
}

const char* celPcCommon::GetPropertyString (csStringID propertyId)
{
  UpdateProperties ();

  if (!propcount) return 0;
  
  for (size_t i=0; i<(*propcount); i++)
  {
    if (props[i].id == propertyId)
      if (props[i].datatype == CEL_DATA_STRING)
      {
        if (!propdata[i])
	{
	  csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	    "cel.celpccommon.getproperty",
	    "Property %s from %s is not correctly set up!",
	    pl->FetchString (propertyId), GetName ());
	  return 0;
	}
	return ((const char**)(propdata[i]))[0];
      }
      else return 0;
  }
  return 0; 
}

bool celPcCommon::GetPropertyVector (csStringID propertyId, csVector2& v)
{
  UpdateProperties ();

  if (!propcount) return false;

  for (size_t i=0; i<(*propcount); i++)
  {
    if (props[i].id == propertyId)
      if (props[i].datatype == CEL_DATA_VECTOR2)
      {
        if (!propdata[i])
	{
	  csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	    "cel.celpccommon.getproperty",
	    "Property %s from %s is not correctly set up!",
	    pl->FetchString (propertyId), GetName ());
	  return false;
	}
	v = ((csVector2*)(propdata[i]))[0];
	return true;
      }
      else return false;
  }
  return false;
}

bool celPcCommon::GetPropertyVector (csStringID propertyId, csVector3& v)
{
  UpdateProperties ();

  if (!propcount) return false;

  for (size_t i=0; i<(*propcount); i++)
  {
    if (props[i].id == propertyId)
      if (props[i].datatype == CEL_DATA_VECTOR3)
      {
        if (!propdata[i])
	{
	  csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	    "cel.celpccommon.getproperty",
	    "Property %s from %s is not correctly set up!",
	    pl->FetchString (propertyId), GetName ());
	  return false;
	}
	v = ((csVector3*)(propdata[i]))[0];
	return true;
      }
      else return false;
  }
  return false;
}

bool celPcCommon::GetPropertyColor (csStringID propertyId, csColor& v )
{
  UpdateProperties ();

  if (!propcount) return false;

  for (size_t i=0; i<(*propcount); i++)
  {
    if (props[i].id == propertyId)
      if (props[i].datatype == CEL_DATA_COLOR)
      {
        if (!propdata[i])
	{
	  csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	    "cel.celpccommon.getproperty",
	    "Property %s from %s is not correctly set up!",
	    pl->FetchString (propertyId), GetName ());
	  return false;
	}
	v = ((csColor*)(propdata[i]))[0];
	return true;
      }
      else return false;
  }
  return false;
}

iCelPropertyClass* celPcCommon::GetPropertyPClass (csStringID propertyId)
{
  UpdateProperties ();

  if (!propcount) return 0;
  
  for (size_t i=0; i<(*propcount); i++)
  {
    if (props[i].id == propertyId)
      if (props[i].datatype == CEL_DATA_PCLASS)
      {
        if (!propdata[i])
	{
	  csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	    "cel.celpccommon.getproperty",
	    "Property %s from %s is not correctly set up!",
	    pl->FetchString (propertyId), GetName ());
	  return 0;
	}
	return ((iCelPropertyClass**)(propdata[i]))[0];
      }
      else return 0;
  }
  return 0; 
}

iCelEntity* celPcCommon::GetPropertyEntity (csStringID propertyId)
{
  UpdateProperties ();

  if (!propcount) return 0;
  
  for (size_t i=0; i<(*propcount); i++)
  {
    if (props[i].id == propertyId)
      if (props[i].datatype == CEL_DATA_ENTITY)
      {
        if (!propdata[i])
	{
	  csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	    "cel.celpccommon.getproperty",
	    "Property %s from %s is not correctly set up!",
	    pl->FetchString (propertyId), GetName ());
	  return 0;
	}
	return ((iCelEntity**)(propdata[i]))[0];
      }
      else return 0;
  }
  return 0; 
}

iBase* celPcCommon::GetPropertyIBase (csStringID propertyId)
{
  UpdateProperties ();

  if (!propcount) return 0;
  
  for (size_t i=0; i<(*propcount); i++)
  {
    if (props[i].id == propertyId)
      if (props[i].datatype == CEL_DATA_IBASE)
      {
        if (!propdata[i])
	{
	  csReport (object_reg, CS_REPORTER_SEVERITY_WARNING,
	    "cel.celpccommon.getproperty",
	    "Property %s from %s is not correctly set up!",
	    pl->FetchString (propertyId), GetName ());
	  return 0;
	}
	return ((iBase**)(propdata[i]))[0];
      }
      else return 0;
  }
  return 0; 
}

const char* celPcCommon::GetPropertyOrActionDescription (csStringID propertyId)
{
  UpdateProperties ();

  if (!propcount) return 0;

  for (size_t i=0; i<(*propcount); i++)
    if (props[i].id == propertyId)
      return props[i].desc;
  return 0;
}

size_t celPcCommon::GetPropertyAndActionCount()
{
  UpdateProperties ();

  if (!propcount) return 0;

  return (*propcount);
}

csStringID celPcCommon::GetPropertyOrActionID (size_t i)
{ 
  UpdateProperties ();

  if (!props) return csInvalidStringID;
  
  return props[i].id; 
}


//---------------------------------------------------------------------------

