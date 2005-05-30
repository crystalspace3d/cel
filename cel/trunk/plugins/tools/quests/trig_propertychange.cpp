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

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celPropertyChangeTriggerFactory)
  SCF_IMPLEMENTS_INTERFACE (iQuestTriggerFactory)
  SCF_IMPLEMENTS_INTERFACE (iPropertyChangeQuestTriggerFactory)
SCF_IMPLEMENT_IBASE_END

celPropertyChangeTriggerFactory::celPropertyChangeTriggerFactory (
	celPropertyChangeTriggerType* type)
{
  SCF_CONSTRUCT_IBASE (0);
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

  SCF_DESTRUCT_IBASE ();
}

csPtr<iQuestTrigger> celPropertyChangeTriggerFactory::CreateTrigger (
    const celQuestParams& params)
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
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questtrigger.propertychange",
      "'entity' attribute is missing for the propertychange trigger!");
    return false;
  }
  prop_par = csStrNew (node->GetAttributeValue ("property"));
  if (!prop_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questtrigger.propertychange",
      "'property' attribute is missing for the propertychange trigger!");
    return false;
  }
  value_par = csStrNew (node->GetAttributeValue ("value"));
  if (!value_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questtrigger.propertychange",
      "'value' attribute is missing for the propertychange trigger!");
    return false;
  }
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

SCF_IMPLEMENT_IBASE (celPropertyChangeTrigger)
  SCF_IMPLEMENTS_INTERFACE (iQuestTrigger)
  SCF_IMPLEMENTS_INTERFACE (iPcPropertyListener)
SCF_IMPLEMENT_IBASE_END

celPropertyChangeTrigger::celPropertyChangeTrigger (
	celPropertyChangeTriggerType* type,
  	const celQuestParams& params,
	const char* entity_par, const char* tag_par,
	const char* prop_par, const char* value_par)
{
  SCF_CONSTRUCT_IBASE (0);
  celPropertyChangeTrigger::type = type;
  csRef<iQuestManager> qm = CS_QUERY_REGISTRY (type->object_reg, iQuestManager);
  entity = csStrNew (qm->ResolveParameter (params, entity_par));
  tag = csStrNew (qm->ResolveParameter (params, tag_par));
  prop = csStrNew (qm->ResolveParameter (params, prop_par));
  value = csStrNew (qm->ResolveParameter (params, value_par));
}

celPropertyChangeTrigger::~celPropertyChangeTrigger ()
{
  DeactivateTrigger ();
  delete[] entity;
  delete[] tag;
  delete[] prop;
  delete[] value;
  SCF_DESTRUCT_IBASE ();
}

void celPropertyChangeTrigger::RegisterCallback (iQuestTriggerCallback* callback)
{
  celPropertyChangeTrigger::callback = callback;
}

void celPropertyChangeTrigger::ClearCallback ()
{
  callback = 0;
}

void celPropertyChangeTrigger::PropertyChanged (iPcProperties* properties,
	size_t idx)
{
  celDataType type = properties->GetPropertyType (idx);
  csString fmt;
  switch (type)
  {
    case CEL_DATA_LONG:
      {
        long v;
	sscanf (value, "%ld", &v);
	if (v == properties->GetPropertyLong (idx))
	{
	  DeactivateTrigger ();
	  callback->TriggerFired ((iQuestTrigger*)this);
        }
      }
      break;
    case CEL_DATA_FLOAT:
      {
        float v;
	sscanf (value, "%g", &v);
	if (v == properties->GetPropertyFloat (idx))
	{
	  DeactivateTrigger ();
	  callback->TriggerFired ((iQuestTrigger*)this);
        }
      }
      break;
    case CEL_DATA_BOOL:
      {
        bool v;
	v = *value == '1' || *value == 'Y' || *value == 'y' || *value == 't'
		|| *value == 'T';
	if (v == properties->GetPropertyBool (idx))
	{
	  DeactivateTrigger ();
	  callback->TriggerFired ((iQuestTrigger*)this);
        }
      }
      break;
    case CEL_DATA_STRING:
      {
	if (!strcmp (value, properties->GetPropertyString (idx)))
	{
	  DeactivateTrigger ();
	  callback->TriggerFired ((iQuestTrigger*)this);
        }
      }
      break;
    default:
      break;
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
  if (!properties) return;	// @@@ Report error!
  // First remove to make sure we don't register ourselves multiple
  // times.
  properties->RemovePropertyListener ((iPcPropertyListener*)this);
  properties->AddPropertyListener ((iPcPropertyListener*)this);
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

