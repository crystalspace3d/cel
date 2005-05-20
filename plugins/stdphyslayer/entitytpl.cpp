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
#include "csutil/debug.h"
#include "plugins/stdphyslayer/entitytpl.h"

#include "behaviourlayer/behave.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celPropertyClassTemplate)
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClassTemplate)
SCF_IMPLEMENT_IBASE_END

celPropertyClassTemplate::celPropertyClassTemplate ()
{
  SCF_CONSTRUCT_IBASE (0);
}

celPropertyClassTemplate::~celPropertyClassTemplate ()
{
  SCF_DESTRUCT_IBASE ();
}

celData& celPropertyClassTemplate::FindCreateProperty (csStringID id)
{
  size_t i;
  for (i = 0 ; i < properties.Length () ; i++)
  {
    if (properties[i].id == id) return properties[i].data;
  }
  ccfProp prop;
  prop.id = id;
  i = properties.Push (prop);
  return properties[i].data;
}

ccfAct& celPropertyClassTemplate::FindCreateAction (csStringID id)
{
  size_t i;
  for (i = 0 ; i < actions.Length () ; i++)
  {
    if (actions[i].id == id) return actions[i];
  }
  ccfAct prop;
  prop.id = id;
  i = actions.Push (prop);
  return actions[i];
}

void celPropertyClassTemplate::SetProperty (csStringID propertyID, long value)
{
  FindCreateProperty (propertyID).Set ((int32)value);
}

void celPropertyClassTemplate::SetProperty (csStringID propertyID, float value)
{
  FindCreateProperty (propertyID).Set (value);
}

void celPropertyClassTemplate::SetProperty (csStringID propertyID, bool value)
{
  FindCreateProperty (propertyID).Set (value);
}

void celPropertyClassTemplate::SetProperty (csStringID propertyID,
	const char* value)
{
  FindCreateProperty (propertyID).Set (value);
}

void celPropertyClassTemplate::SetProperty (csStringID propertyID,
	const csVector2& value)
{
  FindCreateProperty (propertyID).Set (value);
}

void celPropertyClassTemplate::SetProperty (csStringID propertyID,
	const csVector3& value)
{
  FindCreateProperty (propertyID).Set (value);
}

void celPropertyClassTemplate::SetProperty (csStringID propertyID,
	const csColor& value)
{
  FindCreateProperty (propertyID).Set (value);
}

void celPropertyClassTemplate::SetProperty (csStringID propertyID,
	iCelPropertyClass* value)
{
  FindCreateProperty (propertyID).Set (value);
}

void celPropertyClassTemplate::SetProperty (csStringID propertyID,
	iCelEntity* entity)
{
  FindCreateProperty (propertyID).Set (entity);
}

void celPropertyClassTemplate::PerformAction (csStringID actionID,
  	iCelParameterBlock* params)
{
  FindCreateAction (actionID).params = params;
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE_EXT (celEntityTemplate)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iCelEntityTemplate)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celEntityTemplate::CelEntityTemplate)
  SCF_IMPLEMENTS_INTERFACE (iCelEntityTemplate)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celEntityTemplate::celEntityTemplate ()
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiCelEntityTemplate);
}

celEntityTemplate::~celEntityTemplate ()
{
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiCelEntityTemplate);
}

iCelPropertyClassTemplate* celEntityTemplate::CreatePropertyClassTemplate ()
{
  celPropertyClassTemplate* f = new celPropertyClassTemplate ();
  propclasses.Push (f);
  f->DecRef ();
  return f;
}

//---------------------------------------------------------------------------

