/*
    Crystal Space Entity Layer
    Copyright (C) 2006 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_RULESFACT__
#define __CEL_PF_RULESFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "csutil/csstring.h"
#include "csutil/parray.h"
#include "csutil/refarr.h"
#include "csutil/refcount.h"
#include "iutil/virtclk.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/rules.h"
#include "propclass/prop.h"
#include "tools/rulebase.h"

struct iCelEntity;
struct iObjectRegistry;
class celPcRules;

/**
 * Factory for rules.
 */
CEL_DECLARE_FACTORY (Rules)

class celActiveRule : public csRefCount
{
public:
  csRef<iCelRule> rule;

public:
  celActiveRule () { }
};

class celActiveRulesForVariable : public csRefCount
{
public:
  // This array is sorted on priority.
  csRefArray<celActiveRule> active_rules;

public:
  celActiveRulesForVariable () { }
};

typedef csHash<csRef<celActiveRulesForVariable>, csStrKey>
	celActiveRulesForVariableHash;

class rulePropertyListener : public scfImplementation1<rulePropertyListener,
	iPcPropertyListener>
{
private:
  csWeakRef<celPcRules> pcrules;

public:
  rulePropertyListener (celPcRules* pcrules)
  	: scfImplementationType (this), pcrules (pcrules) { }
  virtual ~rulePropertyListener () { }

  virtual void PropertyChanged (iPcProperties* pcprop, size_t idx);
};

struct celTimedRule
{
  csTicks remove_time;
  csRef<celActiveRule> rule;
};

/**
 * This is the rules property class.
 */
class celPcRules : public celPcCommon
{
private:
  // For SendMessage parameters.
  celOneParameterBlock* params;

  // For PerformAction.
  static csStringID action_addrule;
  static csStringID action_deleterule;
  static csStringID action_deleteallrules;
  static csStringID id_name;
  static csStringID id_time;

  // Other fields.
  csRef<iCelRuleBase> rulebase;
  void GetRuleBase ();
  csWeakRef<iPcProperties> pcprop;
  void GetProperties ();
  csRef<rulePropertyListener> prop_listener;

  csRef<iVirtualClock> vc;

  // Active rules.
  celActiveRulesForVariableHash active_rules_for_variable;

  // Rules that time out. This array is sorted on ticks.
  csArray<celTimedRule> timed_rules;

public:
  celPcRules (iObjectRegistry* object_reg);
  virtual ~celPcRules ();

  void PropertyChanged (iPcProperties* pcprop, size_t idx);

  void AddRule (iCelRule* rule);
  void AddRule (iCelRule* rule, csTicks time);
  void DeleteRule (iCelRule* rule);
  void DeleteRule (celActiveRule* rule);
  void DeleteAllRules ();
  celDataType GetPropertyType (const char* name);
  bool GetProperty (const char* name, celData& ret);
  float GetPropertyFloat (const char* name);
  long GetPropertyLong (const char* name);
  bool GetPropertyBool (const char* name);
  bool GetPropertyVector (const char* name, csVector2& v);
  bool GetPropertyVector (const char* name, csVector3& v);
  bool GetPropertyColor (const char* name, csColor& v);
  const char* GetPropertyString (const char* name);

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcrules"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params);
  virtual void TickEveryFrame ();

  struct PcRules : public iPcRules
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcRules);
    virtual void AddRule (iCelRule* rule)
    {
      scfParent->AddRule (rule);
    }
    virtual void AddRule (iCelRule* rule, csTicks time)
    {
      scfParent->AddRule (rule, time);
    }
    virtual void DeleteRule (iCelRule* rule)
    {
      scfParent->DeleteRule (rule);
    }
    virtual void DeleteAllRules ()
    {
      scfParent->DeleteAllRules ();
    }
    virtual celDataType GetPropertyType (const char* name)
    {
      return scfParent->GetPropertyType (name);
    }
    virtual bool GetProperty (const char* name, celData& ret)
    {
      return scfParent->GetProperty (name, ret);
    }
    virtual float GetPropertyFloat (const char* name)
    {
      return scfParent->GetPropertyFloat (name);
    }
    virtual long GetPropertyLong (const char* name)
    {
      return scfParent->GetPropertyLong (name);
    }
    virtual bool GetPropertyBool (const char* name)
    {
      return scfParent->GetPropertyBool (name);
    }
    virtual bool GetPropertyVector (const char* name, csVector2& v)
    {
      return scfParent->GetPropertyVector (name, v);
    }
    virtual bool GetPropertyVector (const char* name, csVector3& v)
    {
      return scfParent->GetPropertyVector (name, v);
    }
    virtual bool GetPropertyColor (const char* name, csColor& v)
    {
      return scfParent->GetPropertyColor (name, v);
    }
    virtual const char* GetPropertyString (const char* name)
    {
      return scfParent->GetPropertyString (name);
    }
  } scfiPcRules;
};

#endif // __CEL_PF_RULESFACT__

