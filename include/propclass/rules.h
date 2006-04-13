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

#ifndef __CEL_PF_RULES__
#define __CEL_PF_RULES__

#include "cstypes.h"
#include "csutil/scf.h"

#include "tools/rulebase.h"

SCF_VERSION (iPcRules, 0, 0, 1);

/**
 * This property class represents a set of active rules for an entity.
 * It uses the rule base system (iCelRuleBase). Note that this property class
 * will automatically load the rule base system if it is missing.
 * When reading properties out of this pcrules it will check if there are
 * any rules affecting the property. If not you will simply get the value
 * from the associated 'pcproperties' property class (if present). Otherwise
 * you will get a modified value according to the rules defined for this
 * property (starting from the value out of pcproperties). If the 'pcproperties'
 * is not present a default value is assumed (0 for numbers, empty string
 * for string, 0-vector for vectors, and black for colors).
 * <p>
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * <ul>
 * <li>AddRule: parameters 'name' (string).
 * <li>DeleteRule: parameters 'name' (string).
 * <li>DeleteAllRules: no parameters.
 * </ul>
 */
struct iPcRules : public iBase
{
  /**
   * Add a rule.
   */
  virtual void AddRule (iCelRule* rule) = 0;

  /**
   * Delete a rule.
   */
  virtual void DeleteRule (iCelRule* rule) = 0;

  /**
   * Delete all rules.
   */
  virtual void DeleteAllRules () = 0;

  /**
   * Get the type of the property. This will base itself on
   * the underlying pcproperties. If there is no pcproperties class
   * with the right property in it then this will return CEL_DATA_NONE
   * even though getting a specific typed value might work.
   */
  virtual celDataType GetPropertyType (const char* name) = 0;

  /// Get a specific property.
  virtual bool GetProperty (const char* name, celData& ret) = 0;

  /// Get a specific property.
  virtual float GetPropertyFloat (const char* name) = 0;
  /// Get a specific property.
  virtual long GetPropertyLong (const char* name) = 0;
  /// Get a specific property.
  virtual bool GetPropertyBool (const char* name) = 0;
  /// Get a specific property.
  virtual bool GetPropertyVector (const char* name, const csVector2& v) = 0;
  /// Get a specific property.
  virtual bool GetPropertyVector (const char* name, const csVector3& v) = 0;
  /// Get a specific property.
  virtual bool GetPropertyColor (const char* name, const csColor& v) = 0;
  /// Get a specific property.
  virtual const char* GetPropertyString (const char* name) = 0;
};

#endif // __CEL_PF_RULES__

