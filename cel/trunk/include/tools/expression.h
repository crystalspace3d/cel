/*
    Crystal Space Entity Layer
    Copyright (C) 2006 by Jorrit Tyberghein
  
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

#ifndef __CEL_TOOLS_EXPRESSION__
#define __CEL_TOOLS_EXPRESSION__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/stringarray.h"

#include "physicallayer/datatype.h"

class csVector2;
class csVector3;
class csColor;

/**
 * Interface for a single expression.
 */
struct iCelExpression : virtual public iBase
{
  SCF_INTERFACE (iCelExpression, 0, 0, 1);

  /**
   * Execute this expression.
   * Return the result of the expression in 'ret'.
   * \param entity is an optional entity context.
   * \return false if there is an error.
   */
  virtual bool Execute (iCelEntity* entity, celData& ret) = 0;

  /**
   * Find a local variable for this expression.
   * Returns csArrayItemNotFound if local variable is not present.
   */
  virtual size_t FindLocalVariable (const char* name) const = 0;

  /**
   * Get an array of local variables.
   */
  virtual const csStringArray& GetLocalVariables () const = 0;

  /// Set the value of a local variable. Do this before calling Execute().
  virtual void SetLocalVariableLong (size_t idx, int32 value) = 0;
  /// Set the value of a local variable. Do this before calling Execute().
  virtual void SetLocalVariableULong (size_t idx, uint32 value) = 0;
  /// Set the value of a local variable. Do this before calling Execute().
  virtual void SetLocalVariableFloat (size_t idx, float value) = 0;
  /// Set the value of a local variable. Do this before calling Execute().
  virtual void SetLocalVariableBool (size_t idx, bool value) = 0;
  /// Set the value of a local variable. Do this before calling Execute().
  virtual void SetLocalVariableString (size_t idx, const char* value) = 0;
  /// Set the value of a local variable. Do this before calling Execute().
  virtual void SetLocalVariableVector2 (size_t idx, const csVector2& value) = 0;
  /// Set the value of a local variable. Do this before calling Execute().
  virtual void SetLocalVariableVector3 (size_t idx, const csVector3& value) = 0;
  /// Set the value of a local variable. Do this before calling Execute().
  virtual void SetLocalVariableColor (size_t idx, const csColor& value) = 0;
};


/**
 * Interface to the expression parser plugin (same as the blxml
 * behaviour layer plugin).
 */
struct iCelExpressionParser : virtual public iBase
{
  SCF_INTERFACE (iCelExpressionParser, 0, 0, 1);

  /**
   * Parse an expression from a given string and return
   * a parsed expression.
   */
  virtual csPtr<iCelExpression> Parse (const char* string) = 0;
};

#endif // __CEL_TOOLS_EXPRESSION__

