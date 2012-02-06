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

#include "plugins/tools/rewards/reward_message.h"

//---------------------------------------------------------------------------
SCF_IMPLEMENT_FACTORY (celMessageRewardType)
CEL_IMPLEMENT_REWARDTYPE(Message)

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.rewards.message",
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

csPtr<iReward> celMessageRewardFactory::CreateReward (
    iQuest* q, iCelParameterBlock* params)
{
  iReward* reward;
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

bool celMessageRewardFactory::Save (iDocumentNode* node)
{
  if (!entity_par.IsEmpty ()) node->SetAttribute ("entity", entity_par);
  if (!class_par.IsEmpty ()) node->SetAttribute ("class", class_par);
  if (!id_par.IsEmpty ()) node->SetAttribute ("id", id_par);
  if (!celParameterTools::WriteParSpecBlock (type->object_reg, node, parameters))
    return false;
  return true;
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

  if (!celParameterTools::ParseParSpecBlock (type->object_reg, node, parameters))
    return false;
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
    csStringID id, const char* value)
{
  size_t idx = parameters.Push (celParSpec ());
  parameters[idx].type = type;
  parameters[idx].id = id;
  parameters[idx].value = value;
}

//---------------------------------------------------------------------------

celMessageReward::celMessageReward (
	celMessageRewardType* type,
  	iCelParameterBlock* params,
	const char* entity_par,
	const char* id_par,
	const csArray<celParSpec>& parameters) 
	: scfImplementationType (this), parameters (parameters)
{
  celMessageReward::type = type;

  pm = csQueryRegistryOrLoad<iParameterManager> 
    (type->object_reg, "cel.parameters.manager");

  msg_id = pm->GetParameter (params, id_par);
  entity = pm->GetParameter (params, entity_par);

  msg_params = celParameterTools::GetParameterBlock (pm, params, parameters, quest_parameters);
}

void celMessageReward::Reward (iCelParameterBlock* params)
{
  const char* msg = msg_id->Get (params);
  if (!msg) return;

  iCelEntity* newent = pm->ResolveEntityParameter (type->pl, params, entity, ent);
  if (!newent) return;
  if (newent != ent) { dispatcher = 0; ent = newent; }

  if (!celParameterTools::FillParameterBlock (type->pl, params, msg_params,
	parameters, quest_parameters))
  {
    Report (type->object_reg,
	    "Could not fill parameters for message '%s'", msg);
    return;
  }

  iCelBehaviour* behave = ent->GetBehaviour ();
  if (behave)
  {
    celData ret;
    behave->SendMessage (msg, 0, ret, msg_params);
  }
  if (!dispatcher)
    dispatcher = ent->QueryMessageChannel ()->CreateMessageDispatcher (
	this, type->pl->FetchStringID (msg));
  dispatcher->SendMessage (msg_params);
}

//---------------------------------------------------------------------------

celClassMessageReward::celClassMessageReward (
	celMessageRewardType* type,
  	iCelParameterBlock* params,
	const char* class_par,
	const char* id_par,
	const csArray<celParSpec>& parameters)
	: scfImplementationType (this), parameters (parameters)
{
  celClassMessageReward::type = type;

  pm = csQueryRegistryOrLoad<iParameterManager> 
    (type->object_reg, "cel.parameters.manager");

  // message id and parameters
  msg_id = pm->GetParameter (params, id_par);
  clazz = pm->GetParameter (params, class_par);

  msg_params = celParameterTools::GetParameterBlock (pm, params, parameters, quest_parameters);
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

  if (!celParameterTools::FillParameterBlock (type->pl, params, msg_params,
	parameters, quest_parameters))
  {
    Report (type->object_reg,
	    "Could not fill parameters for message '%s'", msg);
    return;
  }

  // Old method for behaviours.
  type->pl->SendMessage (entlist, msg, msg_params);

  // New method for pclasses.
  type->pl->SendMessage (type->pl->FetchStringID (msg), 0, entlist, msg_params);
}

