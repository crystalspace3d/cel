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

#include "celtool/stdparams.h"
#include "behaviourlayer/behave.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"

#include "plugins/tools/rewards/reward_action.h"

//---------------------------------------------------------------------------
SCF_IMPLEMENT_FACTORY (celActionRewardType)
CEL_IMPLEMENT_REWARDTYPE(Action)

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.rewards.action",
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

celActionRewardFactory::celActionRewardFactory (
	celActionRewardType* type) : scfImplementationType (this)
{
  celActionRewardFactory::type = type;
}

celActionRewardFactory::~celActionRewardFactory ()
{
}

csPtr<iReward> celActionRewardFactory::CreateReward (
	iQuest* q, iCelParameterBlock* params)
{
  iReward* reward;
  if (!entity_par.IsEmpty())
  {
    reward = new celActionReward (this,
  	params, entity_par, id_par, pcclass_par, tag_par);
  }
  else
  {
    if (!class_par)
    {
      Report (type->object_reg,
	      "No entity or class defined while creating action reward!");
      return csPtr<iReward> (nullptr);
    }

    reward = new celClassActionReward (this,
  	params, class_par, id_par, pcclass_par, tag_par);
  }
  return reward;
}

bool celActionRewardFactory::Load (iDocumentNode* node)
{
  entity_par = node->GetAttributeValue ("entity");
  class_par = node->GetAttributeValue ("class");
  pcclass_par = node->GetAttributeValue ("pc");
  tag_par = node->GetAttributeValue ("tag");
  id_par = node->GetAttributeValue ("id");

  // sanity checks
  if (entity_par.IsEmpty() && class_par.IsEmpty())
    return Report (type->object_reg,
      "'entity' or 'class' attribute required for the action reward!");
  if (id_par.IsEmpty())
    Report (type->object_reg,
      "'id' attribute is missing for the action reward!");
  if (pcclass_par.IsEmpty())
    Report (type->object_reg,
      "'pc' attribute is missing for the action reward!");
  
  // get parameters
  iCelPlLayer* pl = type->pl;
  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    if (!strcmp ("par", value))
    {
      const char* name = child->GetAttributeValue ("name");
      if (!name)
        return Report (type->object_reg,
          "Missing name attribute in a parameter for the action reward!");
      csStringID id = pl->FetchStringID (name);
      const char* str_value = child->GetAttributeValue ("string");
      if (str_value)
      {
	AddParameter (CEL_DATA_STRING, id, str_value);
	continue;
      }
      const char* vec3_value = child->GetAttributeValue ("vector3");
      if (vec3_value)
      {
	AddParameter (CEL_DATA_VECTOR3, id, vec3_value);
	continue;
      }
      const char* vec2_value = child->GetAttributeValue ("vector2");
      if (vec2_value)
      {
	AddParameter (CEL_DATA_VECTOR2, id, vec2_value);
	continue;
      }
      const char* float_value = child->GetAttributeValue ("float");
      if (float_value)
      {
	AddParameter (CEL_DATA_FLOAT, id, float_value);
	continue;
      }
      const char* long_value = child->GetAttributeValue ("long");
      if (long_value)
      {
	AddParameter (CEL_DATA_LONG, id, long_value);
	continue;
      }
      const char* bool_value = child->GetAttributeValue ("bool");
      if (bool_value)
      {
	AddParameter (CEL_DATA_BOOL, id, bool_value);
	continue;
      }
      return Report (type->object_reg,
        "Unknown parameter type for action reward!");
    }
    else
    {
      return Report (type->object_reg,
        "Unexpected child '%s' in the action reward!", value);
    }
  }
  return true;
}

void celActionRewardFactory::SetEntityParameter (
	const char* entity)
{
  entity_par = entity;
}

void celActionRewardFactory::SetIDParameter (
	const char* id)
{
  id_par = id;
}

void celActionRewardFactory::SetPropertyClassParameter (
	const char* propertyclass)
{
  pcclass_par = propertyclass;
}

void celActionRewardFactory::SetTagParameter (
	const char* pctag)
{
  tag_par = pctag;
}

void celActionRewardFactory::SetClassParameter (
        const char* ent_class)
{
  class_par = ent_class;
}

void celActionRewardFactory::AddParameter (celDataType type,
    csStringID id, const char* value)
{
  size_t idx = parameters.Push (celParSpec ());
  parameters[idx].type = type;
  parameters[idx].id = id;
  parameters[idx].value = value;
}

//---------------------------------------------------------------------------

celActionReward::celActionReward (
	celActionRewardFactory* factory,
  	iCelParameterBlock* params,
	const char* entity_par,
	const char* id_par,
	const char* pcclass_par,
	 const char* tag_par)
  : scfImplementationType (this), factory (factory)
{
  pm = csQueryRegistryOrLoad<iParameterManager> 
    (factory->type->object_reg, "cel.parameters.manager");

  pcclass = pm->GetParameter (params, pcclass_par);
  tag = pm->GetParameter (params, tag_par);
  entity = pm->GetParameter (params, entity_par);
  id = pm->GetParameter (params, id_par);
  act_params = celParameterTools::GetParameterBlock (pm, params,
      factory->parameters, quest_parameters);
}

celActionReward::~celActionReward ()
{
}

void celActionReward::Reward (iCelParameterBlock* params)
{
  iCelPlLayer* pl = factory->type->pl;

  bool changed;
  // XXX parsing of entity has to be refactored as it is very commonly
  // done.

  const celData * data = entity->GetData (params);
  if (data->type == CEL_DATA_ENTITY)
  {
    ent = data->value.ent;
  }

  else
  {
    const char* e = entity->Get (params, changed);
    if (changed) ent = 0;
    if (!ent)
    {
      ent = pl->FindEntity (e);
      if (!ent) return;
    }
  }
  csRef<iCelPropertyClass> propertyclass;

  const char* pc = pcclass->Get (params);
  if (!pc) return;
  const char* t = tag->Get (params);
  propertyclass = ent->GetPropertyClassList()->FindByNameAndTag (pc, t);
  if (propertyclass)
  {
    const char* idname = id->Get (params);
    csStringID actionID = pl->FetchStringID (idname);
    if (actionID)
    {
      celData ret;
      if (!celParameterTools::FillParameterBlock (params, act_params, factory->parameters, quest_parameters))
	Report (factory->type->object_reg,
		"Could not fill parameters '%s' in the specified pc!", idname);
      else
	propertyclass->PerformAction (actionID, act_params, ret);
    }
    else
      Report (factory->type->object_reg,
	      "No action '%s' in the specified pc!", idname);
  }
  else
    Report (factory->type->object_reg,
            "No propertyclass  '%s' in the specified entity!", pc);
}

//---------------------------------------------------------------------------

celClassActionReward::celClassActionReward (
	celActionRewardFactory* factory,
  	iCelParameterBlock* params,
	const char* class_par,
	const char* id_par,
	const char* pcclass_par,
	const char* tag_par)
  : scfImplementationType (this), factory (factory)
{
  pm = csQueryRegistryOrLoad<iParameterManager> 
    (factory->type->object_reg, "cel.parameters.manager");
  
  pcclass = pm->GetParameter (params, pcclass_par);
  tag = pm->GetParameter (params, tag_par);
  clazz = pm->GetParameter (params, class_par);
  id = pm->GetParameter (params, id_par);

  act_params = celParameterTools::GetParameterBlock (pm, params,
      factory->parameters, quest_parameters);
  actionID = csInvalidStringID;
}

celClassActionReward::~celClassActionReward ()
{
}

void celClassActionReward::Reward (iCelParameterBlock* params)
{
  // run actions
  iCelEntity *ent;
  csRef<iCelPropertyClass> propertyclass;
  celData ret;

  bool changed;
  const char* clz = clazz->Get (params, changed);
  if (changed || !entlist)
  {
    csStringID ent_class = factory->type->pl->FetchStringID (clz);
    entlist = factory->type->pl->GetClassEntitiesList (ent_class);
  }

  const char* aid = id->Get (params, changed);
  if (changed || actionID == csInvalidStringID)
    actionID = factory->type->pl->FetchStringID (aid);

  const char* pc = pcclass->Get (params);
  if (!pc) return;
  const char* t = tag->Get (params);

  size_t i = entlist->GetCount();
  while (i>0)
  {
    i--;
    ent = entlist->Get(i);
    propertyclass = ent->GetPropertyClassList()->FindByNameAndTag (pc, t);
    if (propertyclass)
    {
      if (!celParameterTools::FillParameterBlock (params, act_params, factory->parameters, quest_parameters))
	Report (factory->type->object_reg,
		"Could not fill parameters in the specified pc '%s' '%s'!", pc, t);
      else
	propertyclass->PerformAction (actionID, act_params, ret);
    }
  }
}


//---------------------------------------------------------------------------

