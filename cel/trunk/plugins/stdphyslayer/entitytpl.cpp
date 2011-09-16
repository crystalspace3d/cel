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
    const csHash<csRef<iParameter>, csStringID>& params)
{
  Create (actionID).params = params;
}

void celPropertyClassTemplate::Merge (celPropertyClassTemplate* other)
{
  for (size_t i = 0 ; i < other->properties.GetSize () ; i++)
  {
    ccfPropAct& prop = other->properties.Get (i);
    properties.Push (prop);
  }
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

iCelPropertyClassTemplate* celEntityTemplate::FindPropertyClassTemplate (
    const char* name, const char* tag)
{
  iCelPropertyClassTemplate* pc = FindPCTemplate (name, tag);
  if (!pc) return 0;
  return static_cast<celPropertyClassTemplate*> (pc);
}

celPropertyClassTemplate* celEntityTemplate::FindPCTemplate (
    const char* name, const char* tag)
{
  for (size_t i = 0 ; i < propclasses.GetSize () ; i++)
  {
    celPropertyClassTemplate* pct = propclasses.Get (i);
    if (pct->GetNameStr () == name)
    {
      if ((tag == 0 || *tag == 0) && pct->GetTagStr ().IsEmpty ())
        return pct;
      if (pct->GetTagStr () == tag)
        return pct;
    }
  }
  return 0;
}

void celEntityTemplate::Merge (iCelEntityTemplate* tpl)
{
  celEntityTemplate* other = static_cast<celEntityTemplate*> (tpl);

  csHash<float, csStringBase>::GlobalIterator itc = other->characteristics.GetIterator ();
  while (itc.HasNext ())
  {
    csStringBase key;
    float amount = itc.Next (key);
    characteristics.Put (key, amount);
  }

  if (!other->layer.IsEmpty ())
    layer = other->layer;
  if (!other->behaviour.IsEmpty ())
    behaviour = other->behaviour;

  for (size_t i = 0 ; i < other->propclasses.GetSize () ; i++)
  {
    celPropertyClassTemplate* pct = other->propclasses.Get (i);
    celPropertyClassTemplate* mergeWith = FindPCTemplate (pct->GetName (),
        pct->GetTag ());
    if (!mergeWith)
    {
      mergeWith = static_cast<celPropertyClassTemplate*> (
          CreatePropertyClassTemplate ());
      mergeWith->SetName (pct->GetName ());
      if (pct->GetTag () && *pct->GetTag ())
        mergeWith->SetTag (pct->GetTag ());
    }
    mergeWith->Merge (pct);
  }

  for (size_t i = 0 ; i < other->messages.GetSize () ; i++)
  {
    ccfMessage& msg = other->messages.Get (i);
    AddMessage (msg.msgid, msg.params);
  }

  csSet<csStringID>::GlobalIterator it = other->classes.GetIterator ();
  while (it.HasNext ())
  {
    csStringID id = it.Next ();
    classes.Add (id);
  }
}

void celEntityTemplate::SetCharacteristic (const char* name, float value)
{
  characteristics.Put (name, value);
}

float celEntityTemplate::GetCharacteristic (const char* name) const
{
  return characteristics.Get (name, 0.0f);
}

void celEntityTemplate::ClearCharacteristic (const char* name)
{
  characteristics.DeleteAll (name);
}

bool celEntityTemplate::HasCharacteristic (const char* name) const
{
  return characteristics.Contains (name);
}

void celEntityTemplate::ClearAll ()
{
  characteristics.DeleteAll ();
}

//---------------------------------------------------------------------------

