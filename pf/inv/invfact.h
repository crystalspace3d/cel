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
struct iObjectRegistry;

/**
 * Factory for inventory.
 */
class celPfInventory : public iCelPropertyClassFactory
{
private:
  iObjectRegistry* object_reg;

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
  iObjectRegistry* object_reg;
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
    bool dirty;
  };

  constraint* FindConstraint (const char* name) const;
  constraint* NewConstraint (const char* name);
  bool TestLocalConstraints (const char* charName);

public:
  celPcInventory (iObjectRegistry* object_reg);
  virtual ~celPcInventory ();

  bool AddEntity (iCelEntity* entity);
  bool RemoveEntity (iCelEntity* entity);
  bool RemoveAll ();
  int GetEntityCount () const { return contents.Length () ; }
  iCelEntity* GetEntity (int idx) const;
  bool SetStrictCharacteristics (const char* charName, bool strict);
  bool HasStrictCharacteristics (const char* charName) const;
  bool SetConstraints (const char* charName, float minValue, float maxValue,
		  float totalMaxValue);
  bool GetConstraints (const char* charName, float& minValue, float& maxValue,
		  float& totalMaxValue) const;
  void RemoveConstraints (const char* charName);
  void RemoveAllConstraints ();
  float GetCurrentCharacteristic (const char* charName) const;

  void MarkDirty (const char* charName);
  bool TestConstraints (const char* charName);
  void Dump ();

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "pcinventory"; }
  virtual iCelEntity* GetEntity () { return entity; }
  virtual void SetEntity (iCelEntity* entity);
  virtual iCelDataBuffer* GetDataBuffer ();
  virtual void Save (iCelDataBuffer* databuf);
  virtual void Load (iCelDataBuffer* databuf);

  struct PcInventory : public iPcInventory
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcInventory);
    virtual bool AddEntity (iCelEntity* entity)
    {
      return scfParent->AddEntity (entity);
    }
    virtual bool RemoveEntity (iCelEntity* entity)
    {
      return scfParent->RemoveEntity (entity);
    }
    virtual bool RemoveAll ()
    {
      return scfParent->RemoveAll ();
    }
    virtual int GetEntityCount () const
    {
      return scfParent->GetEntityCount ();
    }
    virtual iCelEntity* GetEntity (int idx) const
    {
      return scfParent->GetEntity (idx);
    }
    virtual bool SetStrictCharacteristics (const char* charName, bool strict)
    {
      return scfParent->SetStrictCharacteristics (charName, strict);
    }
    virtual bool HasStrictCharacteristics (const char* charName) const
    {
      return scfParent->HasStrictCharacteristics (charName);
    }
    virtual bool SetConstraints (const char* charName,
    	float minValue, float maxValue, float totalMaxValue)
    {
      return scfParent->SetConstraints (charName,
      	minValue, maxValue, totalMaxValue); }
    virtual bool GetConstraints (const char* charName,
    	float& minValue, float& maxValue, float& totalMaxValue) const
    {
      return scfParent->GetConstraints (charName,
      	minValue, maxValue, totalMaxValue);
    }
    virtual void RemoveConstraints (const char* charName)
    {
      scfParent->RemoveConstraints (charName);
    }
    virtual void RemoveAllConstraints ()
    {
      scfParent->RemoveAllConstraints ();
    }
    virtual float GetCurrentCharacteristic (const char* charName) const
    {
      return scfParent->GetCurrentCharacteristic (charName);
    }
    virtual void MarkDirty (const char* charName)
    {
      scfParent->MarkDirty (charName);
    }
    virtual bool TestConstraints (const char* charName)
    {
      return scfParent->TestConstraints (charName);
    }
    virtual void Dump ()
    {
      scfParent->Dump ();
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
  iObjectRegistry* object_reg;
  csVector chars;

  struct charact
  {
    char* name;
    float value;
    float factor;	// For inherited properties
    float add;		// For inherited properties

    charact () : name (NULL), value (0), factor (0), add (0) { }
  };
  
  charact* FindCharact (const char* name) const;

  csVector inventories;

public:
  celPcCharacteristics (iObjectRegistry* object_reg);
  virtual ~celPcCharacteristics ();

  bool SetCharacteristic (const char* name, float value);
  bool SetInheritedCharacteristic (const char* name, float factor, float add);
  float GetCharacteristic (const char* name) const;
  float GetLocalCharacteristic (const char* name) const;
  float GetInheritedCharacteristic (const char* name) const;
  bool ClearCharacteristic (const char* name);
  bool HasCharacteristic (const char* name) const;
  bool ClearAll ();
  void AddToInventory (iPcInventory* inv);
  void RemoveFromInventory (iPcInventory* inv);
  void MarkDirty (const char* charName);
  bool TestConstraints (const char* charName);
  void Dump ();

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "pccharacteristics"; }
  virtual iCelEntity* GetEntity () { return entity; }
  virtual void SetEntity (iCelEntity* entity);
  virtual iCelDataBuffer* GetDataBuffer ();
  virtual void Save (iCelDataBuffer* databuf);
  virtual void Load (iCelDataBuffer* databuf);

  struct PcCharacteristics : public iPcCharacteristics
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcCharacteristics);
    virtual bool SetCharacteristic (const char* name, float value)
    {
      return scfParent->SetCharacteristic (name, value);
    }
    virtual bool SetInheritedCharacteristic (const char* name,
		  float factor, float add)
    {
      return scfParent->SetInheritedCharacteristic (name, factor, add);
    }
    virtual float GetCharacteristic (const char* name) const
    {
      return scfParent->GetCharacteristic (name);
    }
    virtual float GetLocalCharacteristic (const char* name) const
    {
      return scfParent->GetLocalCharacteristic (name);
    }
    virtual float GetInheritedCharacteristic (const char* name) const
    {
      return scfParent->GetInheritedCharacteristic (name);
    }
    virtual bool ClearCharacteristic (const char* name)
    {
      return scfParent->ClearCharacteristic (name);
    }
    virtual bool HasCharacteristic (const char* name) const
    {
      return scfParent->HasCharacteristic (name);
    }
    virtual bool ClearAll ()
    {
      return scfParent->ClearAll ();
    }
    virtual void AddToInventory (iPcInventory* inv)
    {
      scfParent->AddToInventory (inv);
    }
    virtual void RemoveFromInventory (iPcInventory* inv)
    {
      scfParent->RemoveFromInventory (inv);
    }
    virtual void MarkDirty (const char* charName)
    {
      scfParent->MarkDirty (charName);
    }
    virtual bool TestConstraints (const char* charName)
    {
      return scfParent->TestConstraints (charName);
    }
    virtual void Dump ()
    {
      scfParent->Dump ();
    }
  } scfiPcCharacteristics;
};

#endif // __CEL_PF_INVFACT__

