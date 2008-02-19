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

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"

#include "plugins/tools/quests/reward_changeproperty.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_REWARDTYPE(ChangeProperty)

//---------------------------------------------------------------------------

celChangePropertyRewardFactory::celChangePropertyRewardFactory (
	celChangePropertyRewardType* type) : scfImplementationType (this)
{
  celChangePropertyRewardFactory::type = type;
  do_toggle = false;
}

celChangePropertyRewardFactory::~celChangePropertyRewardFactory ()
{
}

csPtr<iQuestReward> celChangePropertyRewardFactory::CreateReward (
    iQuest*, const celQuestParams& params)
{
  iQuestReward* reward;
  if (entity_par)
  {
    reward = new celChangePropertyReward (type,
  	params, prop_par, entity_par, pc_par, tag_par, string_par, long_par,
	float_par, bool_par, diff_par, do_toggle);
  }
  else
  {
    reward = new celClassChangePropertyReward (type,
  	params, prop_par, class_par, pc_par, tag_par, string_par, long_par,
	float_par, bool_par, diff_par, do_toggle);
  }
  return reward;
}

bool celChangePropertyRewardFactory::Load (iDocumentNode* node)
{
  do_toggle = false;
  prop_par = node->GetAttributeValue ("property");
  entity_par = node->GetAttributeValue ("entity");
  class_par = node->GetAttributeValue ("class");
  pc_par = node->GetAttributeValue ("pc");
  tag_par = node->GetAttributeValue ("tag");
  string_par = node->GetAttributeValue ("string");
  long_par = node->GetAttributeValue ("long");
  float_par = node->GetAttributeValue ("float");
  bool_par = node->GetAttributeValue ("bool");
  diff_par = node->GetAttributeValue ("diff");
  if (node->GetAttributeValue ("toggle"))
    do_toggle = true;

  if (prop_par.IsEmpty ())
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questreward.changeproperty",
      "'property' attribute is missing for the changeproperty reward!");
    return false;
  }
  if (entity_par.IsEmpty () && class_par.IsEmpty ())
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questreward.changeproperty",
      "'entity' or 'class' attribute required for the changeproperty reward!");
    return false;
  }
  return true;
}

void celChangePropertyRewardFactory::SetPropertyParameter (
	const char* prop)
{
  prop_par = prop;
}

void celChangePropertyRewardFactory::SetEntityParameter (
	const char* entity)
{
  entity_par = entity;
}

void celChangePropertyRewardFactory::SetClassParameter (
	const char* ent_class)
{
  class_par = ent_class;
}

void celChangePropertyRewardFactory::SetPCParameter (const char* pc,
	const char* tag)
{
  pc_par = pc;
  tag_par = tag;
}

void celChangePropertyRewardFactory::SetStringParameter (
	const char* str)
{
  string_par = str;
}

void celChangePropertyRewardFactory::SetLongParameter (
	const char* str)
{
  long_par = str;
}

void celChangePropertyRewardFactory::SetFloatParameter (
	const char* str)
{
  float_par = str;
}

void celChangePropertyRewardFactory::SetBoolParameter (
	const char* str)
{
  bool_par = str;
}

void celChangePropertyRewardFactory::SetDiffParameter (
	const char* str)
{
  diff_par = str;
}

void celChangePropertyRewardFactory::SetToggle ()
{
  do_toggle = true;
}

//---------------------------------------------------------------------------
//----  celChangePropertyRewardBase

celChangePropertyRewardBase::celChangePropertyRewardBase (
	celChangePropertyRewardType* type,
  	const celQuestParams& params,
	const char* prop_par,
	const char* pc_par,
	const char* tag_par,
	const char* string_par,
	const char* long_par,
	const char* float_par,
	const char* bool_par,
	const char* diff_par,
	bool do_toggle) : scfImplementationType (this)
{
  celChangePropertyRewardBase::type = type;
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  prop = qm->ResolveParameter (params, prop_par);
  pc = qm->ResolveParameter (params, pc_par);
  tag = qm->ResolveParameter (params, tag_par);
  pstring = qm->ResolveParameter (params, string_par);
  plong = qm->ResolveParameter (params, long_par);
  pfloat = qm->ResolveParameter (params, float_par);
  pbool = qm->ResolveParameter (params, bool_par);
  pdiff = qm->ResolveParameter (params, diff_par);
  celChangePropertyRewardBase::do_toggle = do_toggle;
}

void celChangePropertyRewardBase::ChangePropertyOnPc(iCelPropertyClass *pclass)
{
  iCelPlLayer* pl = type->pl;
  // Do NOT use IsEmpty() here! Empty string is valid data.
  if (pstring.GetData () != 0)
  {
    pclass->SetProperty (pl->FetchStringID (prop), pstring);
    return;
  }
  if (!plong.IsEmpty ())
  {
    long l;
    sscanf (plong, "%ld", &l);
    pclass->SetProperty (pl->FetchStringID (prop), l);
    return;
  }
  if (!pfloat.IsEmpty ())
  {
    float f;
    sscanf (pfloat, "%f", &f);
    pclass->SetProperty (pl->FetchStringID (prop), f);
    return;
  }
  if (!pbool.IsEmpty ())
  {
    bool b;
    csScanStr (pbool, "%b", &b);
    pclass->SetProperty (pl->FetchStringID (prop), b);
    return;
  }
  if (!pdiff.IsEmpty ())
  {
      csStringID id = pl->FetchStringID (prop);
      celDataType t = pclass->GetPropertyOrActionType (id);
      switch (t)
      {
        case CEL_DATA_LONG:
	  {
	    long diff;
	    sscanf (pdiff, "%ld", &diff);
	    long l = pclass->GetPropertyLongByID (id);
	    pclass->SetProperty (id, l+diff);
	  }
	  return;
        case CEL_DATA_FLOAT:
	  {
	    float diff;
	    sscanf (pdiff, "%f", &diff);
	    float f = pclass->GetPropertyFloatByID (id);
	    pclass->SetProperty (id, f+diff);
	  }
	  return;
        default: break;
    }
  }
  if (do_toggle)
  {
      csStringID id = pl->FetchStringID (prop);
      celDataType t = pclass->GetPropertyOrActionType (id);
      switch (t)
      {
        case CEL_DATA_LONG:
	  {
	    long l = pclass->GetPropertyLongByID (id);
	    pclass->SetProperty (id, !l);
	  }
	  return;
        case CEL_DATA_FLOAT:
	  {
	    float f = pclass->GetPropertyFloatByID (id);
	    pclass->SetProperty (id, fabs (f) < .00001 ? 1.0f : 0.0f);
	  }
	  return;
        case CEL_DATA_BOOL:
	  {
	    bool f = pclass->GetPropertyBoolByID (id);
	    pclass->SetProperty (id, !f);
	  }
	  return;
        default: break;
      }
  }
  printf ("Warning! ChangeProperty didn't change anything!\n");
  fflush (stdout);
}

void celChangePropertyRewardBase::ChangePropertyOnPcProp(iPcProperties *properties)
{
  // Do NOT use IsEmpty() here! Empty string is valid data.
  if (pstring.GetData () != 0)
  {
    properties->SetProperty (prop, pstring);
    return;
  }
  if (!plong.IsEmpty ())
  {
    long l;
    sscanf (plong, "%ld", &l);
    properties->SetProperty (prop, l);
    return;
  }
  if (!pfloat.IsEmpty ())
  {
    float f;
    sscanf (pfloat, "%f", &f);
    properties->SetProperty (prop, f);
    return;
  }
  if (!pbool.IsEmpty ())
  {
    bool b;
    csScanStr (pbool, "%b", &b);
    properties->SetProperty (prop, b);
    return;
  }
  if (!pdiff.IsEmpty ())
  {
      size_t idx = properties->GetPropertyIndex (prop);
      if (idx != (size_t)~0)
      {
        celDataType t = properties->GetPropertyType (idx);
        switch (t)
        {
          case CEL_DATA_LONG:
	    {
	      long diff;
	      sscanf (pdiff, "%ld", &diff);
	      long l = properties->GetPropertyLong (idx);
	      properties->SetPropertyIndex (idx, l+diff);
	    }
	    return;
          case CEL_DATA_FLOAT:
	    {
	      float diff;
	      sscanf (pdiff, "%f", &diff);
	      float f = properties->GetPropertyFloat (idx);
	      properties->SetPropertyIndex (idx, f+diff);
	    }
	    return;
          default: break;
        }
      }
  }
  if (do_toggle)
  {
      size_t idx = properties->GetPropertyIndex (prop);
      if (idx != (size_t)~0)
      {
        celDataType t = properties->GetPropertyType (idx);
        switch (t)
        {
          case CEL_DATA_LONG:
	    {
	      long l = properties->GetPropertyLong (idx);
	      properties->SetPropertyIndex (idx, !l);
	    }
	    return;
          case CEL_DATA_FLOAT:
	    {
	      float f = properties->GetPropertyFloat (idx);
	      properties->SetPropertyIndex (idx, fabs (f) < .00001 ? 1.0f : 0.0f);
	    }
	    return;
          case CEL_DATA_BOOL:
	    {
	      bool f = properties->GetPropertyBool (idx);
	      properties->SetPropertyIndex (idx, !f);
	    }
	    return;
          default: break;
        }
      }
  }
  printf ("Warning! ChangeProperty didn't change anything!\n");
  fflush (stdout);
}

//---------------------------------------------------------------------------
//----  celChangePropertyReward
celChangePropertyReward::celChangePropertyReward (
	celChangePropertyRewardType* type,
  	const celQuestParams& params,
	const char* prop_par,
	const char* entity_par,
	const char* pc_par,
	const char* tag_par,
	const char* string_par,
	const char* long_par,
	const char* float_par,
	const char* bool_par,
	const char* diff_par,
	bool do_toggle) : celChangePropertyRewardBase(type, params, prop_par,
				pc_par, tag_par, string_par, long_par,
				float_par, bool_par, diff_par, do_toggle)
{
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  entity = qm->ResolveParameter (params, entity_par);
}

void celChangePropertyReward::Reward ()
{
  iCelPlLayer* pl = type->pl;
  // check and/or find the entity.
  if (!ent)
  {
    ent = pl->FindEntity (entity);
    if (!ent) return;
  }
  // PcProperties style
  if (pc.IsEmpty())
  {
    if (!properties)
    {
      pclass = 0;
      properties = CEL_QUERY_PROPCLASS_ENT (ent, iPcProperties);
      if (!properties) return;
    }
    ChangePropertyOnPcProp(properties);
  }
  // Generic Property class style
  if (!pc.IsEmpty())
  {
    if (!pclass)
    {
      pclass = ent->GetPropertyClassList ()->FindByNameAndTag (pc, tag);
      if (!pclass) return;
    }
    ChangePropertyOnPc(pclass);
  }
}

//---------------------------------------------------------------------------
//----  celClassChangePropertyReward
celClassChangePropertyReward::celClassChangePropertyReward (
	celChangePropertyRewardType* type,
  	const celQuestParams& params,
	const char* prop_par,
	const char* class_par,
	const char* pc_par,
	const char* tag_par,
	const char* string_par,
	const char* long_par,
	const char* float_par,
	const char* bool_par,
	const char* diff_par,
	bool do_toggle) : celChangePropertyRewardBase(type, params, prop_par,
				pc_par, tag_par, string_par, long_par,
				float_par, bool_par, diff_par, do_toggle)
{
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  csStringID ent_class = 
	type->pl->FetchStringID(qm->ResolveParameter (params, class_par));
  entlist = type->pl->GetClassEntitiesList(ent_class);
}

void celClassChangePropertyReward::PcReward ()
{
  iCelEntity *ent;
  iCelPropertyClass *pclass;
  for (int i = entlist->GetCount()-1; i>=0; i--)
  {
    ent = entlist->Get(i);
    pclass = ent->GetPropertyClassList ()->FindByNameAndTag (pc, tag);
    if (pclass)
      ChangePropertyOnPc(pclass);
  }
}

void celClassChangePropertyReward::PcPropReward ()
{
  iCelEntity *ent;
  csRef<iPcProperties> properties;
  for (int i = entlist->GetCount()-1; i>=0; i--)
  {
    ent = entlist->Get(i);
    properties = CEL_QUERY_PROPCLASS_ENT (ent, iPcProperties);
    if (properties)
      ChangePropertyOnPcProp(properties);
  }
}

void celClassChangePropertyReward::Reward ()
{
  if (pc.IsEmpty())
    PcPropReward();
  else
    PcReward();
}
