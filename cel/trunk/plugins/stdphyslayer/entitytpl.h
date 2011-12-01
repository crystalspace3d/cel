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

#ifndef __CEL_PLIMP_ENTITYFACT__
#define __CEL_PLIMP_ENTITYFACT__

#include "csutil/csobject.h"
#include "csutil/refarr.h"
#include "physicallayer/entitytpl.h"
#include "physicallayer/datatype.h"
#include "plugins/stdphyslayer/pl.h"
#include "propclass/chars.h"

#include "tools/parameters.h"

class celPlLayer;

struct ccfPropAct
{
  csStringID id;
  // If data.type == CEL_DATA_NONE then params will be used (action).
  celData data;
  csHash<csRef<iParameter>, csStringID> params;
};

struct ccfMessage
{
  csStringID msgid;
  csHash<csRef<iParameter>, csStringID> params;
};

class celPropertyClassTemplate : public scfImplementation1<
	celPropertyClassTemplate, iCelPropertyClassTemplate>
{
private:
  csString name;
  csString tag;
  csArray<ccfPropAct> properties;

  ccfPropAct& Create (csStringID id);

public:
  celPropertyClassTemplate ();
  virtual ~celPropertyClassTemplate ();

  const csArray<ccfPropAct>& GetProperties () const { return properties; }
  void Merge (celPropertyClassTemplate* other);

  virtual void SetName (const char* name)
  {
    celPropertyClassTemplate::name = name;
  }
  virtual const char* GetName () const { return name; }
  const csString& GetNameStr () const { return name; }
  virtual void SetTag (const char* tag) { celPropertyClassTemplate::tag = tag; }
  virtual const char* GetTag () const
  { return tag.IsEmpty () ? 0 : (const char*)tag; }
  const csString& GetTagStr () const { return tag; }
  virtual void SetPropertyVariable (csStringID propertyID, celDataType type,
  	const char* varname);
  virtual void SetProperty (csStringID propertyID, long value);
  virtual void SetProperty (csStringID propertyID, float value);
  virtual void SetProperty (csStringID propertyID, bool value);
  virtual void SetProperty (csStringID propertyID, const char* value);
  virtual void SetProperty (csStringID propertyID, const csVector2& value);
  virtual void SetProperty (csStringID propertyID, const csVector3& value);
  virtual void SetProperty (csStringID propertyID, const csColor& value);
  virtual void SetProperty (csStringID propertyID,
  	iCelPropertyClass* value);
  virtual void SetProperty (csStringID propertyID, iCelEntity* entity);
  virtual void PerformAction (csStringID actionID,
  	const csHash<csRef<iParameter>, csStringID>& params);
  virtual size_t GetPropertyCount () const { return properties.GetSize (); }
  virtual csRef<iCelParameterIterator> GetProperty (size_t idx,
		  csStringID& id, celData& data) const;
  virtual size_t FindProperty (csStringID id) const;
  virtual void RemoveAllProperties ();
};

/**
 * Implementation of iCelEntityTemplate.
 */
class celEntityTemplate : public scfImplementationExt2<
	celEntityTemplate, csObject, iCelEntityTemplate, iTemplateCharacteristics>
{
private:
  csRefArray<celPropertyClassTemplate> propclasses;
  csString layer, behaviour;
  csArray<ccfMessage> messages;
  csSet<csStringID> classes;
  csHash<float, csStringBase> characteristics;
  csRefArray<iCelEntityTemplate> parents;

  celPropertyClassTemplate* FindPCTemplate (const char* name, const char* tag);

public:
  celEntityTemplate ();
  virtual ~celEntityTemplate ();

  virtual void AddClass (csStringID cls);
  virtual void RemoveClass (csStringID cls);
  virtual bool HasClass (csStringID cls);
  virtual const csSet<csStringID>& GetClasses () const { return classes; }

  const csArray<ccfMessage>& GetMessages () const { return messages; }
  virtual size_t GetMessageCount () const { return messages.GetSize (); }
  virtual csRef<iCelParameterIterator> GetMessage (size_t idx,
		  csStringID& id) const;

  virtual iCelPropertyClassTemplate* CreatePropertyClassTemplate ();
  virtual iCelPropertyClassTemplate* FindPropertyClassTemplate (const char* name,
      const char* tag);
  virtual size_t GetPropertyClassTemplateCount () const
  {
    return propclasses.GetSize ();
  }
  virtual iCelPropertyClassTemplate* GetPropertyClassTemplate (size_t idx) const
  {
    return propclasses[idx];
  }
  virtual void SetBehaviour (const char* layer, const char* behaviour)
  {
    celEntityTemplate::layer = layer;
    celEntityTemplate::behaviour = behaviour;
  }
  virtual void AddMessage (csStringID msgid,
      csHash<csRef<iParameter>, csStringID>& params);
  virtual const char* GetBehaviourLayer () const { return layer; }
  const char* GetLayer () const { return layer; }
  virtual const char* GetBehaviour () const { return behaviour; }
  const csRefArray<iCelEntityTemplate>& GetParentsInt () const
  {
    return parents;
  }
  const csRefArray<celPropertyClassTemplate>& GetPropClasses () const
  {
    return propclasses;
  }

  virtual iObject* QueryObject () { return this; }
  virtual const char* GetName () const { return csObject::GetName (); }
  virtual void SetName (const char* n) { csObject::SetName (n); }

  virtual void Merge (iCelEntityTemplate* tpl);
  virtual void AddParent (iCelEntityTemplate* tpl);
  virtual void RemoveParent (iCelEntityTemplate* tpl);
  virtual csPtr<iCelEntityTemplateIterator> GetParents () const;

  virtual iTemplateCharacteristics* GetCharacteristics ()
  {
    return this;
  }

  // ------ For iTemplateCharacteristics ------------------------

  virtual void SetCharacteristic (const char* name, float value);
  virtual float GetCharacteristic (const char* name) const;
  virtual void ClearCharacteristic (const char* name);
  virtual bool HasCharacteristic (const char* name) const;
  virtual void ClearAll ();
};

#endif // __CEL_PLIMP_ENTITYFACT__

