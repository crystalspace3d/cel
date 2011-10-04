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
#include "csutil/objreg.h"
#include "csutil/util.h"
#include "iutil/document.h"
#include "iutil/plugin.h"
#include "ivaria/reporter.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "physicallayer/persist.h"

#include "plugins/tools/sequences/seqop_property.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_FACTORY (celPropertySeqOpType)
CEL_IMPLEMENT_SEQOPTYPE(Property)

//---------------------------------------------------------------------------

celPropertySeqOpFactory::celPropertySeqOpFactory (
	celPropertySeqOpType* type) : scfImplementationType (this)
{
  celPropertySeqOpFactory::type = type;
}

celPropertySeqOpFactory::~celPropertySeqOpFactory ()
{
}

csPtr<iSeqOp> celPropertySeqOpFactory::CreateSeqOp (
    iCelParameterBlock* params)
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
  entity_par = node->GetAttributeValue ("entity");
  if (!entity_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.seqops.property",
      "'entity' attribute is missing for the property seqop!");
    return false;
  }
  pc_par = node->GetAttributeValue ("pc");
  if (!pc_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.seqops.property",
      "'pc' attribute is missing for the property seqop!");
    return false;
  }
  tag_par = node->GetAttributeValue ("tag");
  prop_par = node->GetAttributeValue ("property");
   if (!prop_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.seqops.property",
      "'property' attribute is missing for the property seqop!");
    return false;
  }
 
  // now parse different values
  relative = node->GetAttributeValueAsBool ("relative",false);
  // try float
  float_par = node->GetAttributeValue ("float");
  if (!float_par)
  {
    // else try vector
    csRef<iDocumentNode> v_node = node->GetNode ("v");
    if (v_node)
    {
      vx_par = v_node->GetAttributeValue ("x");
      vy_par = v_node->GetAttributeValue ("y");
      vz_par = v_node->GetAttributeValue ("z");
    }
    else
    {
      // else try long
      long_par = node->GetAttributeValue ("long");
      if (!long_par)
      {
        // else report error
        csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
          "cel.seqops.property",
          "a float, long or vector must be specified for the property seqop!");
        return false;
      }
    }
  }
  return true;
}

void celPropertySeqOpFactory::SetEntityParameter (const char* entity)
{
  entity_par = entity;
}

void celPropertySeqOpFactory::SetPCParameter (const char* pc,
	const char* tag)
{
  pc_par = pc;
  tag_par = tag;
}

void celPropertySeqOpFactory::SetPropertyParameter (const char* propname)
{
  prop_par = propname;
}

void celPropertySeqOpFactory::SetFloatParameter (const char* pfloat)
{
  float_par = pfloat;
}

void celPropertySeqOpFactory::SetLongParameter (const char* plong)
{
  long_par = plong;
}

void celPropertySeqOpFactory::SetVector2Parameter (const char* pvx,
		const char* pvy)
{
  vx_par = pvx;
  vy_par = pvy;
}

void celPropertySeqOpFactory::SetVector3Parameter (const char* pvx,
		const char* pvy,const char* pvz)
{
  SetVector2Parameter (pvx,pvy);
  vz_par = pvz;
}

void celPropertySeqOpFactory::SetRelative (bool is_relative)
{
  relative = is_relative;
}
//---------------------------------------------------------------------------

celPropertySeqOp::celPropertySeqOp (
	celPropertySeqOpType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* pc_par, const char* tag_par,
	const char* prop_par, bool rel_par) 
	: scfImplementationType (this)
{
  celPropertySeqOp::type = type;

  pm = csQueryRegistryOrLoad<iParameterManager> 
    (type->object_reg, "cel.parameters.manager");

  entity_param = pm->GetParameter (params, entity_par);
  pcname_param = pm->GetParameter (params, pc_par);
  tag_param = pm->GetParameter (params, tag_par);
  
  relative = rel_par;

  propname_param = pm->GetParameter (params, prop_par);
}

celPropertySeqOp::~celPropertySeqOp ()
{
}

void celPropertySeqOp::FindPCProperty (iCelParameterBlock* params)
{
  if (prop_ok) return;

  iCelEntity* ent = pm->ResolveEntityParameter (type->pl, params, entity_param, 0);
  if (!ent) return;

  //Get current values of parameters
  pcname   = pcname_param->Get   (params);
  tag      = tag_param->Get      (params);
  propname = propname_param->Get (params);

  pc = ent->GetPropertyClassList()->FindByNameAndTag(pcname,tag);
  if (pc)
  {
    propID = type->pl->FetchStringID(propname);
    celDataType proptype = pc->GetPropertyOrActionType(propID);
    bool readonly = pc->IsPropertyReadOnly(propID);
    if ((proptype == celPropertySeqOp::proptype) && (!readonly))
    {
      prop_ok = 1;
      GetStartValue(params);
    }
  }
}

void celPropertySeqOp::Init (iCelParameterBlock* params)
{
  pc = 0;
  prop_ok = 0;
  FindPCProperty (params);
}

void celPropertySeqOp::Do (float time, iCelParameterBlock* params)
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
  	iCelParameterBlock* params,
	const char* entity_par, const char* pc_par, const char* tag_par,
	bool rel_par, const char* prop_par, const char* pfloat) 
        : celPropertySeqOp(type,params,entity_par,pc_par,tag_par,prop_par,
		rel_par)
{
  proptype = CEL_DATA_FLOAT;
  end_param = pm->GetParameter (params, pfloat);
}

//---------------------------------------------------------------------------
//--------- celVector2PropertySeqOp

celVector2PropertySeqOp::celVector2PropertySeqOp (
	celPropertySeqOpType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* pc_par, const char* tag_par,
	bool rel_par, const char* prop_par, const char* vx, const char* vy) 
        : celPropertySeqOp(type,params,entity_par,pc_par,tag_par,prop_par,
		rel_par)
{
  proptype = CEL_DATA_VECTOR2;
  endx_param = pm->GetParameter (params, vx);
  endy_param = pm->GetParameter (params, vy);
}

//---------------------------------------------------------------------------
//--------- celVector3PropertySeqOp

celVector3PropertySeqOp::celVector3PropertySeqOp (
	celPropertySeqOpType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* pc_par, const char* tag_par,
	bool rel_par,
	const char* prop_par, const char* vx, const char* vy, const char* vz) 
        : celPropertySeqOp(type,params,entity_par,pc_par,tag_par,prop_par,
		rel_par)
{
  proptype = CEL_DATA_VECTOR3;
  endx_param = pm->GetParameter (params, vx);
  endy_param = pm->GetParameter (params, vy);
  endz_param = pm->GetParameter (params, vz);
}

//---------------------------------------------------------------------------

