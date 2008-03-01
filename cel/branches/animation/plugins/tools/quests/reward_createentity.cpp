/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein
    Copyright (C) 2008 by Pablo Martin

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
#include "ivaria/reporter.h"

#include "physicallayer/pl.h"

#include "plugins/tools/quests/reward_createentity.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_REWARDTYPE(CreateEntity)

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.quests.reward.createentity",
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
  template_par = 0;
  name_par = 0;
}

celCreateEntityRewardFactory::~celCreateEntityRewardFactory ()
{
  delete[] template_par;
  delete[] name_par;
}

csPtr<iQuestReward> celCreateEntityRewardFactory::CreateReward (
    iQuest*, const celQuestParams& params)
{
  celCreateEntityReward* newquest = new celCreateEntityReward (type,
  	params, template_par, name_par, celCreateEntityRewardFactory::params);
  return newquest;
}

bool celCreateEntityRewardFactory::Load (iDocumentNode* node)
{
  delete[] template_par; template_par = 0;
  delete[] name_par; name_par = 0;
  params.DeleteAll();

  // required parameters
  template_par = csStrNew (node->GetAttributeValue ("template"));
  if (!template_par)
    return Report (type->object_reg,
      "'template' attribute is missing for the createentity reward!");

  // optional parameters
  name_par = csStrNew (node->GetAttributeValue ("name"));

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
  if (template_par != entity_tpl)
  {
    delete[] template_par;
    template_par = csStrNew (entity_tpl);
  }
}

void celCreateEntityRewardFactory::SetNameParameter (
	const char* name)
{
  if (name_par != name)
  {
    delete[] name_par;
    name_par = csStrNew (name);
  }
}
void celCreateEntityRewardFactory::AddParameter (const char* name, 
		const char* value)
{
  params.PutUnique(name,value);
}

//---------------------------------------------------------------------------
//---------- celCreateEntityReward implementation

celCreateEntityReward::celCreateEntityReward (
	celCreateEntityRewardType* type,
  	const celQuestParams& params,
	const char* template_par,
	const char* name_par,
        const celEntityTemplateParams &tpl_params) : scfImplementationType (this)
{
  celCreateEntityReward::type = type;
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);

  entity_tpl = csStrNew (qm->ResolveParameter (params, template_par));
  name = csStrNew (qm->ResolveParameter (params, name_par));

  // resolve template parameters
  celEntityTemplateParams::ConstGlobalIterator iter = tpl_params.GetIterator();
  while (iter.HasNext())
  {
    csStringFast<12> name;
    const char * val = qm->ResolveParameter(params,iter.Next(name));
    celCreateEntityReward::params.Put(name,val);
  }
}

celCreateEntityReward::~celCreateEntityReward ()
{
  delete[] entity_tpl;
  delete[] name;
}

void celCreateEntityReward::Reward ()
{
  iCelPlLayer* pl = type->pl;
  iCelEntityTemplate* ent_tpl = pl->FindEntityTemplate (entity_tpl);
  if (!ent_tpl)
  {
    Report (type->object_reg,
      		"entity template %s not found for createentity reward!",
				entity_tpl);
    return;
  }

  pl->CreateEntity(ent_tpl,name,params);
}

//---------------------------------------------------------------------------

