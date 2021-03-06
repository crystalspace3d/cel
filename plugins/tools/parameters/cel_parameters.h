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
  csWeakRef<iCelExpressionParser> expparser;
  iCelExpressionParser* GetParser ();
  bool rememberExpression;

  csString str;	// Temporary string returned by ResolveParameter().

  void ResolveParameterData (
    celData& out,
    iCelParameterBlock* params,
    const char* param);

public:
  celParameterManager (iBase* parent) : 
	  scfImplementationType (this, parent), object_reg (0),
	  rememberExpression (false) { }
  virtual ~celParameterManager() {}

  // From iComponent.
  virtual bool Initialize (iObjectRegistry*);

  // From iParameterManager
  virtual csPtr<iParameter> GetParameter (iCelParameterBlock* params,
      const char* param, celDataType type = CEL_DATA_NONE);
  virtual csPtr<iParameter> GetParameter (const char* param,
      celDataType type = CEL_DATA_NONE);
  virtual const char* ResolveParameter (iCelParameterBlock* params,
      const char* param);
  virtual const char* ResolveEntityParameter (
      iCelParameterBlock* params,
      const char* param,
      uint& entid);

  virtual iCelEntity* ResolveEntityParameter (
      iCelPlLayer* pl,
      iCelParameterBlock* params, iParameter* param,
      iCelEntity* ent = 0);
  virtual csRef<iCelEntityList> ResolveEntityListParameter (
      iCelPlLayer* pl,
      iCelParameterBlock* params, iParameter* param);

  virtual void SetRememberExpression (bool remember) { rememberExpression = remember; }
  virtual bool IsRememberingExpressions () const { return rememberExpression; }
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
  celConstantParameter (const char* c, celDataType type);
  celConstantParameter (const celData& in, celDataType type);
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
  virtual bool GetBool (iCelParameterBlock*);
  virtual const char* GetOriginalExpression ()
  {
    return Get (0);
  }
  virtual celDataType GetPossibleType () const { return data.type; }
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
  celData converted;
  celDataType desiredType;
  char prefix;

public:
  celDynamicParameter (iObjectRegistry* object_reg, csStringID dynamic_id,
      const char* parname, celDataType desiredType, char prefix) :
    scfImplementationType (this), object_reg (object_reg),
    dynamic_id (dynamic_id), parname (parname), desiredType (desiredType),
    prefix (prefix) { }
  virtual ~celDynamicParameter () { }

  // From iParamater
  virtual const char* Get (iCelParameterBlock* params);
  virtual const char* Get (iCelParameterBlock* params, bool& changed);
  virtual const celData* GetData (iCelParameterBlock* params);
  virtual int32 GetLong (iCelParameterBlock* params);
  virtual float GetFloat (iCelParameterBlock*);
  virtual bool GetBool (iCelParameterBlock*);
  virtual const char* GetOriginalExpression ()
  {
    str.Empty ();
    str += prefix;
    str += parname;
    return str;
  }
  virtual celDataType GetPossibleType () const { return desiredType; }
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
  celDataType desiredType;
  csString expressionString;	// Used to remember the expression.

public:
  celExpressionParameter (iObjectRegistry* object_reg, iCelEntity* entity,
      iCelExpression* expression, const char* parname, celDataType desiredType)
    : scfImplementationType (this), object_reg (object_reg),
      entity (entity), expression (expression), parname (parname),
      desiredType (desiredType) { }
  virtual ~celExpressionParameter () { }

  // From iParamater
  virtual const char* Get (iCelParameterBlock* params);
  virtual const char* Get (iCelParameterBlock* params, bool& changed);
  virtual const celData* GetData (iCelParameterBlock* params);
  virtual int32 GetLong (iCelParameterBlock* params);
  virtual float GetFloat (iCelParameterBlock*);
  virtual bool GetBool (iCelParameterBlock*);
  void SetOriginalExpression (const char* exp)
  {
    expressionString = exp;
  }
  virtual const char* GetOriginalExpression ()
  {
    return expressionString;
  }
  virtual celDataType GetPossibleType () const { return desiredType; }
};

//---------------------------------------------------------------------------

#endif // __CEL_TOOLS_PARAMETERS__

