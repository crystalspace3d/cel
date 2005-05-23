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

class celPlLayer;

struct ccfPropAct
{
  csStringID id;
  celData data;	// If data.type == CEL_DATA_NONE then params will be used (action).
  csRef<iCelParameterBlock> params;
};

class celPropertyClassTemplate : public iCelPropertyClassTemplate
{
private:
  csString name;
  csString tag;
  csArray<ccfPropAct> properties;

  ccfPropAct& FindCreate (csStringID id);

public:
  celPropertyClassTemplate ();
  virtual ~celPropertyClassTemplate ();

  const csArray<ccfPropAct>& GetProperties () const { return properties; }

  SCF_DECLARE_IBASE;

  virtual void SetName (const char* name)
  {
    celPropertyClassTemplate::name = name;
  }
  virtual const char* GetName () const { return name; }
  virtual void SetTag (const char* tag) { celPropertyClassTemplate::tag = tag; }
  virtual const char* GetTag () const { return tag; }
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
  	iCelParameterBlock* params);
};

/**
 * Implementation of iCelEntityTemplate.
 */
class celEntityTemplate : public csObject
{
private:
  csRefArray<celPropertyClassTemplate> propclasses;
  csString layer, behaviour;

public:
  celEntityTemplate ();
  virtual ~celEntityTemplate ();

  iCelPropertyClassTemplate* CreatePropertyClassTemplate ();
  void SetBehaviour (const char* layer, const char* behaviour)
  {
    celEntityTemplate::layer = layer;
    celEntityTemplate::behaviour = behaviour;
  }
  const char* GetLayer () const { return layer; }
  const char* GetBehaviour () const { return behaviour; }
  const csRefArray<celPropertyClassTemplate> GetPropClasses () const
  {
    return propclasses;
  }

  SCF_DECLARE_IBASE_EXT (csObject);

  //------------------- iCelEntityTemplate implementation --------------------
  struct CelEntityTemplate : public iCelEntityTemplate
  {
    SCF_DECLARE_EMBEDDED_IBASE (celEntityTemplate);
    celEntityTemplate* GetCelEntityTemplate () const
    {
      return (celEntityTemplate*)scfParent;
    }
    virtual iObject* QueryObject () { return scfParent; }
    virtual const char* GetName () const { return scfParent->GetName (); }
    virtual void SetName (const char* n) { scfParent->SetName (n); }
    virtual iCelPropertyClassTemplate* CreatePropertyClassTemplate ()
    {
      return scfParent->CreatePropertyClassTemplate ();
    }
    virtual void SetBehaviour (const char* layer, const char* behaviour)
    {
      scfParent->SetBehaviour (layer, behaviour);
    }
  } scfiCelEntityTemplate;
  friend struct CelEntityTemplate;
};

#endif // __CEL_PLIMP_ENTITYFACT__

