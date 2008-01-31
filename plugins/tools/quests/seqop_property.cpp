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
#include "csutil/objreg.h"
#include "csutil/util.h"
#include "iutil/document.h"
#include "ivaria/reporter.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "physicallayer/persist.h"

#include "plugins/tools/quests/seqop_property.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_SEQOPTYPE(Property)

//---------------------------------------------------------------------------

celPropertySeqOpFactory::celPropertySeqOpFactory (
	celPropertySeqOpType* type) : scfImplementationType (this)
{
  celPropertySeqOpFactory::type = type;
  entity_par = 0;
  tag_par = 0;
  pc_par = 0;
  prop_par = 0;
  float_par = 0;
  long_par = 0;
  vx_par = 0;
  vy_par = 0;
  vz_par = 0;
}

celPropertySeqOpFactory::~celPropertySeqOpFactory ()
{
  delete[] entity_par;
  delete[] tag_par;
  delete[] pc_par;
  delete[] prop_par;
  delete[] float_par;
  delete[] long_par;
  delete[] vx_par;
  delete[] vy_par;
  delete[] vz_par;
}

csPtr<iQuestSeqOp> celPropertySeqOpFactory::CreateSeqOp (
    const celQuestParams& params)
{
  celPropertySeqOp *seqop = 0;
  if (float_par)
  {
    seqop = new celFloatPropertySeqOp (type,
  	params, entity_par, pc_par, tag_par, relative, prop_par, float_par);
  }
  else if (long_par)
  {
    seqop = new celLongPropertySeqOp (type,
  	params, entity_par, pc_par, tag_par, relative, prop_par, long_par);
  }
  else if (vz_par)
  {
    seqop = new celVector3PropertySeqOp (type,
  	params, entity_par, pc_par, tag_par, relative, prop_par, vx_par, 
	vy_par, vz_par);
  }
  else if (vy_par)
  {
    seqop = new celVector2PropertySeqOp (type,
  	params, entity_par, pc_par, tag_par, relative, prop_par, vx_par, vy_par);
  }
  return seqop;
}

bool celPropertySeqOpFactory::Load (iDocumentNode* node)
{
  delete[] entity_par; entity_par = 0;
  delete[] tag_par; tag_par = 0;
  delete[] pc_par; pc_par = 0;
  delete[] prop_par; prop_par = 0;
  delete[] float_par; float_par = 0;
  delete[] long_par; long_par = 0;
  delete[] vx_par; vx_par = 0;
  delete[] vy_par; vy_par = 0;
  delete[] vz_par; vz_par = 0;

  entity_par = csStrNew (node->GetAttributeValue ("entity"));
  if (!entity_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questreward.property",
      "'entity' attribute is missing for the property seqop!");
    return false;
  }
  pc_par = csStrNew (node->GetAttributeValue ("pc"));
  if (!pc_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questreward.property",
      "'pc' attribute is missing for the property seqop!");
    return false;
  }
  tag_par = csStrNew (node->GetAttributeValue ("tag"));
  prop_par = csStrNew (node->GetAttributeValue ("property"));
   if (!prop_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questreward.property",
      "'property' attribute is missing for the property seqop!");
    return false;
  }
 
  // now parse different values
  relative = node->GetAttributeValueAsBool ("relative",false);
  // try float
  float_par = csStrNew (node->GetAttributeValue ("float"));
  if (!float_par)
  {
    // else try vector
    csRef<iDocumentNode> v_node = node->GetNode ("v");
    if (v_node)
    {
      vx_par = csStrNew (v_node->GetAttributeValue ("x"));
      vy_par = csStrNew (v_node->GetAttributeValue ("y"));
      vz_par = csStrNew (v_node->GetAttributeValue ("z"));
    }
    else
    {
      // else try long
      long_par = csStrNew (node->GetAttributeValue ("long"));
      if (!long_par)
      {
        // else report error
        csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
          "cel.questreward.property",
          "a float, long or vector must be specified for the property seqop!");
        return false;
      }
    }
  }
  return true;
}

void celPropertySeqOpFactory::SetEntityParameter (const char* entity)
{
  if (entity_par != entity)
  {
    delete[] entity_par;
    entity_par = csStrNew (entity);
  }
}

void celPropertySeqOpFactory::SetPCParameter (const char* pc,
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

void celPropertySeqOpFactory::SetPropertyParameter (const char* propname)
{
  if (prop_par != propname)
  {
    delete[] prop_par;
    prop_par = csStrNew (propname);
  }
}

void celPropertySeqOpFactory::SetFloatParameter (const char* pfloat)
{
  if (float_par != pfloat)
  {
    delete[] float_par;
    float_par = csStrNew (pfloat);
  }
}

void celPropertySeqOpFactory::SetLongParameter (const char* plong)
{
  if (long_par != plong)
  {
    delete[] long_par;
    long_par = csStrNew (plong);
  }
}

void celPropertySeqOpFactory::SetVector2Parameter (const char* pvx,
		const char* pvy)
{
  if (vx_par != pvx)
  {
    delete[] vx_par;
    vx_par = csStrNew (pvx);
  }
  if (vy_par != pvy)
  {
    delete[] vy_par;
    vy_par = csStrNew (pvy);
  }
}

void celPropertySeqOpFactory::SetVector3Parameter (const char* pvx,
		const char* pvy,const char* pvz)
{
  SetVector2Parameter(pvx,pvy);
  if (vz_par != pvz)
  {
    delete[] vz_par;
    vz_par = csStrNew (pvz);
  }
}

void celPropertySeqOpFactory::SetRelative (bool is_relative)
{
  relative = is_relative;
}
//---------------------------------------------------------------------------

static float ToFloat (const char* s)
{
  if (!s) return 0.0f;
  float f;
  sscanf (s, "%f", &f);
  return f;
}

celPropertySeqOp::celPropertySeqOp (
	celPropertySeqOpType* type,
  	const celQuestParams& params,
	const char* entity_par, const char* pc_par, const char* tag_par,
	const char* prop_par, bool rel_par) 
	: scfImplementationType (this)
{
  celPropertySeqOp::type = type;
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);

  entity = csStrNew (qm->ResolveParameter (params, entity_par));
  pcname = csStrNew (qm->ResolveParameter (params, pc_par));
  tag = csStrNew (qm->ResolveParameter (params, tag_par));
  
  relative = rel_par;

  propname = csStrNew (qm->ResolveParameter (params, prop_par));
}

celPropertySeqOp::~celPropertySeqOp ()
{
  delete[] entity;
  delete[] tag;
  delete[] propname;
  delete[] pcname;
}

void celPropertySeqOp::FindPCProperty ()
{
  if (prop_ok) return;

  iCelEntity* ent = type->pl->FindEntity (entity);
  if (ent)
  {
    pc = ent->GetPropertyClassList()->FindByNameAndTag(pcname,tag);
    if (pc)
    {
      propID = type->pl->FetchStringID(propname);
      celDataType proptype = pc->GetPropertyOrActionType(propID);
      bool readonly = pc->IsPropertyReadOnly(propID);
      if ((proptype == celPropertySeqOp::proptype) && (!readonly))
      {
        prop_ok = 1;
        GetStartValue();
      }
    }
  }
}

void celPropertySeqOp::Init ()
{
  pc = 0;
  prop_ok = 0;
  FindPCProperty ();
}

void celPropertySeqOp::Do (float time)
{
  if (prop_ok && pc)
  {
     SetCurrentValue(time);
  }
}

//---------------------------------------------------------------------------
//--------- celFloatPropertySeqOp

celFloatPropertySeqOp::celFloatPropertySeqOp (
	celPropertySeqOpType* type,
  	const celQuestParams& params,
	const char* entity_par, const char* pc_par, const char* tag_par,
	bool rel_par, const char* prop_par, const char* pfloat) 
        : celPropertySeqOp(type,params,entity_par,pc_par,tag_par,prop_par,
		rel_par)
{
  proptype = CEL_DATA_FLOAT;
  // XXX duplicate looking for quest manager
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  end = ToFloat(qm->ResolveParameter (params, pfloat));
}

//---------------------------------------------------------------------------
//--------- celVector2PropertySeqOp

celVector2PropertySeqOp::celVector2PropertySeqOp (
	celPropertySeqOpType* type,
  	const celQuestParams& params,
	const char* entity_par, const char* pc_par, const char* tag_par,
	bool rel_par, const char* prop_par, const char* vx, const char* vy) 
        : celPropertySeqOp(type,params,entity_par,pc_par,tag_par,prop_par,
		rel_par)
{
  proptype = CEL_DATA_VECTOR2;
  // XXX duplicate looking for quest manager
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  end.x = ToFloat(qm->ResolveParameter (params, vx));
  end.y = ToFloat(qm->ResolveParameter (params, vy));
}

//---------------------------------------------------------------------------
//--------- celVector3PropertySeqOp

celVector3PropertySeqOp::celVector3PropertySeqOp (
	celPropertySeqOpType* type,
  	const celQuestParams& params,
	const char* entity_par, const char* pc_par, const char* tag_par,
	bool rel_par,
	const char* prop_par, const char* vx, const char* vy, const char* vz) 
        : celPropertySeqOp(type,params,entity_par,pc_par,tag_par,prop_par,
		rel_par)
{
  proptype = CEL_DATA_VECTOR3;
  // XXX duplicate looking for quest manager
  csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  end.x = ToFloat(qm->ResolveParameter (params, vx));
  end.y = ToFloat(qm->ResolveParameter (params, vy));
  end.z = ToFloat(qm->ResolveParameter (params, vz));
}

//---------------------------------------------------------------------------

