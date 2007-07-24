/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein

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
#include "plugins/stdphyslayer/entitytpl.h"

#include "behaviourlayer/behave.h"

//---------------------------------------------------------------------------

celPropertyClassTemplate::celPropertyClassTemplate ()
	: scfImplementationType (this)
{
}

celPropertyClassTemplate::~celPropertyClassTemplate ()
{
}

ccfPropAct& celPropertyClassTemplate::Create (csStringID id)
{
  ccfPropAct prop;
  prop.id = id;
  size_t i = properties.Push (prop);
  return properties[i];
}

void celPropertyClassTemplate::SetPropertyVariable (csStringID propertyID,
    celDataType type, const char* varname)
{
  Create (propertyID).data.SetParameter (varname, type);
}

void celPropertyClassTemplate::SetProperty (csStringID propertyID, long value)
{
  Create (propertyID).data.Set ((int32)value);
}

void celPropertyClassTemplate::SetProperty (csStringID propertyID, float value)
{
  Create (propertyID).data.Set (value);
}

void celPropertyClassTemplate::SetProperty (csStringID propertyID, bool value)
{
  Create (propertyID).data.Set (value);
}

void celPropertyClassTemplate::SetProperty (csStringID propertyID,
	const char* value)
{
  Create (propertyID).data.Set (value);
}

void celPropertyClassTemplate::SetProperty (csStringID propertyID,
	const csVector2& value)
{
  Create (propertyID).data.Set (value);
}

void celPropertyClassTemplate::SetProperty (csStringID propertyID,
	const csVector3& value)
{
  Create (propertyID).data.Set (value);
}

void celPropertyClassTemplate::SetProperty (csStringID propertyID,
	const csColor& value)
{
  Create (propertyID).data.Set (value);
}

void celPropertyClassTemplate::SetProperty (csStringID propertyID,
	iCelPropertyClass* value)
{
  Create (propertyID).data.Set (value);
}

void celPropertyClassTemplate::SetProperty (csStringID propertyID,
	iCelEntity* entity)
{
  Create (propertyID).data.Set (entity);
}

void celPropertyClassTemplate::PerformAction (csStringID actionID,
  	iCelParameterBlock* params)
{
  Create (actionID).params = params;
}

//---------------------------------------------------------------------------

celEntityTemplate::celEntityTemplate () : scfImplementationType (this)
{
}

celEntityTemplate::~celEntityTemplate ()
{
}

iCelPropertyClassTemplate* celEntityTemplate::CreatePropertyClassTemplate ()
{
  celPropertyClassTemplate* f = new celPropertyClassTemplate ();
  propclasses.Push (f);
  f->DecRef ();
  return f;
}

void celEntityTemplate::AddMessage (const char* msgid,
    iCelParameterBlock* params)
{
  size_t i = messages.Push (ccfMessage ());
  messages[i].msgid = msgid;
  messages[i].params = params;
}

void celEntityTemplate::AddClass (csStringID cls)
{
  classes.Add (cls);
}

void celEntityTemplate::RemoveClass (csStringID cls)
{
  classes.Delete (cls);
}

bool celEntityTemplate::HasClass (csStringID cls)
{
  return classes.In (cls);
}

//---------------------------------------------------------------------------

