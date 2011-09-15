/*
    Crystal Space Entity Layer
    Copyright (C) 2009 by Jorrit Tyberghein
    Copyright (C) 2009 by Pablo Martin
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
#include "iutil/document.h"
#include "iutil/plugin.h"
#include "ivaria/reporter.h"
#include "tools/parameters.h"
#include "celtool/stdparams.h"

#include "physicallayer/pl.h"

#include "plugins/tools/rewards/reward_createentity.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_FACTORY (celCreateEntityRewardType)
CEL_IMPLEMENT_REWARDTYPE(CreateEntity)

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.rewards.createentity",
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
//---------- celCreateEntityRewardFactory implementation

celCreateEntityRewardFactory::celCreateEntityRewardFactory (
	celCreateEntityRewardType* type) : scfImplementationType (this)
{
  celCreateEntityRewardFactory::type = type;
  params.AttachNew (new celVariableParameterBlock ());
}

celCreateEntityRewardFactory::~celCreateEntityRewardFactory ()
{
}

csPtr<iReward> celCreateEntityRewardFactory::CreateReward (
    iQuest* q, const celParams& params)
{
  celCreateEntityReward* newquest = new celCreateEntityReward (type,
  	params, template_par, name_par, celCreateEntityRewardFactory::params);
  return newquest;
}

bool celCreateEntityRewardFactory::Load (iDocumentNode* node)
{
  template_par.Empty ();
  name_par.Empty ();
  params.AttachNew (new celVariableParameterBlock ());

  // required parameters
  template_par = node->GetAttributeValue ("template");
  if (!template_par)
    return Report (type->object_reg,
      "'template' attribute is missing for the createentity reward!");

  // optional parameters
  name_par = node->GetAttributeValue ("name");

  // now parse template parameters
  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* node_name = child->GetValue ();
    if (!strcmp ("par", node_name))
    {
      const char *name = child->GetAttributeValue ("name");
      const char *value = child->GetAttributeValue ("value");
      if (!name || !value)
      {
        return Report (type->object_reg,
"Missing name or value attribute in a parameter for the createentity reward!");
      }
      AddParameter(name,value);
    }
  }
  return true;
}

void celCreateEntityRewardFactory::SetEntityTemplateParameter (
	const char* entity_tpl)
{
  template_par = entity_tpl;
}

void celCreateEntityRewardFactory::SetNameParameter (
	const char* name)
{
  name_par = name;
}

void celCreateEntityRewardFactory::AddParameter (const char* name, 
		const char* value)
{
  csStringID id = type->pl->FetchStringID (name);
  for (size_t i = 0 ; i < params->GetParameterCount () ; i++)
  {
    celDataType t;
    csStringID pid = params->GetParameterDef (i, t);
    if (pid == id)
    {
      params->GetParameter (i).Set (value);
      return;
    }
  }
  size_t idx = params->GetParameterCount ();
  params->SetParameterDef (idx, id);
  params->GetParameter (idx).Set (value);
}

//---------------------------------------------------------------------------
//---------- celCreateEntityReward implementation

celCreateEntityReward::celCreateEntityReward (
	celCreateEntityRewardType* type,
  	const celParams& params,
	const char* template_par,
	const char* name_par,
        iCelParameterBlock* tpl_params) : scfImplementationType (this)
{
  celCreateEntityReward::type = type;

  pm = csQueryRegistryOrLoad<iParameterManager> 
    (type->object_reg, "cel.parameters.manager");

  entity_tpl = pm->GetParameter (params, template_par);
  name = pm->GetParameter (params, name_par);

  // Resolve template parameters.
  size_t idx = 0;
  celCreateEntityReward::params.AttachNew (new celVariableParameterBlock ());
  for (size_t i = 0 ; i < tpl_params->GetParameterCount () ; i++)
  {
    celDataType t;
    csStringID id = tpl_params->GetParameterDef (i, t);
    const char* name = type->pl->FetchString (id);
    // @@@ Support dynamic parameters?
    const celData* data = tpl_params->GetParameterByIndex (i);
    csString val;
    celParameterTools::ToString (*data, val);

    celCreateEntityReward::params->SetParameterDef (idx, type->pl->FetchStringID (name));
    celCreateEntityReward::params->GetParameter (idx).Set (val);
    idx++;
  }
}

celCreateEntityReward::~celCreateEntityReward ()
{
}

void celCreateEntityReward::Reward (iCelParameterBlock* params)
{
  const char* e = entity_tpl->Get (params);
  if (!e) return;

  iCelPlLayer* pl = type->pl;
  iCelEntityTemplate* ent_tpl = pl->FindEntityTemplate (e);
  if (!ent_tpl)
  {
    Report (type->object_reg,
      		"entity template %s not found for createentity reward!", e);
    return;
  }

  const char* n = name->Get (params);
  if (!n) return;
  pl->CreateEntity (ent_tpl, n, celCreateEntityReward::params);
}

//---------------------------------------------------------------------------

