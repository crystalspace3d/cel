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

class celCharIterator : public scfImplementation1<celCharIterator,
	iCharacteristicsIterator>
{
private:
  csHash<float, csStringBase>::ConstGlobalIterator it;
  csString key;

public:
  celCharIterator (const csHash<float, csStringBase>::ConstGlobalIterator it) :
	  scfImplementationType (this), it (it)
  {
  }
  virtual ~celCharIterator () { }
  virtual bool HasNext () const { return it.HasNext (); }
  virtual const char* Next (float& value)
  {
    value = it.Next (key);
    return key;
  }
};

//---------------------------------------------------------------------------

class celParameterIterator : public scfImplementation1<celParameterIterator,
	iCelParameterIterator>
{
private:
  csHash<csRef<iParameter>, csStringID>::ConstGlobalIterator it;

public:
  celParameterIterator (const csHash<csRef<iParameter>, csStringID>::ConstGlobalIterator it) :
	  scfImplementationType (this), it (it)
  {
  }
  virtual ~celParameterIterator () { }
  virtual bool HasNext () const { return it.HasNext (); }
  virtual iParameter* Next (csStringID& id)
  {
    csRef<iParameter> par = it.Next (id);
    return par;
  }
};

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

void celPropertyClassTemplate::ReplaceActionParameters (size_t idx,
  	const csHash<csRef<iParameter>, csStringID>& params)
{
  properties[idx].params = params;
}

void celPropertyClassTemplate::Merge (celPropertyClassTemplate* other)
{
  for (size_t i = 0 ; i < other->properties.GetSize () ; i++)
  {
    ccfPropAct& prop = other->properties.Get (i);
    properties.Push (prop);
  }
}

csRef<iCelParameterIterator> celPropertyClassTemplate::GetProperty (size_t idx,
		  csStringID& id, celData& data) const
{
  csRef<celParameterIterator> parit;
  id = properties[idx].id;
  data = properties[idx].data;
  parit.AttachNew (new celParameterIterator (
			  properties[idx].params.GetIterator ()));
  return parit;
}

size_t celPropertyClassTemplate::FindProperty (csStringID id) const
{
  for (size_t i = 0 ; i < properties.GetSize () ; i++)
    if (properties[i].id == id) return i;
  return csArrayItemNotFound;
}

void celPropertyClassTemplate::RemoveAllProperties ()
{
  properties.DeleteAll ();
}

void celPropertyClassTemplate::RemoveProperty (csStringID id)
{
  size_t i = 0;
  while (i < properties.GetSize ())
  {
    if (properties[i].id == id)
      properties.DeleteIndex (i);
    else
      i++;
  }
}

void celPropertyClassTemplate::RemovePropertyByIndex (size_t idx)
{
  properties.DeleteIndex (idx);
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

void celEntityTemplate::RemovePropertyClassTemplate (size_t idx)
{
  propclasses.DeleteIndex (idx);
}

void celEntityTemplate::AddMessage (csStringID msgid,
      csHash<csRef<iParameter>, csStringID>& params)
{
  size_t i = messages.Push (ccfMessage ());
  messages[i].msgid = msgid;
  messages[i].params = params;
}

csRef<iCelParameterIterator> celEntityTemplate::GetMessage (size_t idx,
		  csStringID& id) const
{
  csRef<celParameterIterator> parit;
  id = messages[idx].msgid;
  parit.AttachNew (new celParameterIterator (
			  messages[idx].params.GetIterator ()));
  return parit;
}

void celEntityTemplate::AddClass (csStringID cls)
{
  classes.Add (cls);
}

void celEntityTemplate::RemoveClass (csStringID cls)
{
  classes.Delete (cls);
}

void celEntityTemplate::RemoveClasses ()
{
  classes.DeleteAll ();
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
      if (tag && pct->GetTagStr () == tag)
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

void celEntityTemplate::AddParent (iCelEntityTemplate* tpl)
{
  parents.Push (tpl);
}

void celEntityTemplate::RemoveParent (iCelEntityTemplate* tpl)
{
  parents.Delete (tpl);
}

void celEntityTemplate::RemoveParents ()
{
  parents.DeleteAll ();
}

class celTemplateIterator : public scfImplementation1<celTemplateIterator, iCelEntityTemplateIterator>
{
  csRefArray<iCelEntityTemplate>::ConstIterator it;

public:
  celTemplateIterator (const csRefArray<iCelEntityTemplate>::ConstIterator& it) :
    scfImplementationType (this), it (it)
  {
  }
  virtual ~celTemplateIterator ()
  {
  }
  virtual bool HasNext () const { return it.HasNext (); }
  virtual iCelEntityTemplate* Next ()
  {
    return it.Next ();
  }
};

csPtr<iCelEntityTemplateIterator> celEntityTemplate::GetParents () const
{
  celTemplateIterator* it = new celTemplateIterator (parents.GetIterator ());
  return it;
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

csPtr<iCharacteristicsIterator> celEntityTemplate::GetAllCharacteristics () const
{
  return new celCharIterator (characteristics.GetIterator ());
}

void celEntityTemplate::ClearAll ()
{
  characteristics.DeleteAll ();
}

//---------------------------------------------------------------------------

