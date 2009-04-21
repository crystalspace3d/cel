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
#include "plugins/tools/quests/reward_message.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_REWARDTYPE(Message)

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.quests.reward.message",
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

celMessageRewardFactory::celMessageRewardFactory (
	celMessageRewardType* type) : scfImplementationType (this)
{
  celMessageRewardFactory::type = type;
}

csPtr<iQuestReward> celMessageRewardFactory::CreateReward (
    iQuest*, const celQuestParams& params)
{
  iQuestReward* reward;
  if (!entity_par.IsEmpty())
  {
    reward = new celMessageReward (type,
  	params, entity_par, id_par, parameters);
  }
  else
  {
    reward = new celClassMessageReward (type,
  	params, class_par, id_par, parameters);
  }
  return reward;
}

bool celMessageRewardFactory::Load (iDocumentNode* node)
{
  parameters.DeleteAll();
  entity_par = node->GetAttributeValue ("entity");
  class_par = node->GetAttributeValue ("class");
  id_par = node->GetAttributeValue ("id");

  if (entity_par.IsEmpty() && class_par.IsEmpty())
    return Report (type->object_reg,
      "'entity' or 'class' attribute required for the message reward!");
  if (id_par.IsEmpty())
    Report (type->object_reg,
      "'id' attribute is missing for the message reward!");

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
          "Missing name attribute in a parameter for the message reward!");
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
        "Unknown parameter type for message reward!");
    }
    else
    {
      return Report (type->object_reg,
        "Unexpected child '%s' in the message reward!", value);
    }
  }
  return true;
}

void celMessageRewardFactory::SetEntityParameter (
	const char* entity)
{
  entity_par = entity;
}

void celMessageRewardFactory::SetClassParameter (
	const char* ent_class)
{
  class_par = ent_class;
}

void celMessageRewardFactory::SetIDParameter (
	const char* id)
{
  id_par = id;
}

void celMessageRewardFactory::AddParameter (celDataType type,
    csStringID id, const char* name, const char* value)
{
  size_t idx = parameters.Push (celParSpec ());
  parameters[idx].type = type;
  parameters[idx].id = id;
  parameters[idx].name = name;
  parameters[idx].value = value;
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

celMessageReward::celMessageReward (
	celMessageRewardType* type,
  	const celQuestParams& params,
	const char* entity_par,
	const char* id_par,
	const csArray<celParSpec>& parameters) 
	: scfImplementationType (this), parameters (parameters)
{
  celMessageReward::type = type;
  qm = csQueryRegistry<iQuestManager> (type->object_reg);

  msg_id = qm->GetParameter (params, id_par);
  entity = qm->GetParameter (params, entity_par);

  quest_parameters.SetSize (parameters.GetSize (), 0);
  msg_params = qm->GetParameterBlock (params, parameters, quest_parameters);
}

void celMessageReward::Reward (iCelParameterBlock* params)
{
  const char* msg = msg_id->Get (params);
  if (!msg) return;

  bool changed;
  // XXX parsing of entity has to be refactored as it is very commonly
  // done.
  const celData * data = entity->GetData(params);
  if (data->type == CEL_DATA_ENTITY)
  {
    iCelEntity *new_ent = data->value.ent;
    if (new_ent != ent)
      dispatcher = 0;  // Clear previous dispatcher.
    ent = new_ent;
  }
  else
  {
    const char* e = entity->Get (params, changed);
    if (changed) { ent = 0; }
    if (!ent)
    {
      dispatcher = 0;  // Clear previous dispatcher.
      iCelPlLayer* pl = type->pl;
      ent = pl->FindEntity (e);
      if (!ent) return;
    }
  }
  qm->FillParameterBlock (params, msg_params, parameters, quest_parameters);
  iCelBehaviour* behave = ent->GetBehaviour ();
  if (behave)
  {
    celData ret;
    behave->SendMessage (msg, 0, ret, msg_params);
  }
  if (!dispatcher)
    dispatcher = ent->QueryMessageChannel ()->CreateMessageDispatcher (
	this, msg);
  dispatcher->SendMessage (msg_params);
}

//---------------------------------------------------------------------------

celClassMessageReward::celClassMessageReward (
	celMessageRewardType* type,
  	const celQuestParams& params,
	const char* class_par,
	const char* id_par,
	const csArray<celParSpec>& parameters)
	: scfImplementationType (this), parameters (parameters)
{
  celClassMessageReward::type = type;
  qm = csQueryRegistry<iQuestManager> (type->object_reg);

  // message id and parameters
  msg_id = qm->GetParameter (params, id_par);
  clazz = qm->GetParameter (params, class_par);

  quest_parameters.SetSize (parameters.GetSize (), 0);
  msg_params = qm->GetParameterBlock (params, parameters, quest_parameters);
}

void celClassMessageReward::Reward (iCelParameterBlock* params)
{
  const char* msg = msg_id->Get (params);
  if (!msg) return;
  bool changed;
  const char* clz = clazz->Get (params, changed);
  if (changed || !entlist)
  {
    csStringID ent_class = type->pl->FetchStringID (clz);
    entlist = type->pl->GetClassEntitiesList (ent_class);
  }

  qm->FillParameterBlock (params, msg_params, parameters, quest_parameters);

  // Old method for behaviours.
  type->pl->SendMessage (entlist, msg, msg_params);

  // New method for pclasses.
  type->pl->SendMessage (msg, 0, entlist, msg_params);
}

