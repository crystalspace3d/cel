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

#include "cssysdef.h"
#include "iutil/objreg.h"
#include "iutil/plugin.h"
#include "cstool/initapp.h"
#include "ivaria/reporter.h"
#include "plugins/propclass/rules/rulesfact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "tools/expression.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY_ALT (Rules, "pclogic.rules", "pcrules")

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.propclass.rules",
    	msg, arg);
  else
  {
    csPrintfV (msg, arg);
    csPrintf ("\n");
    fflush (stdout);
  }

  va_end (arg);
  return false;
}

//---------------------------------------------------------------------------

void rulePropertyListener::PropertyChanged (iPcProperties* pcprop, size_t idx)
{
  if (pcrules)
  {
    pcrules->PropertyChanged (pcprop, idx);
  }
}

//---------------------------------------------------------------------------

csStringID celPcRules::id_name = csInvalidStringID;
csStringID celPcRules::id_time = csInvalidStringID;
PropertyHolder celPcRules::propinfo;

SCF_IMPLEMENT_IBASE_EXT (celPcRules)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcRules)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcRules::PcRules)
  SCF_IMPLEMENTS_INTERFACE (iPcRules)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcRules::celPcRules (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcRules);

  // For actions.
  if (id_name == csInvalidStringID)
  {
    id_name = pl->FetchStringID ("cel.parameter.name");
    id_time = pl->FetchStringID ("cel.parameter.time");
  }

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    AddAction (action_addrule, "cel.action.AddRule");
    AddAction (action_deleterule, "cel.action.DeleteRule");
    AddAction (action_deleteallrules, "cel.action.DeleteAllRules");
  }

  // For SendMessage parameters.
  params = new celOneParameterBlock ();
  params->SetParameterDef (id_name, "name");

  vc = csQueryRegistry<iVirtualClock> (object_reg);

  GetRuleBase ();
}

celPcRules::~celPcRules ()
{
  if (pcprop && prop_listener)
    pcprop->RemovePropertyListener (prop_listener);
  delete params;
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcRules);
}

#define RULES_SERIAL 1

csPtr<iCelDataBuffer> celPcRules::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (RULES_SERIAL);
  // @@@ TODO
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcRules::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != RULES_SERIAL)
    return Report (object_reg, "Couldn't load pcrules!");
  // @@@ TODO
  return true;
}

bool celPcRules::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  GetRuleBase ();
  if (!rulebase) return false;
  switch (idx)
  {
    case action_addrule:
      {
        CEL_FETCH_STRING_PAR (name,params,id_name);
        if (!p_name)
          return Report (object_reg,
      	    "Missing parameter 'name' for action AddRule!");
        iCelRule* rule = rulebase->FindRule (name);
        if (!rule)
          return Report (object_reg, "Can't find rule '%s'!", name);
        CEL_FETCH_LONG_PAR (time,params,id_time);
        if (p_time)
          AddRule (rule, time);
        else
          AddRule (rule);
        return true;
      }
    case action_deleterule:
      {
        CEL_FETCH_STRING_PAR (name,params,id_name);
        if (!p_name)
          return Report (object_reg,
      	    "Missing parameter 'name' for action AddRule!");
        iCelRule* rule = rulebase->FindRule (name);
        if (!rule)
          return Report (object_reg, "Can't find rule '%s'!", name);
        DeleteRule (rule);
        return true;
      }
    case action_deleteallrules:
      DeleteAllRules ();
      return true;
    default:
      return false;
  }
}

void celPcRules::GetRuleBase ()
{
  if (!rulebase)
  {
    rulebase = csQueryRegistryOrLoad<iCelRuleBase> (object_reg,
    	"cel.manager.rules");
    if (!rulebase)
    {
      Report (object_reg, "Can't find rule base plugin!");
      return;
    }
  }
}

static int CompareTime (celTimedRule const& r1,
	celTimedRule const& r2)
{
  // Reverse sort!
  size_t p1 = r1.remove_time;
  size_t p2 = r2.remove_time;
  if (p1 < p2) return -1;
  else if (p1 > p2) return 1;
  else return 0;
}

static int ComparePriorityRule (celActiveRule* const& r1,
	celActiveRule* const& r2)
{
  // Reverse sort!
  size_t p1 = r1->rule->GetPriorityNumber ();
  size_t p2 = r2->rule->GetPriorityNumber ();
  if (p1 < p2) return -1;
  else if (p1 > p2) return 1;
  else return 0;
}

void celPcRules::AddRule (iCelRule* rule)
{
  celActiveRule* active_rule = new celActiveRule ();
  active_rule->rule = rule;
  celActiveRulesForVariable* av = active_rules_for_variable.Get (
  	rule->GetVariable (), 0);
  if (!av)
  {
    av = new celActiveRulesForVariable ();
    active_rules_for_variable.Put (rule->GetVariable (), av);
    av->DecRef ();
  }
  av->active_rules.InsertSorted (active_rule, ComparePriorityRule);
  active_rule->DecRef ();

  iCelBehaviour* ble = entity->GetBehaviour ();
  if (ble)
  {
    celData ret;
    params->GetParameter (0).Set (rule->GetVariable ());
    ble->SendMessage ("pcrules_modifypar", this, ret, params);
  }
}

void celPcRules::TickEveryFrame ()
{
  if (timed_rules.GetSize () == 0) return;
  csTicks current = vc->GetCurrentTicks ();
  do
  {
    if (timed_rules[0].remove_time > current) return;
    DeleteRule (timed_rules[0].rule);
    timed_rules.DeleteIndex (0);
  }
  while (timed_rules.GetSize () > 0);
}

void celPcRules::AddRule (iCelRule* rule, csTicks time)
{
  pl->RemoveCallbackEveryFrame (this, CEL_EVENT_PRE);
  pl->CallbackEveryFrame (this, CEL_EVENT_PRE);

  celActiveRule* active_rule = new celActiveRule ();
  active_rule->rule = rule;
  celActiveRulesForVariable* av = active_rules_for_variable.Get (
  	rule->GetVariable (), 0);
  if (!av)
  {
    av = new celActiveRulesForVariable ();
    active_rules_for_variable.Put (rule->GetVariable (), av);
    av->DecRef ();
  }
  av->active_rules.InsertSorted (active_rule, ComparePriorityRule);
  celTimedRule timed_rule;
  timed_rule.remove_time = vc->GetCurrentTicks () + time;
  timed_rule.rule = active_rule;
  timed_rules.InsertSorted (timed_rule, CompareTime);
  active_rule->DecRef ();

  iCelBehaviour* ble = entity->GetBehaviour ();
  if (ble)
  {
    celData ret;
    params->GetParameter (0).Set (rule->GetVariable ());
    ble->SendMessage ("pcrules_modifypar", this, ret, params);
  }
}

void celPcRules::DeleteRule (celActiveRule* rule)
{
  celActiveRulesForVariable* av = active_rules_for_variable.Get (
  	rule->rule->GetVariable (), 0);
  if (av)
  {
    size_t i = av->active_rules.GetSize ();
    while (i > 0)
    {
      i--;
      if (av->active_rules[i] == rule)
      {
        av->active_rules.DeleteIndex (i);
	break;
      }
    }

    iCelBehaviour* ble = entity->GetBehaviour ();
    if (ble)
    {
      celData ret;
      params->GetParameter (0).Set (rule->rule->GetVariable ());
      ble->SendMessage ("pcrules_modifypar", this, ret, params);
    }
  }
}

void celPcRules::DeleteRule (iCelRule* rule)
{
  celActiveRulesForVariable* av = active_rules_for_variable.Get (
  	rule->GetVariable (), 0);
  if (av)
  {
    size_t i = av->active_rules.GetSize ();
    while (i > 0)
    {
      i--;
      if (av->active_rules[i]->rule == rule)
      {
        av->active_rules.DeleteIndex (i);
      }
    }

    iCelBehaviour* ble = entity->GetBehaviour ();
    if (ble)
    {
      celData ret;
      params->GetParameter (0).Set (rule->GetVariable ());
      ble->SendMessage ("pcrules_modifypar", this, ret, params);
    }
  }
}

void celPcRules::DeleteAllRules ()
{
  csStringArray vars;
  celActiveRulesForVariableHash::GlobalIterator it =
  	active_rules_for_variable.GetIterator ();
  while (it.HasNext ())
  {
    csStringBase var;
    it.Next (var);
    vars.Push (var);
  }

  active_rules_for_variable.DeleteAll ();

  iCelBehaviour* ble = entity->GetBehaviour ();
  if (ble)
  {
    celData ret;
    for (size_t i = 0 ; i < vars.GetSize () ; i++)
    {
      params->GetParameter (0).Set (vars[i]);
      ble->SendMessage ("pcrules_modifypar", this, ret, params);
    }
  }
}

void celPcRules::PropertyChanged (iPcProperties* pcprop, size_t idx)
{
  iCelBehaviour* ble = entity->GetBehaviour ();
  if (ble)
  {
    celData ret;
    const char* var = pcprop->GetPropertyName (idx);
    celActiveRulesForVariable* av = active_rules_for_variable.Get (var, 0);
    if (av)
    {
      params->GetParameter (0).Set (var);
      ble->SendMessage ("pcrules_modifypar", this, ret, params);
    }
  }
}

void celPcRules::GetProperties ()
{
  if (pcprop) return;
  pcprop = CEL_QUERY_PROPCLASS_ENT (entity, iPcProperties);
  if (pcprop)
  {
    prop_listener.AttachNew (new rulePropertyListener (this));
    pcprop->AddPropertyListener (prop_listener);
  }
  else
  {
    prop_listener = 0;
  }
}

celDataType celPcRules::GetPropertyType (const char* name)
{
  GetProperties ();
  if (pcprop)
  {
    size_t idx = pcprop->GetPropertyIndex (name);
    if (idx == csArrayItemNotFound) return CEL_DATA_NONE;
    return pcprop->GetPropertyType (idx);
  }
  return CEL_DATA_NONE;
}

bool celPcRules::GetProperty (const char* name, celData& ret)
{
  ret.Clear ();
  GetProperties ();
  if (pcprop)
  {
    size_t idx = pcprop->GetPropertyIndex (name);
    if (idx != csArrayItemNotFound)
    {
      celDataType type = pcprop->GetPropertyType (idx);
      switch (type)
      {
        case CEL_DATA_FLOAT: ret.Set (pcprop->GetPropertyFloat (idx)); break;
        case CEL_DATA_LONG:
	  ret.Set ((int32)pcprop->GetPropertyLong (idx)); break;
        case CEL_DATA_BOOL: ret.Set (pcprop->GetPropertyBool (idx)); break;
        case CEL_DATA_STRING: ret.Set (pcprop->GetPropertyString (idx)); break;
        case CEL_DATA_VECTOR2:
	  {
	    csVector2 v;
	    pcprop->GetPropertyVector (idx, v);
	    ret.Set (v);
          }
	  break;
        case CEL_DATA_VECTOR3:
	  {
	    csVector3 v;
	    pcprop->GetPropertyVector (idx, v);
	    ret.Set (v);
          }
	  break;
        case CEL_DATA_COLOR:
	  {
	    csColor v;
	    pcprop->GetPropertyColor (idx, v);
	    ret.Set (v);
          }
	  break;
        default:
	  break;
      }
    }
  }
  celActiveRulesForVariable* av = active_rules_for_variable.Get (name, 0);
  if (av)
  {
    size_t i;
    // @@@ Need to sort rules on priority!
    for (i = 0 ; i < av->active_rules.GetSize () ; i++)
    {
      celActiveRule* ar = av->active_rules[i];
      iCelExpression* expr = ar->rule->GetExpression ();
      size_t idx = ar->rule->GetVariableIndex ();
      if (idx != csArrayItemNotFound)
        expr->SetLocalVariable (idx, ret);
      expr->Execute (entity, ret);
    }
  }
  return true;
}

float celPcRules::GetPropertyFloat (const char* name)
{
  celData ret;
  if (!GetProperty (name, ret)) return 0.0f;
  switch (ret.type)
  {
    case CEL_DATA_FLOAT: return ret.value.f;
    case CEL_DATA_BOOL: return ret.value.bo ? 1.0f : 0.0f;
    case CEL_DATA_LONG: return float (ret.value.l);
    case CEL_DATA_ULONG: return float (ret.value.ul);
    case CEL_DATA_STRING:
    {
      float val;
      sscanf (ret.value.s->GetData (), "%f", &val);
      return val;
    }
    default: return 0.0f;
  }
}

long celPcRules::GetPropertyLong (const char* name)
{
  celData ret;
  if (!GetProperty (name, ret)) return 0;
  switch (ret.type)
  {
    case CEL_DATA_FLOAT: return (long)ret.value.f;
    case CEL_DATA_BOOL: return ret.value.bo ? 1 : 0;
    case CEL_DATA_LONG: return long (ret.value.l);
    case CEL_DATA_ULONG: return long (ret.value.ul);
    case CEL_DATA_STRING:
    {
      long val;
      sscanf (ret.value.s->GetData (), "%ld", &val);
      return val;
    }
    default: return 0;
  }
}

bool celPcRules::GetPropertyBool (const char* name)
{
  celData ret;
  if (!GetProperty (name, ret)) return false;
  switch (ret.type)
  {
    case CEL_DATA_FLOAT: return fabs (ret.value.f) > 0.00001;
    case CEL_DATA_BOOL: return ret.value.bo;
    case CEL_DATA_LONG: return (ret.value.l ? true : false);
    case CEL_DATA_ULONG: return (ret.value.ul ? true : false);
    case CEL_DATA_STRING:
    {
      long val;
      sscanf (ret.value.s->GetData (), "%ld", &val);
      return (val ? true : false);
    }
    default: return false;
  }
}

bool celPcRules::GetPropertyVector (const char* name, csVector2& v)
{
  celData ret;
  v.Set (0, 0);
  if (!GetProperty (name, ret)) return true;
  switch (ret.type)
  {
    case CEL_DATA_VECTOR2: v.Set (ret.value.v.x, ret.value.v.y); return true;
    case CEL_DATA_STRING:
    {
      sscanf (ret.value.s->GetData (), "%f,%f", &v.x, &v.y);
      return true;
    }
    default: return true;
  }
}

bool celPcRules::GetPropertyVector (const char* name, csVector3& v)
{
  celData ret;
  v.Set (0, 0, 0);
  if (!GetProperty (name, ret)) return true;
  switch (ret.type)
  {
    case CEL_DATA_VECTOR3:
      v.Set (ret.value.v.x, ret.value.v.y, ret.value.v.z);
      return true;
    case CEL_DATA_STRING:
    {
      sscanf (ret.value.s->GetData (), "%f,%f,%f", &v.x, &v.y, &v.z);
      return true;
    }
    default: return true;
  }
}

bool celPcRules::GetPropertyColor (const char* name, csColor& v)
{
  celData ret;
  v.Set (0, 0, 0);
  if (!GetProperty (name, ret)) return true;
  switch (ret.type)
  {
    case CEL_DATA_COLOR:
      v.Set (ret.value.col.red, ret.value.col.green, ret.value.col.blue);
      return true;
    case CEL_DATA_STRING:
    {
      sscanf (ret.value.s->GetData (), "%f,%f,%f", &v.red, &v.green, &v.blue);
      return true;
    }
    default: return true;
  }
}

const char* celPcRules::GetPropertyString (const char* name)
{
  celData ret;
  if (!GetProperty (name, ret)) return "";
  switch (ret.type)
  {
    case CEL_DATA_STRING: return ret.value.s->GetData ();
    default: return "";
  }
}

//---------------------------------------------------------------------------

