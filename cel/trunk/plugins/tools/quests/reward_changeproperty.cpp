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

#include "plugins/tools/quests/quests.h"
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
  prop = qm->ResolveParameter (params, prop_par, prop_dynamic);
  pc = qm->ResolveParameter (params, pc_par, pc_dynamic);
  if (pc_dynamic != csInvalidStringID)
    pc.Empty ();
  tag = qm->ResolveParameter (params, tag_par, tag_dynamic);
  if (tag_dynamic != csInvalidStringID)
    tag.Empty ();
  pstring = qm->ResolveParameter (params, string_par, pstring_dynamic);
  plong = qm->ResolveParameter (params, long_par, plong_dynamic);
  pfloat = qm->ResolveParameter (params, float_par, pfloat_dynamic);
  pbool = qm->ResolveParameter (params, bool_par, pbool_dynamic);
  pdiff = qm->ResolveParameter (params, diff_par, pdiff_dynamic);
  celChangePropertyRewardBase::do_toggle = do_toggle;
}

void celChangePropertyRewardBase::ChangePropertyOnPc (iCelPropertyClass *pclass,
    iCelParameterBlock* params)
{
  const char* p = GetDynamicParValue (type->object_reg, params, prop_dynamic, prop);
  if (!p) return;
  iCelPlLayer* pl = type->pl;
  // Do NOT use IsEmpty() here! Empty string is valid data.
  if (pstring_dynamic != csInvalidStringID || pstring.GetData () != 0)
  {
    const char* v = GetDynamicParValue (type->object_reg, params, pstring_dynamic, pstring);
    if (!v) return;
    pclass->SetProperty (pl->FetchStringID (p), v);
    return;
  }
  if (plong_dynamic != csInvalidStringID || !plong.IsEmpty ())
  {
    const char* v = GetDynamicParValue (type->object_reg, params, plong_dynamic, plong);
    if (!v) return;
    long l;
    sscanf (v, "%ld", &l);
    pclass->SetProperty (pl->FetchStringID (p), l);
    return;
  }
  if (pfloat_dynamic != csInvalidStringID || !pfloat.IsEmpty ())
  {
    const char* v = GetDynamicParValue (type->object_reg, params, pfloat_dynamic, pfloat);
    if (!v) return;
    float f;
    sscanf (v, "%f", &f);
    pclass->SetProperty (pl->FetchStringID (p), f);
    return;
  }
  if (pbool_dynamic != csInvalidStringID || !pbool.IsEmpty ())
  {
    const char* v = GetDynamicParValue (type->object_reg, params, pbool_dynamic, pbool);
    if (!v) return;
    bool b;
    csScanStr (v, "%b", &b);
    pclass->SetProperty (pl->FetchStringID (p), b);
    return;
  }
  if (pdiff_dynamic || !pdiff.IsEmpty ())
  {
    const char* v = GetDynamicParValue (type->object_reg, params, pdiff_dynamic, pdiff);
    if (!v) return;
    csStringID id = pl->FetchStringID (p);
    celDataType t = pclass->GetPropertyOrActionType (id);
    switch (t)
    {
      case CEL_DATA_LONG:
	{
	  long diff;
	  sscanf (v, "%ld", &diff);
	  long l = pclass->GetPropertyLongByID (id);
	  pclass->SetProperty (id, l+diff);
	}
	return;
      case CEL_DATA_FLOAT:
	{
	  float diff;
	  sscanf (v, "%f", &diff);
	  float f = pclass->GetPropertyFloatByID (id);
	  pclass->SetProperty (id, f+diff);
	}
	return;
      default: break;
    }
  }
  if (do_toggle)
  {
    csStringID id = pl->FetchStringID (p);
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

void celChangePropertyRewardBase::ChangePropertyOnPcProp (iPcProperties *properties,
    iCelParameterBlock* params)
{
  const char* p = GetDynamicParValue (type->object_reg, params, prop_dynamic, prop);
  if (!p) return;
  // Do NOT use IsEmpty() here! Empty string is valid data.
  if (pstring_dynamic != csInvalidStringID || pstring.GetData () != 0)
  {
    const char* v = GetDynamicParValue (type->object_reg, params, pstring_dynamic, pstring);
    if (!v) return;
    properties->SetProperty (p, v);
    return;
  }
  if (plong_dynamic != csInvalidStringID || !plong.IsEmpty ())
  {
    const char* v = GetDynamicParValue (type->object_reg, params, plong_dynamic, plong);
    if (!v) return;
    long l;
    sscanf (v, "%ld", &l);
    properties->SetProperty (p, l);
    return;
  }
  if (pfloat_dynamic != csInvalidStringID || !pfloat.IsEmpty ())
  {
    const char* v = GetDynamicParValue (type->object_reg, params, pfloat_dynamic, pfloat);
    if (!v) return;
    float f;
    sscanf (v, "%f", &f);
    properties->SetProperty (p, f);
    return;
  }
  if (pbool_dynamic != csInvalidStringID || !pbool.IsEmpty ())
  {
    const char* v = GetDynamicParValue (type->object_reg, params, pbool_dynamic, pbool);
    if (!v) return;
    bool b;
    csScanStr (v, "%b", &b);
    properties->SetProperty (p, b);
    return;
  }
  if (pdiff_dynamic != csInvalidStringID || !pdiff.IsEmpty ())
  {
    const char* v = GetDynamicParValue (type->object_reg, params, pdiff_dynamic, pdiff);
    if (!v) return;
    size_t idx = properties->GetPropertyIndex (p);
    if (idx != (size_t)~0)
    {
      celDataType t = properties->GetPropertyType (idx);
      switch (t)
      {
        case CEL_DATA_LONG:
	  {
	    long diff;
	    sscanf (v, "%ld", &diff);
	    long l = properties->GetPropertyLong (idx);
	    properties->SetPropertyIndex (idx, l+diff);
	  }
	  return;
        case CEL_DATA_FLOAT:
	  {
	    float diff;
	    sscanf (v, "%f", &diff);
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
    size_t idx = properties->GetPropertyIndex (p);
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
  entity = qm->ResolveParameter (params, entity_par, entity_dynamic);
  if (entity_dynamic != csInvalidStringID)
    entity.Empty ();
}

void celChangePropertyReward::Reward (iCelParameterBlock* params)
{
  iCelPlLayer* pl = type->pl;
  // check and/or find the entity.
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
  // Generic Property class style
  if (pc_dynamic != csInvalidStringID || !pc.IsEmpty())
  {
    if (pc_dynamic != csInvalidStringID || tag_dynamic != csInvalidStringID)
    {
      const char* p = GetDynamicParValue (type->object_reg, params, pc_dynamic, pc);
      if (!p) return;
      if (pc != p) { pclass = 0; pc = p; }
      const char* t = GetDynamicParValue (type->object_reg, params, tag_dynamic, tag);
      if (tag != t) { pclass = 0; tag = t; }
    }
    if (!pclass)
    {
      pclass = ent->GetPropertyClassList ()->FindByNameAndTag (pc, tag);
      if (!pclass) return;
    }
    ChangePropertyOnPc (pclass, params);
  }
  else
  {
    // PcProperties style
    if (!properties)
    {
      pclass = 0;
      properties = CEL_QUERY_PROPCLASS_ENT (ent, iPcProperties);
      if (!properties) return;
    }
    ChangePropertyOnPcProp (properties, params);
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
}

void celClassChangePropertyReward::PcReward (iCelParameterBlock* params)
{
  iCelEntity *ent;
  iCelPropertyClass *pclass;
  for (int i = entlist->GetCount()-1; i>=0; i--)
  {
    ent = entlist->Get(i);
    const char* p = GetDynamicParValue (type->object_reg, params, pc_dynamic, pc);
    if (!p) return;
    const char* t = GetDynamicParValue (type->object_reg, params, tag_dynamic, tag);
    pclass = ent->GetPropertyClassList ()->FindByNameAndTag (p, t);
    if (pclass)
      ChangePropertyOnPc (pclass, params);
  }
}

void celClassChangePropertyReward::PcPropReward (iCelParameterBlock* params)
{
  iCelEntity *ent;
  csRef<iPcProperties> properties;
  for (int i = entlist->GetCount()-1; i>=0; i--)
  {
    ent = entlist->Get(i);
    properties = CEL_QUERY_PROPCLASS_ENT (ent, iPcProperties);
    if (properties)
      ChangePropertyOnPcProp (properties, params);
  }
}

void celClassChangePropertyReward::Reward (iCelParameterBlock* params)
{
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

  if (pc_dynamic != csInvalidStringID || !pc.IsEmpty())
    PcReward (params);
  else
    PcPropReward (params);
}

