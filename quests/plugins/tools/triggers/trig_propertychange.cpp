/*
    Crystal Space Entity Layer
    Copyright (C) 2009 by Jorrit Tyberghein
	Copyright (C) 2009 by Sam Devlin

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
#include "csutil/objreg.h"
#include "csutil/dirtyaccessarray.h"
#include "csutil/util.h"
#include "csutil/scanstr.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/document.h"
#include "iutil/plugin.h"
#include "ivaria/reporter.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"

#include "plugins/tools/triggers/trig_propertychange.h"

//---------------------------------------------------------------------------
SCF_IMPLEMENT_FACTORY (celPropertyChangeTriggerType)
CEL_IMPLEMENT_TRIGGERTYPE(PropertyChange)

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR,
    	"cel.triggers.propertychange", msg, arg);
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

celPropertyChangeTriggerFactory::celPropertyChangeTriggerFactory (
	celPropertyChangeTriggerType* type) : scfImplementationType (this)
{
  celPropertyChangeTriggerFactory::type = type;
  value_par = 0;
}

celPropertyChangeTriggerFactory::~celPropertyChangeTriggerFactory ()
{
  delete[] value_par;
}

csPtr<iTrigger> celPropertyChangeTriggerFactory::CreateTrigger (
    const celParams& params)
{
  celPropertyChangeTrigger* trig = 0;
  if (!op_par)
    trig = new celPropertyChangeTrigger (type,
      	 params, entity_par, tag_par, prop_par, value_par, onchange_par);
  else if (!strcmp(op_par,"eq"))
    trig = new celPropertyChangeTrigger (type,
      	 params, entity_par, tag_par, prop_par, value_par, onchange_par);
  else if (!strcmp(op_par,"ne"))
    trig = new celPropertyChangeTriggerNe (type,
      	 params, entity_par, tag_par, prop_par, value_par, onchange_par);
  else if (!strcmp(op_par,"le"))
    trig = new celPropertyChangeTriggerLe (type,
      	 params, entity_par, tag_par, prop_par, value_par, onchange_par);
  else if (!strcmp(op_par,"ge"))
    trig = new celPropertyChangeTriggerGe (type,
      	 params, entity_par, tag_par, prop_par, value_par, onchange_par);
  else if (!strcmp(op_par,"gt"))
    trig = new celPropertyChangeTriggerGt (type,
      	 params, entity_par, tag_par, prop_par, value_par, onchange_par);
  else if (!strcmp(op_par,"lt"))
    trig = new celPropertyChangeTriggerLt (type,
      	 params, entity_par, tag_par, prop_par, value_par, onchange_par);
  return trig;
}

bool celPropertyChangeTriggerFactory::Load (iDocumentNode* node)
{
  entity_par = node->GetAttributeValue ("entity");
  tag_par = node->GetAttributeValue ("entity_tag");
  if (entity_par.IsEmpty())
    return Report (type->object_reg,
      "'entity' attribute is missing for the propertychange trigger!");
  prop_par = node->GetAttributeValue ("property");
  if (prop_par.IsEmpty())
    return Report (type->object_reg,
      "'property' attribute is missing for the propertychange trigger!");
  value_par = csStrNew (node->GetAttributeValue ("value"));
  op_par = node->GetAttributeValue ("operation");
  onchange_par = node->GetAttributeValueAsBool ("onchange",false);
  return true;
}

void celPropertyChangeTriggerFactory::SetEntityParameter (
	const char* entity, const char* tag)
{
  entity_par = entity;
  tag_par = tag;
}

void celPropertyChangeTriggerFactory::SetPropertyParameter (
	const char* prop)
{
  prop_par = prop;
}

void celPropertyChangeTriggerFactory::SetValueParameter (
	const char* value)
{
  if (value_par == value)
    return;
  delete[] value_par;
  value_par = csStrNew (value);
}

void celPropertyChangeTriggerFactory::SetOperationParameter (

	const char* op)
{
  op_par = op;
}

//---------------------------------------------------------------------------

celPropertyChangeTrigger::celPropertyChangeTrigger (
	celPropertyChangeTriggerType* type,
  	const celParams& params,
	const char* entity_par, const char* tag_par,
	const char* prop_par, const char* value_par, bool onchange)
	: scfImplementationType (this)
{
  celPropertyChangeTrigger::type = type;

  csRef<iPluginManager> plugin_mgr = 
    csQueryRegistry<iPluginManager> (type->object_reg);
  csRef<iParameterManager> pm = csLoadPlugin<iParameterManager> 
    (plugin_mgr, "cel.parameters.manager");

  entity = pm->ResolveParameter (params, entity_par);
  tag = pm->ResolveParameter (params, tag_par);
  prop = pm->ResolveParameter (params, prop_par);
  if (value_par)
    value = csStrNew (pm->ResolveParameter (params, value_par));
  else
    value = 0;
  on_change = onchange;
}

celPropertyChangeTrigger::~celPropertyChangeTrigger ()
{
  DeactivateTrigger ();
  delete[] value;
}

void celPropertyChangeTrigger::RegisterCallback (iTriggerCallback* callback)
{
  celPropertyChangeTrigger::callback = callback;
}

void celPropertyChangeTrigger::ClearCallback ()
{
  callback = 0;
}

bool celPropertyChangeTrigger::TestProperty (size_t idx)
{
  if (!value) return true;
  celDataType type = properties->GetPropertyType (idx);
  switch (type)
  {
    case CEL_DATA_LONG:
      {
        long v;
	sscanf (value, "%ld", &v);
	return DoTest(v,properties->GetPropertyLong (idx));
      }
      break;
    case CEL_DATA_FLOAT:
      {
        float v;
	sscanf (value, "%g", &v);
	return DoTest(v,properties->GetPropertyFloat (idx));
      }
      break;
    case CEL_DATA_BOOL:
      {
        bool v;
	csScanStr (value, "%b", &v);
	return DoTest(v,properties->GetPropertyBool (idx));
      }
      break;
    case CEL_DATA_STRING:
      {
	return DoTest(value,properties->GetPropertyString (idx));
      }
      break;
    default:
      break;
  }
  return false;
}

void celPropertyChangeTrigger::PropertyChanged (iPcProperties*,
	size_t idx)
{
  const char* pn = properties->GetPropertyName (idx);
  if (strcmp (pn, prop) != 0) return;
  // value change test
  if (on_change)
  {
    bool proptest = TestProperty (idx);
    // if both proptest and is_true have the same value there has been no change.
    if (is_true != proptest) // change detected
    {
      is_true = proptest;
      on_condition = proptest;
      if (on_condition)
        callback->TriggerFired ((iTrigger*)this, 0);
    }
  }
  // value met test
  else
  {
    is_true = TestProperty (idx);
    if (is_true)
      callback->TriggerFired ((iTrigger*)this, 0);
  }
}

void celPropertyChangeTrigger::FindProperties ()
{
  if (properties) return;
  iCelPlLayer* pl = type->pl;
  iCelEntity* ent = pl->FindEntity (entity);
  if (!ent) return;
  properties = CEL_QUERY_PROPCLASS_TAG_ENT (ent, iPcProperties, tag);
}

void celPropertyChangeTrigger::ActivateTrigger ()
{
  FindProperties ();
  if (!properties)
  {
    Report (type->object_reg,
    	"Can't find pcproperties for propertychange trigger!");
    return;
  }
  // Check if the property already has the value. We want to trigger only
  // when there is a property change, not when the property already had the
  // value.
  is_true = CheckValue();

  // Condition is always initially false, as we want a property change, so wont become
  // true until the value changes from different value to the listened one.
  // (only used if on_change is true).
  on_condition = false;

  // First remove to make sure we don't register ourselves multiple
  // times.
  properties->RemovePropertyListener ((iPcPropertyListener*)this);
  properties->AddPropertyListener ((iPcPropertyListener*)this);
}

bool celPropertyChangeTrigger::CheckValue ()
{
  if (!properties) return false;
  size_t idx = properties->GetPropertyIndex (prop);
  if (idx == csArrayItemNotFound) return false;
  // Check() returns false in case there is no value.
  if (!value) return false;
  return TestProperty (idx);
}

bool celPropertyChangeTrigger::Check ()
{
  bool rc;
  if (on_change)
    rc = on_condition;
  else
    rc = is_true;
  if (rc)
  {
    DeactivateTrigger ();
    callback->TriggerFired ((iTrigger*)this, 0);
  }
  return rc;
}

void celPropertyChangeTrigger::DeactivateTrigger ()
{
  if (!properties) return;
  properties->RemovePropertyListener ((iPcPropertyListener*)this);
}

bool celPropertyChangeTrigger::LoadAndActivateTrigger (iCelDataBuffer*)
{
  ActivateTrigger ();
  return true;
}

void celPropertyChangeTrigger::SaveTriggerState (iCelDataBuffer*)
{
}

//---------------------------------------------------------------------------

