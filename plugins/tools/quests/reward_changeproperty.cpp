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

SCF_IMPLEMENT_IBASE (celChangePropertyRewardFactory)
  SCF_IMPLEMENTS_INTERFACE (iQuestRewardFactory)
  SCF_IMPLEMENTS_INTERFACE (iChangePropertyQuestRewardFactory)
SCF_IMPLEMENT_IBASE_END

celChangePropertyRewardFactory::celChangePropertyRewardFactory (
	celChangePropertyRewardType* type)
{
  SCF_CONSTRUCT_IBASE (0);
  celChangePropertyRewardFactory::type = type;
  prop_par = 0;
  entity_par = 0;
  pc_par = 0;
  tag_par = 0;
  string_par = 0;
  long_par = 0;
  float_par = 0;
  bool_par = 0;
  diff_par = 0;
  do_toggle = false;
}

celChangePropertyRewardFactory::~celChangePropertyRewardFactory ()
{
  delete[] prop_par;
  delete[] entity_par;
  delete[] pc_par;
  delete[] tag_par;
  delete[] string_par;
  delete[] long_par;
  delete[] float_par;
  delete[] bool_par;
  delete[] diff_par;

  SCF_DESTRUCT_IBASE ();
}

csPtr<iQuestReward> celChangePropertyRewardFactory::CreateReward (
    const csHash<csStrKey,csStrKey>& params)
{
  celChangePropertyReward* trig = new celChangePropertyReward (type,
  	params, prop_par, entity_par, pc_par, tag_par, string_par, long_par,
	float_par, bool_par, diff_par, do_toggle);
  return trig;
}

bool celChangePropertyRewardFactory::Load (iDocumentNode* node)
{
  delete[] prop_par; prop_par = 0;
  delete[] entity_par; entity_par = 0;
  delete[] pc_par; pc_par = 0;
  delete[] tag_par; tag_par = 0;
  delete[] string_par; string_par = 0;
  delete[] long_par; long_par = 0;
  delete[] float_par; float_par = 0;
  delete[] bool_par; bool_par = 0;
  delete[] diff_par; diff_par = 0;
  do_toggle = false;
  prop_par = csStrNew (node->GetAttributeValue ("property"));
  entity_par = csStrNew (node->GetAttributeValue ("entity"));
  pc_par = csStrNew (node->GetAttributeValue ("pc"));
  tag_par = csStrNew (node->GetAttributeValue ("tag"));
  string_par = csStrNew (node->GetAttributeValue ("string"));
  long_par = csStrNew (node->GetAttributeValue ("long"));
  float_par = csStrNew (node->GetAttributeValue ("float"));
  bool_par = csStrNew (node->GetAttributeValue ("bool"));
  diff_par = csStrNew (node->GetAttributeValue ("diff"));
  if (node->GetAttributeValue ("toggle"))
    do_toggle = true;

  if (!prop_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questreward.debugprint",
      "'property' attribute is missing for the changeproperty reward!");
    return false;
  }
  if (!entity_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questreward.debugprint",
      "'entity' attribute is missing for the changeproperty reward!");
    return false;
  }
  return true;
}

void celChangePropertyRewardFactory::SetPropertyParameter (
	const char* prop)
{
  if (prop_par == prop) return;
  delete[] prop_par;
  prop_par = csStrNew (prop);
}

void celChangePropertyRewardFactory::SetEntityParameter (
	const char* entity)
{
  if (entity_par != entity)
  {
    delete[] entity_par;
    entity_par = csStrNew (entity);
  }
}

void celChangePropertyRewardFactory::SetPCParameter (const char* pc,
	const char* tag)
{
  if (pc_par != pc)
  {
    delete[] pc_par;
    pc_par = csStrNew (pc);
  }
  if (tag_par != tag)
  {
    delete[] tag_par;
    tag_par = csStrNew (tag);
  }
}

void celChangePropertyRewardFactory::SetStringParameter (
	const char* str)
{
  if (string_par == str) return;
  delete[] string_par;
  string_par = csStrNew (str);
}

void celChangePropertyRewardFactory::SetLongParameter (
	const char* str)
{
  if (long_par == str) return;
  delete[] long_par;
  long_par = csStrNew (str);
}

void celChangePropertyRewardFactory::SetFloatParameter (
	const char* str)
{
  if (float_par == str) return;
  delete[] float_par;
  float_par = csStrNew (str);
}

void celChangePropertyRewardFactory::SetBoolParameter (
	const char* str)
{
  if (bool_par == str) return;
  delete[] bool_par;
  bool_par = csStrNew (str);
}

void celChangePropertyRewardFactory::SetDiffParameter (
	const char* str)
{
  if (diff_par == str) return;
  delete[] diff_par;
  diff_par = csStrNew (str);
}

void celChangePropertyRewardFactory::SetToggle ()
{
  do_toggle = true;
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celChangePropertyReward)
  SCF_IMPLEMENTS_INTERFACE (iQuestReward)
SCF_IMPLEMENT_IBASE_END

celChangePropertyReward::celChangePropertyReward (
	celChangePropertyRewardType* type,
  	const csHash<csStrKey,csStrKey>& params,
	const char* prop_par,
	const char* entity_par,
	const char* pc_par,
	const char* tag_par,
	const char* string_par,
	const char* long_par,
	const char* float_par,
	const char* bool_par,
	const char* diff_par,
	bool do_toggle)
{
  SCF_CONSTRUCT_IBASE (0);
  celChangePropertyReward::type = type;
  csRef<iQuestManager> qm = CS_QUERY_REGISTRY (type->object_reg, iQuestManager);
  prop = csStrNew (qm->ResolveParameter (params, prop_par));
  entity = csStrNew (qm->ResolveParameter (params, entity_par));
  pc = csStrNew (qm->ResolveParameter (params, pc_par));
  tag = csStrNew (qm->ResolveParameter (params, tag_par));
  pstring = csStrNew (qm->ResolveParameter (params, string_par));
  plong = csStrNew (qm->ResolveParameter (params, long_par));
  pfloat = csStrNew (qm->ResolveParameter (params, float_par));
  pbool = csStrNew (qm->ResolveParameter (params, bool_par));
  pdiff = csStrNew (qm->ResolveParameter (params, diff_par));
  celChangePropertyReward::do_toggle = do_toggle;
}

celChangePropertyReward::~celChangePropertyReward ()
{
  delete[] prop;
  delete[] entity;
  delete[] pc;
  delete[] tag;
  delete[] pstring;
  delete[] plong;
  delete[] pfloat;
  delete[] pbool;
  delete[] pdiff;
  SCF_DESTRUCT_IBASE ();
}

void celChangePropertyReward::Reward ()
{
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (type->object_reg, iCelPlLayer);
  if (!pc && !properties)
  {
    if (!ent)
    {
      ent = pl->FindEntity (entity);
      if (!ent) return;
    }
    properties = CEL_QUERY_PROPCLASS_ENT (ent, iPcProperties);
    if (!properties) return;
  }
  if (pc && !pclass)
  {
    if (!ent)
    {
      ent = pl->FindEntity (entity);
      if (!ent) return;
    }
    pclass = ent->GetPropertyClassList ()->FindByNameAndTag (pc, tag);
    if (!pclass) return;
  }

  if (pstring)
  {
    if (pc) pclass->SetProperty (pl->FetchStringID (prop), pstring);
    else properties->SetProperty (prop, pstring);
    return;
  }
  if (plong)
  {
    long l;
    sscanf (plong, "%ld", &l);
    if (pc) pclass->SetProperty (pl->FetchStringID (prop), l);
    else properties->SetProperty (prop, l);
    return;
  }
  if (pfloat)
  {
    float f;
    sscanf (pfloat, "%f", &f);
    if (pc) pclass->SetProperty (pl->FetchStringID (prop), f);
    else properties->SetProperty (prop, f);
    return;
  }
  if (pbool)
  {
    bool b;
    csScanStr (pbool, "%b", &b);
    if (pc) pclass->SetProperty (pl->FetchStringID (prop), b);
    else properties->SetProperty (prop, b);
    return;
  }
  if (pdiff)
  {
    if (pc)
    {
      csStringID id = pl->FetchStringID (prop);
      celDataType t = pclass->GetPropertyOrActionType (id);
      switch (t)
      {
        case CEL_DATA_LONG:
	  {
	    long diff;
	    sscanf (pdiff, "%ld", &diff);
	    long l = pclass->GetPropertyLong (id);
	    pclass->SetProperty (id, l+diff);
	  }
	  return;
        case CEL_DATA_FLOAT:
	  {
	    float diff;
	    sscanf (pdiff, "%f", &diff);
	    float f = pclass->GetPropertyFloat (id);
	    pclass->SetProperty (id, f+diff);
	  }
	  return;
        default: break;
      }
    }
    else
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
  }
  if (do_toggle)
  {
    if (pc)
    {
      csStringID id = pl->FetchStringID (prop);
      celDataType t = pclass->GetPropertyOrActionType (id);
      switch (t)
      {
        case CEL_DATA_LONG:
	  {
	    long l = pclass->GetPropertyLong (id);
	    pclass->SetProperty (id, !l);
	  }
	  return;
        case CEL_DATA_FLOAT:
	  {
	    float f = pclass->GetPropertyFloat (id);
	    pclass->SetProperty (id, fabs (f) < .00001 ? 1.0f : 0.0f);
	  }
	  return;
        case CEL_DATA_BOOL:
	  {
	    bool f = pclass->GetPropertyBool (id);
	    pclass->SetProperty (id, !f);
	  }
	  return;
        default: break;
      }
    }
    else
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
  }
  printf ("Warning! ChangeProperty didn't change anything!\n");
  fflush (stdout);
}

//---------------------------------------------------------------------------

