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

#ifndef __CEL_RULEBASE__
#define __CEL_RULEBASE__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/ref.h"
#include "csutil/hash.h"
#include "csutil/csstring.h"

struct iCelExpression;

/**
 * A rule.
 */
struct iCelRule : public virtual iBase
{
  SCF_INTERFACE (iCelRule, 0, 0, 1);

  /**
   * Get the name of this rule.
   */
  virtual const char* GetName () const = 0;

  /**
   * Get the name of the variable that this rule will influence.
   */
  virtual const char* GetVariable () const = 0;

  /**
   * Set the name of the variable that this rule will influence.
   */
  virtual void SetVariable (const char* var) = 0 ;

  /**
   * Get the index of the local variable that will be used by
   * the expression for the variable that this rule influences.
   */
  virtual size_t GetVariableIndex () = 0;

  /**
   * Get the expression corresponding with this rule.
   */
  virtual iCelExpression* GetExpression () = 0;

  /**
   * Set the expression corresponding with this rule.
   */
  virtual void SetExpression (iCelExpression* expr) = 0;

  /**
   * Get the priority ID for this rule.
   */
  virtual csStringID GetPriority () = 0;

  /**
   * Set the priority ID for this rule.
   */
  virtual void SetPriority (csStringID priority) = 0;

  /**
   * Get a priority number as defined by the rule base.
   */
  virtual size_t GetPriorityNumber () const = 0;
};

/// Hash of rules.
typedef csHash<csRef<iCelRule>,csStringBase> celRuleHash;

/**
 * Interface for the Rule System.
 */
struct iCelRuleBase : public virtual iBase
{
  SCF_INTERFACE (iCelRuleBase, 0, 0, 1);

  /**
   * Create a new rule.
   */
  virtual iCelRule* CreateRule (const char* name) = 0;

  /**
   * Find a rule by name.
   */
  virtual iCelRule* FindRule (const char* name) = 0;

  /**
   * Iterate over the rules.
   */
  virtual const celRuleHash& GetRules () const = 0;

  /**
   * Delete a rule.
   */
  virtual void DeleteRule (iCelRule* rule) = 0;

  /**
   * Clear priority table.
   */
  virtual void ClearPriorityTable () = 0;

  /**
   * Add a priority. Call this function as many times as you want.
   * Every new priority you use here will get a higher priority then
   * the previous priorities.
   */
  virtual void AddPriority (csStringID priority) = 0;
};

#endif // __CEL_RULEBASE__

