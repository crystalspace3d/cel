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

#include "physicallayer/datatype.h"

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

