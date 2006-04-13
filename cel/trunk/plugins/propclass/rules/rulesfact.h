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

/**
 * Factory for rules.
 */
CEL_DECLARE_FACTORY (Rules)

class celActiveRule
{
public:
  csRef<iCelRule> rule;

public:
  celActiveRule () { }
};

class celActiveRulesForVariable : public csRefCount
{
public:
  csPDelArray<celActiveRule> active_rules;

public:
  celActiveRulesForVariable () { }
};

typedef csHash<csRef<celActiveRulesForVariable>, csStrKey>
	celActiveRulesForVariableHash;

/**
 * This is the rules property class.
 */
class celPcRules : public celPcCommon
{
private:
  // For SendMessage parameters.
  //static csStringID id_message;
  //celOneParameterBlock* params;

  // For PerformAction.
  // id_message is shared.
  static csStringID action_addrule;
  static csStringID action_deleterule;
  static csStringID action_deleteallrules;
  static csStringID id_name;

  // Other fields.
  csRef<iCelRuleBase> rulebase;
  void GetRuleBase ();
  csWeakRef<iPcProperties> pcprop;
  void GetProperties ();

  // Active rules.
  celActiveRulesForVariableHash active_rules_for_variable;

public:
  celPcRules (iObjectRegistry* object_reg);
  virtual ~celPcRules ();

  void AddRule (iCelRule* rule);
  void DeleteRule (iCelRule* rule);
  void DeleteAllRules ();
  celDataType GetPropertyType (const char* name);
  bool GetProperty (const char* name, celData& ret);
  float GetPropertyFloat (const char* name);
  long GetPropertyLong (const char* name);
  bool GetPropertyBool (const char* name);
  bool GetPropertyVector (const char* name, const csVector2& v);
  bool GetPropertyVector (const char* name, const csVector3& v);
  bool GetPropertyColor (const char* name, const csColor& v);
  const char* GetPropertyString (const char* name);

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcrules"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params);

  struct PcRules : public iPcRules
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcRules);
    virtual void AddRule (iCelRule* rule)
    {
      scfParent->AddRule (rule);
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
    virtual bool GetPropertyVector (const char* name, const csVector2& v)
    {
      return scfParent->GetPropertyVector (name, v);
    }
    virtual bool GetPropertyVector (const char* name, const csVector3& v)
    {
      return scfParent->GetPropertyVector (name, v);
    }
    virtual bool GetPropertyColor (const char* name, const csColor& v)
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

