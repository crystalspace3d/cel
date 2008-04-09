/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein

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

#include "celtool/stdparams.h"
#include "behaviourlayer/behave.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"

#include "plugins/tools/quests/quests.h"
#include "plugins/tools/quests/reward_action.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_REWARDTYPE(Action)

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.quests.reward.action",
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

csPtr<iQuestReward> celActionRewardFactory::CreateReward (
    iQuest*, const celQuestParams& params)
{
  iQuestReward* reward;
  if (!entity_par.IsEmpty())
  {
    reward = new celActionReward (type,
  	params, entity_par, id_par, pcclass_par, tag_par, parameters);
  }
  else
  {
    reward = new celClassActionReward (type,
  	params, class_par, id_par, pcclass_par, tag_par, parameters);
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
      csString fullname = "cel.parameter.";
      fullname += name;
      csStringID id = pl->FetchStringID (fullname);
      const char* str_value = child->GetAttributeValue ("string");
      if (str_value)
      {
	AddParameter (CEL_DATA_STRING, id, name, str_value);
	continue;
      }
      const char* vec3_value = child->GetAttributeValue ("vector3");
      if (vec3_value)
      {
	AddParameter (CEL_DATA_VECTOR3, id, name, vec3_value);
	continue;
      }
      const char* vec2_value = child->GetAttributeValue ("vector2");
      if (vec2_value)
      {
	AddParameter (CEL_DATA_VECTOR2, id, name, vec2_value);
	continue;
      }
      const char* float_value = child->GetAttributeValue ("float");
      if (float_value)
      {
	AddParameter (CEL_DATA_FLOAT, id, name, float_value);
	continue;
      }
      const char* long_value = child->GetAttributeValue ("long");
      if (long_value)
      {
	AddParameter (CEL_DATA_LONG, id, name, long_value);
	continue;
      }
      const char* bool_value = child->GetAttributeValue ("bool");
      if (bool_value)
      {
	AddParameter (CEL_DATA_BOOL, id, name, bool_value);
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
    csStringID id, const char* name, const char* value)
{
  size_t idx = parameters.Push (celParSpec ());
  parameters[idx].type = type;
  parameters[idx].id = id;
  parameters[idx].name = name;
  parameters[idx].value = value;
}

//---------------------------------------------------------------------------

celActionReward::celActionReward (
	celActionRewardType* type,
  	const celQuestParams& params,
	const char* entity_par,
	const char* id_par,
	const char* pcclass_par,
	const char* tag_par,
	const csArray<celParSpec>& parameters) : scfImplementationType (this)
{
  celActionReward::type = type;
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  pcclass = qm->ResolveParameter (params, pcclass_par, pcclass_dynamic);
  tag = qm->ResolveParameter (params, tag_par, tag_dynamic);
  entity = qm->ResolveParameter (params, entity_par, entity_dynamic);
  id = qm->ResolveParameter (params, id_par, id_dynamic);
  act_params = qm->ResolveParameterBlock (params, parameters);
}

celActionReward::~celActionReward ()
{
  delete act_params;
}

void celActionReward::Reward (iCelParameterBlock* params)
{
  iCelPlLayer* pl = type->pl;

  if (entity_dynamic != csInvalidStringID)
  {
    const char* e = GetDynamicParValue (type->object_reg, params, entity_dynamic, entity);
    if (!e) return;
    if (entity != e) { ent = 0; entity = e; }
  }

  if (!ent)
  {
    ent = pl->FindEntity (entity);
    if (!ent) return;
  }
  csRef<iCelPropertyClass> propertyclass;

  const char* pc = GetDynamicParValue (type->object_reg, params, pcclass_dynamic, pcclass);
  if (!pc) return;
  const char* t = GetDynamicParValue (type->object_reg, params, tag_dynamic, tag);
  propertyclass = ent->GetPropertyClassList()->FindByNameAndTag (pc, t);
  if (propertyclass)
  {
    csString fullname = "cel.action.";
    fullname += id;
    csStringID actionID = pl->FetchStringID(fullname);
    if (actionID)
    {
      celData ret;
      propertyclass->PerformAction(actionID, act_params, ret);
    }
    else
      Report (type->object_reg,
		"No action  'cel.action.%s' in the specified pc!", 
		id.GetData());
  }
  else
    Report (type->object_reg,
            "No propertyclass  '%s' in the specified entity!", 
	    pcclass.GetData());
}

//---------------------------------------------------------------------------

celClassActionReward::celClassActionReward (
	celActionRewardType* type,
  	const celQuestParams& params,
	const char* class_par,
	const char* id_par,
	const char* pcclass_par,
	const char* tag_par,
	const csArray<celParSpec>& parameters) : scfImplementationType (this)
{
  iCelPlLayer* pl = type->pl;
  celClassActionReward::type = type;
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  pcclass = qm->ResolveParameter (params, pcclass_par, pcclass_dynamic);
  tag = qm->ResolveParameter (params, tag_par, tag_dynamic);
  act_params = qm->ResolveParameterBlock (params, parameters);

  // Get the entity class list pointer.
  clazz = qm->ResolveParameter (params, class_par, clazz_dynamic);
  if (clazz_dynamic == csInvalidStringID)
  {
    csStringID ent_class = type->pl->FetchStringID (clazz);
    entlist = type->pl->GetClassEntitiesList (ent_class);
  }
  else
  {
    clazz.Empty ();
  }

  // prepare action name
  csString id = qm->ResolveParameter (params, id_par, id_dynamic);
  if (id_dynamic != csInvalidStringID)
  {
    csString fullname = "cel.action.";
    fullname += id;
    actionID = pl->FetchStringID(fullname);
  }
}

celClassActionReward::~celClassActionReward ()
{
  delete act_params;
}

void celClassActionReward::Reward (iCelParameterBlock* params)
{
  // run actions
  iCelEntity *ent;
  csRef<iCelPropertyClass> propertyclass;
  celData ret;

  if (clazz_dynamic != csInvalidStringID)
  {
    const char* cl = GetDynamicParValue (type->object_reg, params, clazz_dynamic, clazz);
    if (!cl) return;
    if (clazz != cl)
    {
      clazz = cl;
      csStringID ent_class = type->pl->FetchStringID (clazz);
      entlist = type->pl->GetClassEntitiesList (ent_class);
    }
  }

  const char* pc = GetDynamicParValue (type->object_reg, params, pcclass_dynamic, pcclass);
  if (!pc) return;
  const char* t = GetDynamicParValue (type->object_reg, params, tag_dynamic, tag);

  for (int i = entlist->GetCount()-1; i>=0; i--)
  {
    ent = entlist->Get(i);
    propertyclass = ent->GetPropertyClassList()->FindByNameAndTag (pc, t);
    if (propertyclass)
    {
      propertyclass->PerformAction(actionID, act_params, ret);
    }
  }
}


//---------------------------------------------------------------------------

