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
#include "iutil/objreg.h"
#include "csutil/debug.h"
#include "pf/common/stdpcimp.h"
#include "pl/pl.h"
#include "pl/entity.h"
#include "pl/propchg.h"
#include "pl/persist.h"
#include "bl/behave.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celPcCommon)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClass)
SCF_IMPLEMENT_IBASE_END

celPcCommon::celPcCommon (iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (NULL);
  celPcCommon::object_reg = object_reg;
  DG_ADDI (this, "celPcCommon()");
  entity = NULL;

  propcount = NULL;
}

celPcCommon::~celPcCommon ()
{
  DG_REM (this);
}

void celPcCommon::SetEntity (iCelEntity* entity)
{
  celPcCommon::entity = entity;
}

void celPcCommon::FirePropertyChangeCallback (int propertyId)
{
  int i;
  for (i = 0 ; i < callbacks.Length () ; i++)
  {
    iCelPropertyChangeCallback* cb = callbacks[i];
    cb->PropertyChanged (propertyId, this);
  }
}

bool celPcCommon::AddPropertyChangeCallback (iCelPropertyChangeCallback* cb)
{
  int idx = callbacks.Find (cb);
  if (idx != -1) return false;
  callbacks.Push (cb);
  return true;
}

bool celPcCommon::RemovePropertyChangeCallback (
  	iCelPropertyChangeCallback* cb)
{
  int idx = callbacks.Find (cb);
  if (idx == -1) return false;
  callbacks.Delete (idx);
  return true;
}


bool celPcCommon::SetProperty( csStringID propertyId, long l )
{
  UpdateProperties( object_reg );

  if( !propcount ) return false;

  for( int i=0; i<(*propcount); i++ )
  {
    if( props[i].id == propertyId )
      if( props[i].datatype == CEL_DATA_LONG ) 
      {
	((long*)(propdata[i]))[0] = l;
	return true;
      }
      else return false;
  }
  return false; 
}

bool celPcCommon::SetProperty( csStringID propertyId, float f )
{
  UpdateProperties( object_reg );

  if( !propcount ) return false;

  for( int i=0; i<(*propcount); i++ )
  {
    if( props[i].id == propertyId )
      if( props[i].datatype == CEL_DATA_FLOAT ) 
      {
	((float*)(propdata[i]))[0] = f;
	return true;
      }
      else return false;
  }
  return false; 
}

bool celPcCommon::SetProperty( csStringID propertyId, bool b )
{
  UpdateProperties( object_reg );

  if( !propcount ) return false;

  for( int i=0; i<(*propcount); i++ )
  {
    if( props[i].id == propertyId )
      if( props[i].datatype == CEL_DATA_BOOL ) 
      {
	((bool*)(propdata[i]))[0] = b;
	return true;
      }
      else return false;
  }
  return false; 
}

bool celPcCommon::SetProperty( csStringID propertyId, const char* s )
{
  UpdateProperties( object_reg );

  if( !propcount ) return false;

  for( int i=0; i<(*propcount); i++ )
  {
    if( props[i].id == propertyId )
      if( props[i].datatype == CEL_DATA_STRING ) 
      {
	char** ptr = (char**) propdata[i];
	if ( *ptr != s )
	  delete[] (*ptr);
	*ptr = csStrNew( s );
	return true;
      }
      else return false;
  }
  return false; 
}

bool celPcCommon::SetProperty( csStringID propertyId, const csVector3& v )
{
  UpdateProperties( object_reg );

  if( !propcount ) return false;

  for( int i=0; i<(*propcount); i++ )
  {
    if( props[i].id == propertyId )
      if( props[i].datatype == CEL_DATA_VECTOR3 ) 
      {
	((csVector3*)(propdata[i]))[0] = v;
	return true;
      }
      else return false;
  }
  return false; 
}

celDataType celPcCommon::GetPropertyOrActionType( csStringID propertyId )
{
  UpdateProperties( object_reg ); 

  if( !propcount ) return CEL_DATA_NONE;

  for( int i=0; i<(*propcount); i++ )
    if( props[i].id == propertyId )
      return props[i].datatype;
  return CEL_DATA_NONE;
}

bool celPcCommon::IsPropertyReadOnly( csStringID propertyId )
{
  UpdateProperties( object_reg );

  for( int i=0; i<(*propcount); i++ )
    if( props[i].id == propertyId )
      return props[i].readonly;
  return true;
}

long celPcCommon::GetPropertyLong( csStringID propertyId )
{
  UpdateProperties( object_reg ); 

  if( !propcount ) return 0;

  for( int i=0; i<(*propcount); i++ )
  {
    if( props[i].id == propertyId )
      if( props[i].datatype == CEL_DATA_LONG ) 
      {
	return ((long*)(propdata[i]))[0];
      } else return 0;
  }
  return 0;
}

float celPcCommon::GetPropertyFloat( csStringID propertyId )
{
  UpdateProperties( object_reg );

  if( !propcount ) return 0;

  for( int i=0; i<(*propcount); i++ )
  {
    if( props[i].id == propertyId )
      if( props[i].datatype == CEL_DATA_FLOAT ) 
      {
	return ((float*)(propdata[i]))[0];
      } else return 0;
  }
  return 0; 
}

bool celPcCommon::GetPropertyBool( csStringID propertyId )
{
  UpdateProperties( object_reg );

  if( !propcount ) return false;

  for( int i=0; i<(*propcount); i++ )
  {
    if( props[i].id == propertyId )
      if( props[i].datatype == CEL_DATA_BOOL ) 
      {
	return ((long*)(propdata[i]))[0];
      } else return false;
  }
  return false; 
}

const char* celPcCommon::GetPropertyString( csStringID propertyId )
{
  UpdateProperties( object_reg );

  if( !propcount ) return 0;
  
  for( int i=0; i<(*propcount); i++ )
  {
    if( props[i].id == propertyId )
      if( props[i].datatype == CEL_DATA_STRING ) 
      {
	return ((const char**)(propdata[i]))[0];
      }
      else return 0;
  }
  return 0; 
}

bool celPcCommon::GetPropertyVector (csStringID propertyId, csVector3& v )
{
  UpdateProperties( object_reg ); 

  if( !propcount ) return false;

  for( int i=0; i<(*propcount); i++ )
  {
    if( props[i].id == propertyId )
      if( props[i].datatype == CEL_DATA_VECTOR3 ) 
      {
	v = ((csVector3*)(propdata[i]))[0];
	return true;
      }
      else return false;
  }
  return false;
}

const char* celPcCommon::GetPropertyOrActionDescription( csStringID propertyId )
{
  UpdateProperties( object_reg ); 

  if( !propcount ) return 0;

  for( int i=0; i<(*propcount); i++ )
    if( props[i].id == propertyId )
      return props[i].desc;
  return 0;
}

int celPcCommon::GetPropertyAndActionCount() const
{
  UpdateProperties( object_reg ); 

  if( !propcount ) return 0;

  return (*propcount);
}

csStringID celPcCommon::GetPropertyOrActionID( int i ) 
{ 
  UpdateProperties( object_reg );

  if( !props ) return csInvalidStringID;
  
  return props[i].id; 
}


//---------------------------------------------------------------------------

