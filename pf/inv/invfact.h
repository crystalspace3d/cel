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

#ifndef __CEL_PF_INVFACT__
#define __CEL_PF_INVFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "csutil/csvector.h"
#include "pl/propclas.h"
#include "pl/propfact.h"
#include "pf/inv.h"
#include "pf/chars.h"

struct iCelEntity;

/**
 * Factory for inventory.
 */
class celPfInventory : public iCelPropertyClassFactory
{
private:

public:
  celPfInventory (iBase* parent);
  virtual ~celPfInventory ();
  bool Initialize (iObjectRegistry* object_reg);

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "pfinventory"; }
  virtual iCelPropertyClass* CreatePropertyClass (const char* type);
  virtual int GetTypeCount () const { return 2; }
  virtual const char* GetTypeName (int idx) const;

  struct Component : public iComponent
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPfInventory);
    virtual bool Initialize (iObjectRegistry* p)
    { return scfParent->Initialize (p); }
  } scfiComponent;
};

/**
 * This is an inventory property class.
 */
class celPcInventory : public iCelPropertyClass
{
private:
  iCelEntity* entity;
  csVector contents;
  csVector constraints;

  struct constraint
  {
    char* charName;
    float minValue;
    float maxValue;
    float totalMaxValue;
    float currentValue;
    bool strict;
  };

  constraint* FindConstraint (const char* name) const;
  constraint* NewConstraint (const char* name);
  void UpdateConstraints (iCelEntity* entity, bool add);

public:
  celPcInventory ();
  virtual ~celPcInventory ();

  bool AddEntity (iCelEntity* entity);
  void RemoveEntity (iCelEntity* entity);
  void RemoveAll ();
  int GetEntityCount () const { return contents.Length () ; }
  iCelEntity* GetEntity (int idx) const;
  void SetStrictCharacteristics (const char* charName, bool strict);
  bool HasStrictCharacteristics (const char* charName) const;
  void SetConstraints (const char* charName, float minValue, float maxValue,
		  float totalMaxValue);
  bool GetConstraints (const char* charName, float& minValue, float& maxValue,
		  float& totalMaxValue) const;
  void RemoveConstraints (const char* charName);
  float GetCurrentCharacteristic (const char* charName) const;
  const char* TestAddEntity (iCelEntity* entity);

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "pcinventory"; }
  virtual iCelEntity* GetEntity () { return entity; }
  virtual void SetEntity (iCelEntity* entity);

  struct PcInventory : public iPcInventory
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcInventory);
    virtual bool AddEntity (iCelEntity* entity)
    {
      return scfParent->AddEntity (entity);
    }
    virtual void RemoveEntity (iCelEntity* entity)
    {
      scfParent->RemoveEntity (entity);
    }
    virtual void RemoveAll ()
    {
      scfParent->RemoveAll ();
    }
    virtual int GetEntityCount () const
    {
      return scfParent->GetEntityCount ();
    }
    virtual iCelEntity* GetEntity (int idx) const
    {
      return scfParent->GetEntity (idx);
    }
    virtual void SetStrictCharacteristics (const char* charName, bool strict)
    {
      scfParent->SetStrictCharacteristics (charName, strict);
    }
    virtual bool HasStrictCharacteristics (const char* charName) const
    {
      return scfParent->HasStrictCharacteristics (charName);
    }
    virtual void SetConstraints (const char* charName, float minValue, float maxValue,
		  float totalMaxValue)
    {
      scfParent->SetConstraints (charName, minValue, maxValue, totalMaxValue);
    }
    virtual bool GetConstraints (const char* charName, float& minValue, float& maxValue,
		  float& totalMaxValue) const
    {
      return scfParent->GetConstraints (charName, minValue, maxValue, totalMaxValue);
    }
    virtual void RemoveConstraints (const char* charName)
    {
      scfParent->RemoveConstraints (charName);
    }
    virtual float GetCurrentCharacteristic (const char* charName) const
    {
      return scfParent->GetCurrentCharacteristic (charName);
    }
    virtual const char* TestAddEntity (iCelEntity* entity)
    {
      return scfParent->TestAddEntity (entity);
    }
  } scfiPcInventory;
};

/**
 * This is a characteristics property class.
 */
class celPcCharacteristics : public iCelPropertyClass
{
private:
  iCelEntity* entity;
  csVector chars;

  struct charact
  {
    char* name;
    float value;
  };
  
  charact* FindCharact (const char* name) const;

public:
  celPcCharacteristics ();
  virtual ~celPcCharacteristics ();

  void SetCharProperty (const char* name, float value);
  float GetCharProperty (const char* name) const;
  void ClearProperty (const char* name);
  bool HasProperty (const char* name) const;
  void ClearAll ();

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "pccharacteristics"; }
  virtual iCelEntity* GetEntity () { return entity; }
  virtual void SetEntity (iCelEntity* entity);

  struct PcCharacteristics : public iPcCharacteristics
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcCharacteristics);
    virtual void SetCharProperty (const char* name, float value)
    {
      scfParent->SetCharProperty (name, value);
    }
    virtual float GetCharProperty (const char* name) const
    {
      return scfParent->GetCharProperty (name);
    }
    virtual void ClearProperty (const char* name)
    {
      scfParent->ClearProperty (name);
    }
    virtual bool HasProperty (const char* name) const
    {
      return scfParent->HasProperty (name);
    }
    virtual void ClearAll ()
    {
      scfParent->ClearAll ();
    }
  } scfiPcCharacteristics;
};

#endif // __CEL_PF_INVFACT__

