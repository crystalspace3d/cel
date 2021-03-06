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

typedef csHash<csRef<celActiveRulesForVariable>, csStringBase>
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
class celPcRules : public scfImplementationExt1<
	celPcRules, celPcCommon, iPcRules>
{
private:
  // For SendMessage parameters.
  celOneParameterBlock* params;

  csRef<iMessageDispatcher> dispatcher_modifypar;

  // For actions.
  static csStringID id_name;
  static csStringID id_time;

  enum actionids
  {
    action_addrule = 0,
    action_deleterule,
    action_deleteallrules
  };

  static PropertyHolder propinfo;

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

  void SendModifyPar (const char* rulevar);

public:
  celPcRules (iObjectRegistry* object_reg);
  virtual ~celPcRules ();

  void PropertyChanged (iPcProperties* pcprop, size_t idx);

  virtual void AddRule (iCelRule* rule);
  virtual void AddRule (iCelRule* rule, csTicks time);
  virtual void DeleteRule (iCelRule* rule);
  void DeleteRule (celActiveRule* rule);
  virtual void DeleteAllRules ();
  virtual celDataType GetPropertyType (const char* name);
  virtual bool GetProperty (const char* name, celData& ret);
  virtual float GetPropertyFloat (const char* name);
  virtual long GetPropertyLong (const char* name);
  virtual bool GetPropertyBool (const char* name);
  virtual bool GetPropertyVector (const char* name, csVector2& v);
  virtual bool GetPropertyVector (const char* name, csVector3& v);
  virtual bool GetPropertyColor (const char* name, csColor& v);
  virtual const char* GetPropertyString (const char* name);

  virtual bool PerformActionIndexed (int idx, iCelParameterBlock* params,
      celData& ret);
  virtual void TickEveryFrame ();
};

#endif // __CEL_PF_RULESFACT__

