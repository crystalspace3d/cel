/*
    Crystal Space Entity Layer
    Copyright (C) 2004 by Jorrit Tyberghein

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
#include "ivaria/reporter.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"

#include "plugins/tools/quests/trig_propertychange.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_TRIGGERTYPE(PropertyChange)

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR,
    	"cel.quests.trigger.propertychange", msg, arg);
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
  entity_par = 0;
  tag_par = 0;
  prop_par = 0;
  value_par = 0;
}

celPropertyChangeTriggerFactory::~celPropertyChangeTriggerFactory ()
{
  delete[] entity_par;
  delete[] tag_par;
  delete[] prop_par;
  delete[] value_par;
}

csPtr<iQuestTrigger> celPropertyChangeTriggerFactory::CreateTrigger (
    iQuest*, const celQuestParams& params)
{
  celPropertyChangeTrigger* trig = new celPropertyChangeTrigger (type,
  	params, entity_par, tag_par, prop_par, value_par);
  return trig;
}

bool celPropertyChangeTriggerFactory::Load (iDocumentNode* node)
{
  delete[] entity_par; entity_par = 0;
  delete[] tag_par; tag_par = 0;
  delete[] prop_par; prop_par = 0;
  delete[] value_par; value_par = 0;

  entity_par = csStrNew (node->GetAttributeValue ("entity"));
  tag_par = csStrNew (node->GetAttributeValue ("entity_tag"));
  if (!entity_par)
    return Report (type->object_reg,
      "'entity' attribute is missing for the propertychange trigger!");
  prop_par = csStrNew (node->GetAttributeValue ("property"));
  if (!prop_par)
    return Report (type->object_reg,
      "'property' attribute is missing for the propertychange trigger!");
  value_par = csStrNew (node->GetAttributeValue ("value"));
  return true;
}

void celPropertyChangeTriggerFactory::SetEntityParameter (
	const char* entity, const char* tag)
{
  if (entity_par != entity)
  {
    delete[] entity_par;
    entity_par = csStrNew (entity);
  }
  if (tag_par != tag)
  {
    delete[] tag_par;
    tag_par = csStrNew (tag);
  }
}

void celPropertyChangeTriggerFactory::SetPropertyParameter (
	const char* prop)
{
  if (prop_par == prop) 
    return;

  delete[] prop_par;
  prop_par = csStrNew (prop);
}

void celPropertyChangeTriggerFactory::SetValueParameter (
	const char* value)
{
  if (value_par == value) 
    return;

  delete[] value_par;
  value_par = csStrNew (value);
}

//---------------------------------------------------------------------------

celPropertyChangeTrigger::celPropertyChangeTrigger (
	celPropertyChangeTriggerType* type,
  	const celQuestParams& params,
	const char* entity_par, const char* tag_par,
	const char* prop_par, const char* value_par)
	: scfImplementationType (this)
{
  celPropertyChangeTrigger::type = type;
  csRef<iQuestManager> qm = CS_QUERY_REGISTRY (type->object_reg, iQuestManager);
  entity = csStrNew (qm->ResolveParameter (params, entity_par));
  tag = csStrNew (qm->ResolveParameter (params, tag_par));
  prop = csStrNew (qm->ResolveParameter (params, prop_par));
  if (value_par)
    value = csStrNew (qm->ResolveParameter (params, value_par));
  else
    value = 0;
}

celPropertyChangeTrigger::~celPropertyChangeTrigger ()
{
  DeactivateTrigger ();
  delete[] entity;
  delete[] tag;
  delete[] prop;
  delete[] value;
}

void celPropertyChangeTrigger::RegisterCallback (iQuestTriggerCallback* callback)
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
	return (v == properties->GetPropertyLong (idx));
      }
      break;
    case CEL_DATA_FLOAT:
      {
        float v;
	sscanf (value, "%g", &v);
	return (v == properties->GetPropertyFloat (idx));
      }
      break;
    case CEL_DATA_BOOL:
      {
        bool v;
	csScanStr (value, "%b", &v);
	return (v == properties->GetPropertyBool (idx));
      }
      break;
    case CEL_DATA_STRING:
      {
	return (!strcmp (value, properties->GetPropertyString (idx)));
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
  if (TestProperty (idx))
  {
    DeactivateTrigger ();
    callback->TriggerFired ((iQuestTrigger*)this);
  }
}

void celPropertyChangeTrigger::FindProperties ()
{
  if (properties) return;
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (type->object_reg, iCelPlLayer);
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
  // First remove to make sure we don't register ourselves multiple
  // times.
  properties->RemovePropertyListener ((iPcPropertyListener*)this);
  properties->AddPropertyListener ((iPcPropertyListener*)this);
}

bool celPropertyChangeTrigger::Check ()
{
  if (!properties) return false;
  size_t idx = properties->GetPropertyIndex (prop);
  if (idx == csArrayItemNotFound) return false;
  // Check() returns false in case there is no value.
  if (!value) return false;
  return TestProperty (idx);
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

