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

#ifndef __CEL_TOOLS_RULEBASE__
#define __CEL_TOOLS_RULEBASE__

#include "csutil/util.h"
#include "csutil/hash.h"
#include "csutil/weakrefarr.h"
#include "csutil/stringarray.h"
#include "csutil/csstring.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"

#include "physicallayer/pl.h"
#include "tools/rulebase.h"
#include "tools/expression.h"

/**
 * A rule.
 */
class celRule : public scfImplementation1<celRule, iCelRule>
{
private:
  csString name;
  csString var;
  csRef<iCelExpression> expr;
  csStringID priority;

public:
  celRule (const char* name) : scfImplementationType (this), name (name)
  {
    priority = csArrayItemNotFound;
  }
  virtual ~celRule () { }

  virtual const char* GetName () const { return name; }
  virtual const char* GetVariable () const { return var; }
  virtual void SetVariable (const char* var)
  {
    celRule::var = var;
  }
  virtual iCelExpression* GetExpression () { return expr; }
  virtual void SetExpression (iCelExpression* expr)
  {
    celRule::expr = expr;
  }
  virtual csStringID GetPriority () { return priority; }
  virtual void SetPriority (csStringID priority)
  {
    celRule::priority = priority;
  }
};

/**
 * This is the rule system for CEL. Basically it is a collection
 * of rules (or expressions). The 'pcrules' property class works
 * closely together with this.
 */
class celRuleBase : public scfImplementation2<
		   celRuleBase, iCelRuleBase, iComponent>
{
private:
  iObjectRegistry* object_reg;
  csRef<iCelPlLayer> pl;
  csRef<iCelExpressionParser> parser;

  celRuleHash rules;

  iCelPlLayer* GetPL ();
  iCelExpressionParser* GetParser ();

public:
  celRuleBase (iBase* parent);
  virtual ~celRuleBase ();
  virtual bool Initialize (iObjectRegistry* object_reg);

  virtual iCelRule* CreateRule (const char* name);
  virtual iCelRule* FindRule (const char* name) { return rules.Get (name, 0); }
  virtual const celRuleHash& GetRules () const { return rules; }
  virtual void DeleteRule (iCelRule* rule);
};

#endif // __CEL_TOOLS_RULEBASE__

