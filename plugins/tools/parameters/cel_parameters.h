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

#ifndef __CEL_TOOLS_PARAMETERS__
#define __CEL_TOOLS_PARAMETERS__

#include "iutil/comp.h"
#include "tools/expression.h"
#include "tools/parameters.h"

#include "behaviourlayer/behave.h"


struct iObjectRegistry;


class celParameterManager : public scfImplementation2<celParameterManager,
	iParameterManager, iComponent>
{
private:
  iObjectRegistry* object_reg;
  csWeakRef<iCelPlLayer> pl;
  csRef<iCelExpressionParser> expparser;
  iCelExpressionParser* GetParser ();

  csString str;	// Temporary string returned by ResolveParameter().

public:
  celParameterManager (iBase* parent) : 
	  scfImplementationType (this, parent), object_reg(0) { }
  virtual ~celParameterManager() {}

  // From iComponent.
  virtual bool Initialize (iObjectRegistry*);

  // From iParameterManager
  virtual csPtr<iParameter> GetParameter (iCelParameterBlock* params, const char* param);
  virtual const char* ResolveParameter (iCelParameterBlock* params, const char* param);
  virtual csPtr<celVariableParameterBlock> GetParameterBlock (
  	iCelParameterBlock* params,
	const csArray<celParSpec>& parameters,
	csRefArray<iParameter>& quest_parameters);
  virtual bool FillParameterBlock (
    iCelParameterBlock* params,
	celVariableParameterBlock* act_params,
	const csArray<celParSpec>& parameters,
	const csRefArray<iParameter>& quest_parameters);
};
//---------------------------------------------------------------------------

class celConstantParameter : public scfImplementation1<celConstantParameter,
  iParameter>
{
private:
  celData data;
  csString str;	// This string is used to hold temporary conversion to string.

public:
  celConstantParameter () : scfImplementationType (this) { }
  celConstantParameter (const char* c) : scfImplementationType (this)
  {
    data.Set (c);
  }
  virtual ~celConstantParameter () { }

  // From iParamater
  virtual const celData* GetData (iCelParameterBlock*)
  {
    return &data;
  }
  virtual const char* Get (iCelParameterBlock*);
  virtual const char* Get (iCelParameterBlock*, bool& changed)
  {
    changed = false;
    return Get (0);
  }
  virtual int32 GetLong (iCelParameterBlock*);
  virtual float GetFloat (iCelParameterBlock*);
};

//---------------------------------------------------------------------------

class celDynamicParameter : public scfImplementation1<celDynamicParameter,
  iParameter>
{
private:
  iObjectRegistry* object_reg;
  csStringID dynamic_id;
  csString parname;
  csString oldvalue;
  csString str;	// This string is used to hold temporary conversion to string.

public:
  celDynamicParameter (iObjectRegistry* object_reg, csStringID dynamic_id,
      const char* parname) : scfImplementationType (this), object_reg (object_reg),
      dynamic_id (dynamic_id), parname (parname) { }
  virtual ~celDynamicParameter () { }

  // From iParamater
  virtual const char* Get (iCelParameterBlock* params);
  virtual const char* Get (iCelParameterBlock* params, bool& changed);
  virtual const celData* GetData (iCelParameterBlock* params);
  virtual int32 GetLong (iCelParameterBlock* params);
  virtual float GetFloat (iCelParameterBlock*);
};

//---------------------------------------------------------------------------

class celExpressionParameter : public scfImplementation1<celExpressionParameter,
  iParameter>
{
private:
  iObjectRegistry* object_reg;
  iCelEntity* entity;
  celData data;
  csRef<iCelExpression> expression;
  csString parname;
  csString oldvalue;
  csString str;	// This string is used to hold temporary conversion to string.

public:
  celExpressionParameter (iObjectRegistry* object_reg, iCelEntity* entity,
      iCelExpression* expression, const char* parname)
    : scfImplementationType (this), object_reg (object_reg),
      entity (entity), expression (expression), parname (parname) { }
  virtual ~celExpressionParameter () { }

  // From iParamater
  virtual const char* Get (iCelParameterBlock* params);
  virtual const char* Get (iCelParameterBlock* params, bool& changed);
  virtual const celData* GetData (iCelParameterBlock* params);
  virtual int32 GetLong (iCelParameterBlock* params);
  virtual float GetFloat (iCelParameterBlock*);
};

//---------------------------------------------------------------------------

#endif // __CEL_TOOLS_PARAMETERS__

