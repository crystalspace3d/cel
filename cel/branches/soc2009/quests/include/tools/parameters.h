/*
    Crystal Space Entity Layer
    Copyright (C) 2004-2006 by Jorrit Tyberghein
  
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

#ifndef __CEL_PARAMETERS__
#define __CEL_PARAMETERS__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/scf_implementation.h"
#include "csutil/weakref.h"
#include "csutil/hash.h"
#include "csgeom/vector3.h"
#include "csutil/refarr.h"
#include "physicallayer/pl.h"

class celVariableParameterBlock;
struct iCelDataBuffer;
struct iCelParameterBlock;
struct celParSpec
{
  celDataType type;
  csStringID id;
  csString name;
  csString value;
};

//---------------------------------------------------------------------------
/**
 * This interface represents a parameter to a sequence,
 * trigger, or reward. The paramater plugin provides
 * different types of parameters (constant, static parameters ($notation),
 * dynamic parameters (@notation), or expressions (=notation).
 * This interface makes abstraction of those.
 */
struct iParameter : public virtual iBase
{
  SCF_INTERFACE (iParameter, 0, 0, 2);

  /**
   * Get this variable as its correct datatype.
   */
  virtual const celData* GetData (iCelParameterBlock* params) = 0;

  /**
   * Get the value of this expression as a string.
   * \param params is an optional parameter block given to the reward.
   */
  virtual const char* Get (iCelParameterBlock* params) = 0;

  /**
   * Get the value of this expression as a string.
   * \param params is an optional parameter block given to the reward.
   * \param changed is set to true if the returned value is different
   * from the last time Get() was called. Note! This doesn't work
   * if you call the Get() with only one parameter above!
   */
  virtual const char* Get (iCelParameterBlock* params, bool& changed) = 0;

  /**
   * Get the value of this expression as a long.
   * \param params is an optional parameter block given to the reward.
   */
  virtual int32 GetLong (iCelParameterBlock* params) = 0;
};

//---------------------------------------------------------------------------
typedef csHash<csStringBase,csStringBase> celParams;
//---------------------------------------------------------------------------

/**
 * This interface provides a generic functions for
 * the evaluation of any type of parameter (constant, dynamic, 
 * or expression)
 */
struct iParameterManager : public virtual iBase
{
  /**
   * Get a parameter that can be evalulated later on an as-needed basis.
   * Returns 0 for an illegal parameter (error reporting has been done).
   */
  virtual csPtr<iParameter> GetParameter (
  	const celParams& params,
	const char* param) = 0;

  /**
   * This is a convenience function to resolve a quest parameter during
   * creation of rewards, triggers, and sequence operations. This routine
   * knows how to recognize parameter usage (starting with '$') and will in
   * that case try to resolve the parameter by finding it in 'params'. 
   * Otherwise it will just return the unmodified string. This version 
   * doesn't support dynamic parameters.
   */
  virtual const char* ResolveParameter (
  	const celParams& params,
	const char* param) = 0;

    /**
   * This is a convenience function to get a parameter block during
   * creation of rewards, triggers, and sequence operations. This routine
   * knows how to recognize parameter usage (starting with '$' or '@') and will in
   * that case try to resolve the parameter by finding it in 'params'.
   * \param params is the quest parameters.
   * \param paramspec is the parameter specifications and unparsed values.
   * \param quest_parameters is an array that should have the same length
   * as the 'paramspec' array. It will be filled with the parameters.
   */
  virtual csPtr<celVariableParameterBlock> GetParameterBlock (
  	const celParams& params,
	const csArray<celParSpec>& parameters,
	csRefArray<iParameter>& quest_parameters) = 0;

   /**
   * Fill in the dynamic parameters in a parameter block.
   * \param params is the parameter block given to the reward.
   * \param msg_params is the resolved parameter block as returned by
   * GetParameterBlock().
   * \param parameters is the parameter specifications and unparsed values.
   * \param quest_parameters is an array with quest parameters.
   */
  virtual void FillParameterBlock (
    iCelParameterBlock* params,
	celVariableParameterBlock* act_params,
	const csArray<celParSpec>& parameters,
	const csRefArray<iParameter>& quest_parameters) = 0;


};
//---------------------------------------------------------------------------

#endif // __CEL_PARAMETERS__

