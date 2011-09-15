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

#ifndef __CEL_TOOLS_SEQOP_PROPERTY__
#define __CEL_TOOLS_SEQOP_PROPERTY__

#include "csutil/util.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "tools/sequences.h"

#include "physicallayer/propclas.h"

struct iObjectRegistry;

/**
 * A standard seqop type that can animate a property class property.
 * Any property can be animated this way, assuming it is of the right type
 * and read write.
 *
 * This seqop type listens to the name 'cel.seqops.property'.
 */
CEL_DECLARE_SEQOPTYPE(Property,"cel.seqops.property")

/**
 * The 'property' seqop factory.
 * It will build different specializations of a property seqop depending
 * on the parameters read from xml, or set by user.
 */
class celPropertySeqOpFactory : public scfImplementation2<
	celPropertySeqOpFactory, iSeqOpFactory,
	iPropertySeqOpFactory>
{
private:
  csRef<celPropertySeqOpType> type;
  csString entity_par;
  csString pc_par;
  csString prop_par;
  csString tag_par;
  csString float_par;
  csString long_par;
  csString vx_par;
  csString vy_par;
  csString vz_par;
  bool relative;

public:
  celPropertySeqOpFactory (celPropertySeqOpType* type);
  virtual ~celPropertySeqOpFactory ();

  virtual csPtr<iSeqOp> CreateSeqOp (
      iCelParameterBlock* params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iPropertySeqOpFactory -----------------------
  virtual void SetEntityParameter (const char* entity);
  virtual void SetPCParameter (const char* pc, const char* tag = 0);
  virtual void SetPropertyParameter (const char* property_name);
  virtual void SetLongParameter (const char* plong);
  virtual void SetFloatParameter (const char* pfloat);
  virtual void SetVector2Parameter (const char* vectorx, const char* vectory);
  virtual void SetVector3Parameter (const char* vectorx, const char* vectory,
  	const char* vectorz);
  virtual void SetRelative (bool is_relative);
};

/**
 * Base property seqop.
 * Implements basic mechanisms, and provides hooks so type specific
 * implementations can be done.
 */
class celPropertySeqOp : public scfImplementation1<
	celPropertySeqOp, iSeqOp>
{
protected:
  csRef<celPropertySeqOpType> type;
  csString entity;
  csString pcname;
  csString propname;
  csString tag;
  bool relative;

  csRef<iParameter>  entity_param;
  csRef<iParameter>  pcname_param;
  csRef<iParameter>  propname_param;
  csRef<iParameter>  tag_param;

  csWeakRef<iCelPropertyClass> pc;
  celDataType proptype; // must be set in all specializations.

  bool prop_ok;
  csStringID propID;

  // function to find the pc in the specified entity, and check if the property
  // type is ok.
  void FindPCProperty (iCelParameterBlock* params);

public:
  celPropertySeqOp (celPropertySeqOpType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* pc_par, const char* tag_par,
	const char* prop_par, bool rel_par);
  virtual ~celPropertySeqOp ();
  virtual void Init (iCelParameterBlock* params);
  virtual void Do (float time, iCelParameterBlock* params);
  // We don't need to save or load anything as the sequence is totally
  // determined by the constructor and time passed (no internal data
  // structures to take care of).
  virtual bool Load (iCelDataBuffer* databuf) { return true; };
  virtual void Save (iCelDataBuffer* databuf) {};
  // virtual functions to get and set the value in type dependent way.
  // must be implemented by all specializations.
  virtual void SetCurrentValue(float time) = 0;
  virtual void GetStartValue(iCelParameterBlock* params) = 0;
};

/**
 * float property seqop.
 * also to be used for other 'single value' types, like long.
 */
class celFloatPropertySeqOp : public celPropertySeqOp
{
protected:
  float start;
  float end;
  float diff;

  csRef<iParameter> end_param;

  virtual void SetCurrentValue(float time) 
  { 
    pc->SetProperty(propID, start + (time*diff)); 
  }
  virtual void GetStartValue(iCelParameterBlock* params) 
  { 
    end = end_param->GetFloat (params);

    start = pc->GetPropertyFloatByID(propID);
    if (relative)
      end += start;
    diff = end - start;
  }
public:
  celFloatPropertySeqOp (celPropertySeqOpType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* pc_par, const char* tag_par, 
	bool rel_par, const char* prop_par, const char* pfloat);
};

/**
 * long property seqop.
 * uses celFloatPropertySeqOp as base type, as everything is the same
 * except actually reading and writing the property.
 */
class celLongPropertySeqOp : public celFloatPropertySeqOp
{
  virtual void SetCurrentValue(float time) 
  { 
    pc->SetProperty(propID, (long)(start + (time*diff))); 
  }
  virtual void GetStartValue(iCelParameterBlock* params) 
  { 
    end = end_param->GetFloat (params);

    start = (float)pc->GetPropertyLongByID(propID);
    if (relative)
      end += start;
    diff = end - start;
  }
public:
  celLongPropertySeqOp (celPropertySeqOpType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* pc_par, const char* tag_par,
	bool rel_par, const char* prop_par, const char* pfloat) 
        : celFloatPropertySeqOp(type, params, entity_par, pc_par, tag_par,
	                        rel_par, prop_par, pfloat)
  {
    proptype = CEL_DATA_LONG;
  }
};

/**
 * vector2 property seqop.
 */
class celVector2PropertySeqOp : public celPropertySeqOp
{
  csVector2 start;
  csVector2 end;
  csVector2 diff;

  csRef<iParameter> endx_param;
  csRef<iParameter> endy_param;

  virtual void SetCurrentValue(float time) 
  { 
    pc->SetProperty(propID, start + (time*diff)); 
  }
  virtual void GetStartValue(iCelParameterBlock* params) 
  { 
    end.x = endx_param->GetFloat (params);
    end.y = endy_param->GetFloat (params);

    pc->GetPropertyVectorByID(propID,start);
    if (relative)
      end += start;
    diff = end - start;
  }
public:
  celVector2PropertySeqOp (celPropertySeqOpType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* pc_par, const char* tag_par,
	bool rel_par, const char* prop_par, const char* vx, const char* vy);
};

/**
 * vector3 property seqop.
 */
class celVector3PropertySeqOp : public celPropertySeqOp
{
  csVector3 start;
  csVector3 end;
  csVector3 diff;

  csRef<iParameter> endx_param;
  csRef<iParameter> endy_param;
  csRef<iParameter> endz_param;

  virtual void SetCurrentValue(float time) 
  { 
    pc->SetProperty(propID, start + (time*diff)); 
  }
  virtual void GetStartValue(iCelParameterBlock* params) 
  { 
    end.x = endy_param->GetFloat (params);
    end.y = endy_param->GetFloat (params);
    end.z = endy_param->GetFloat (params);

    pc->GetPropertyVectorByID(propID,start);
    if (relative)
      end += start;
    diff = end - start;
  }
public:
  celVector3PropertySeqOp (celPropertySeqOpType* type,
  	iCelParameterBlock* params,
	const char* entity_par, const char* pc_par, const char* tag_par,
	bool rel_par, const char* prop_par, const char* vx, const char* vy, 
	const char* vz);
};

#endif // __CEL_TOOLS_SEQOP_PROPERTY__

