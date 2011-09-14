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
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef __CEL_PF_INVFACT__
#define __CEL_PF_INVFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/refarr.h"
#include "csutil/array.h"
#include "csutil/parray.h"
#include "csutil/scf.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "physicallayer/entitytpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/inv.h"
#include "propclass/chars.h"

struct iCelEntity;
struct iCelEntityTemplate;
struct iObjectRegistry;

/**
 * Factory for inventory.
 */
CEL_DECLARE_FACTORY (Inventory)
CEL_DECLARE_FACTORY (Characteristics)   

struct TemplateStack
{
  csRef<iCelEntityTemplate> tpl;
  int amount;
};

/**
 * This is an inventory property class.
 */
class celPcInventory : public scfImplementationExt1<
	celPcInventory, celPcCommon, iPcInventory>
{
private:
  csRefArray<iCelEntity> contents;
  csArray<TemplateStack> templatedContents;
  struct constraint
  {
    csString charName;
    float minValue;
    float maxValue;
    float totalMaxValue;
    float currentValue;
    bool strict;
    bool dirty;
  };
  csPDelArray<constraint> constraints;
  csRefArray<iPcInventoryListener> listeners;

  csRef<iMessageDispatcher> dispatcher_add;
  csRef<iMessageDispatcher> dispatcher_remove;

  constraint* FindConstraint (const char* name) const;
  constraint* NewConstraint (const char* name);
  bool TestLocalConstraints (const char* charName);

  static csStringID id_entity;
  static csStringID id_amount;
  celVariableParameterBlock* params;

  csRef<iCelInventorySpace> space;

  csSet<csStringID> allowedClasses;

  // For actions.
  enum actionids
  {
    action_addtemplate = 0,
    action_removetemplate
  };
  static csStringID id_name;

  static PropertyHolder propinfo;

public:
  celPcInventory (iObjectRegistry* object_reg);
  virtual ~celPcInventory ();

  virtual bool AddEntity (iCelEntity* entity, iCelParameterBlock* params = 0);
  virtual bool RemoveEntity (iCelEntity* entity);
  virtual bool RemoveEntity (iCelParameterBlock* params);
  virtual bool AddEntityTemplate (iCelEntityTemplate* tpl, int count);
  virtual bool RemoveEntityTemplate (iCelEntityTemplate* tpl, int count);
  virtual bool RemoveAll ();

  virtual size_t GetEntityCount () const { return contents.GetSize () ; }
  virtual iCelEntity* GetEntity (size_t idx) const;
  virtual bool In (iCelEntity* entity) const;
  virtual size_t FindEntity (iCelEntity* entity) const;
  virtual size_t FindEntity (csStringID classid) const;
  virtual bool In (const char* name) const;
  virtual size_t FindEntity (const char* name) const;

  virtual size_t GetEntityTemplateCount () const;
  virtual iCelEntityTemplate* GetEntityTemplate (size_t idx) const;
  virtual int GetEntityTemplateAmount (size_t idx) const;
  virtual bool In (iCelEntityTemplate* tpl) const;
  virtual size_t FindEntityTemplate (iCelEntityTemplate* tpl) const;
  virtual size_t FindEntityTemplate (const char* name) const;
  virtual size_t FindEntityTemplate (csStringID classid) const;

  virtual iCelEntity* GetEntitySlot (iCelParameterBlock* params) const;
  virtual bool SetStrictCharacteristics (const char* charName, bool strict);
  virtual bool HasStrictCharacteristics (const char* charName) const;
  virtual bool SetConstraints (const char* charName, float minValue,
  	float maxValue, float totalMaxValue);
  virtual bool GetConstraints (const char* charName, float& minValue,
  	float& maxValue, float& totalMaxValue) const;
  virtual void RemoveConstraints (const char* charName);
  virtual void RemoveAllConstraints ();
  virtual float GetCurrentCharacteristic (const char* charName) const;

  virtual void SetSpace (iCelInventorySpace* space);
  virtual iCelInventorySpace* GetSpace ();

  virtual void MarkDirty (const char* charName);
  virtual bool TestConstraints (const char* charName);
  virtual void Dump ();

  virtual void AddInventoryListener (iPcInventoryListener* listener);
  virtual void RemoveInventoryListener (iPcInventoryListener* listener);
  void FireInventoryListenersAdd (iCelEntity* entity);
  void FireInventoryListenersAdd (iCelEntityTemplate* tpl, int amount);
  void FireInventoryListenersRemove (iCelEntity* entity);
  void FireInventoryListenersRemove (iCelEntityTemplate* tpl, int amount);

  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformActionIndexed (int idx, iCelParameterBlock* params,
      celData& ret);

  virtual void AddAllowedClass (csStringID cls);
  virtual void ClearAllowedClasses ();
  virtual bool IsClassAllowed (csStringID cls) const;
};

/**
 * This is a characteristics property class.
 */
class celPcCharacteristics : public scfImplementationExt1<
	celPcCharacteristics, celPcCommon, iPcCharacteristics>
{
private:
  struct charact
  {
    char* name;
    float value;
    float factor;	// For inherited properties
    float add;		// For inherited properties

    charact () : name (0), value (0), factor (0), add (0) { }
    ~charact () { delete[] name; }
  };
  csPDelArray<charact> chars;
  
  charact* FindCharact (const char* name) const;

  csArray<iPcInventory*> inventories;

  // For actions.
  enum actionids
  {
    action_hascharacteristic = 0
  };
  static csStringID id_name;

  static PropertyHolder propinfo;

public:
  celPcCharacteristics (iObjectRegistry* object_reg);
  virtual ~celPcCharacteristics ();

  virtual bool SetCharacteristic (const char* name, float value);
  virtual bool SetInheritedCharacteristic (const char* name, float factor,
  	float add);
  virtual float GetCharacteristic (const char* name) const;
  virtual float GetLocalCharacteristic (const char* name) const;
  virtual float GetInheritedCharacteristic (const char* name) const;
  virtual bool ClearCharacteristic (const char* name);
  virtual bool HasCharacteristic (const char* name) const;
  virtual bool ClearAll ();
  virtual void AddToInventory (iPcInventory* inv);
  virtual void RemoveFromInventory (iPcInventory* inv);
  virtual void MarkDirty (const char* charName);
  virtual bool TestConstraints (const char* charName);
  virtual void Dump ();

  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformActionIndexed (int idx, iCelParameterBlock* params,
      celData& ret);

  virtual bool SetProperty (csStringID, float);
  virtual float GetPropertyFloat (csStringID);
};

#endif // __CEL_PF_INVFACT__

