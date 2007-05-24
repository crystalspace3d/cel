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

#include "cssysdef.h"
#include "csutil/objreg.h"
#include "csutil/event.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/plugin.h"

#include "tools/expression.h"
#include "plugins/tools/rulebase/rulebase.h"

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celRuleBase)

//--------------------------------------------------------------------------

size_t celRule::GetVariableIndex ()
{
  if (var_idx == csArrayItemNotFound)
  {
    var_idx = expr->FindLocalVariable (var);
  }
  return var_idx;
}

size_t celRule::GetPriorityNumber () const
{
  return rulebase->GetPriorityNumber (priority);
}

//--------------------------------------------------------------------------

celRuleBase::celRuleBase (iBase* parent)
  : scfImplementationType (this, parent)
{
}

celRuleBase::~celRuleBase ()
{
}

bool celRuleBase::Initialize (iObjectRegistry* object_reg)
{
  celRuleBase::object_reg = object_reg;

  return true;
}

iCelPlLayer* celRuleBase::GetPL ()
{
  if (!pl)
  {
    pl = csQueryRegistry<iCelPlLayer> (object_reg);
    // @@@ Error
  }
  return pl;
}

iCelExpressionParser* celRuleBase::GetParser ()
{
  if (parser) return parser;
  csRef<iObjectRegistryIterator> it = object_reg->Get (
      scfInterfaceTraits<iCelExpressionParser>::GetID (),
      scfInterfaceTraits<iCelExpressionParser>::GetVersion ());
  iBase* b = it->Next ();
  if (b)
  {
    parser = scfQueryInterface<iCelExpressionParser> (b);
  }
  if (!parser)
  {
    csRef<iPluginManager> plugmgr = csQueryRegistry<iPluginManager> (
	object_reg);
    parser = CS_LOAD_PLUGIN (plugmgr, "cel.behaviourlayer.xml",
	iCelExpressionParser);
    if (!parser)
    {
      // @@@ Error
      return 0;
    }
    object_reg->Register (parser, "iCelExpressionParser");
  }
  return parser;
}

iCelRule* celRuleBase::CreateRule (const char* name)
{
  celRule* rule = new celRule (this, name);
  rules.Put (name, rule);
  rule->DecRef ();
  return rule;
}

void celRuleBase::DeleteRule (iCelRule* rule)
{
  rules.Delete (rule->GetName (), rule);
}

void celRuleBase::ClearPriorityTable ()
{
  priorities.DeleteAll ();
  priority_hash.DeleteAll ();
}

void celRuleBase::AddPriority (csStringID priority)
{
  size_t idx = priorities.Push (priority);
  priority_hash.Put (priority, idx);
}

//---------------------------------------------------------------------------
