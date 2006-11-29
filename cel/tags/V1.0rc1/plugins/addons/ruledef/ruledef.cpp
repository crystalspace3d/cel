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
#include "csutil/plugmgr.h"
#include "cstool/initapp.h"
#include "iutil/objreg.h"
#include "iutil/document.h"
#include "iutil/object.h"
#include "ivaria/reporter.h"
#include "imap/services.h"
#include "iengine/mesh.h"

#include "physicallayer/pl.h"
#include "propclass/mesh.h"
#include "tools/rulebase.h"
#include "tools/expression.h"

#include "ruledef.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celAddOnRuleDef)

SCF_IMPLEMENT_IBASE (celAddOnRuleDef)
  SCF_IMPLEMENTS_INTERFACE (iLoaderPlugin)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celAddOnRuleDef::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END


enum
{
  XMLTOKEN_RULE,
  XMLTOKEN_PRIORITIES,
  XMLTOKEN_PRIORITY
};


celAddOnRuleDef::celAddOnRuleDef (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
  object_reg = 0;
}

celAddOnRuleDef::~celAddOnRuleDef ()
{
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiComponent);
  SCF_DESTRUCT_IBASE ();
}

bool celAddOnRuleDef::Initialize (iObjectRegistry* object_reg)
{
  celAddOnRuleDef::object_reg = object_reg;
  synldr = CS_QUERY_REGISTRY (object_reg, iSyntaxService);
  if (!synldr)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"cel.addons.ruledef",
    	"Can't find syntax services!");
    return false;
  }

  xmltokens.Register ("rule", XMLTOKEN_RULE);
  xmltokens.Register ("priorities", XMLTOKEN_PRIORITIES);
  xmltokens.Register ("priority", XMLTOKEN_PRIORITY);

  return true;
}

iCelExpressionParser* celAddOnRuleDef::GetParser ()
{
  csRef<iObjectRegistryIterator> it = object_reg->Get (
      scfInterfaceTraits<iCelExpressionParser>::GetID (),
      scfInterfaceTraits<iCelExpressionParser>::GetVersion ());
  iBase* b = it->Next ();
  csRef<iCelExpressionParser> parser;
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
      // @@@ Error report.
      return 0;
    }
    object_reg->Register (parser, "iCelExpressionParser");
  }
  return parser;
}

csPtr<iBase> celAddOnRuleDef::Parse (iDocumentNode* node,
	iStreamSource*, iLoaderContext* ldr_context, iBase*)
{
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  csRef<iCelRuleBase> rulebase = csQueryRegistryOrLoad<iCelRuleBase> (
  	object_reg, "cel.rulebase");
  if (!rulebase)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
    	"cel.addons.ruledef",
    	"Can't load the rule base plugin!");
    return 0;
  }
  iCelExpressionParser* parser = GetParser ();

  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    csStringID id = xmltokens.Request (value);
    switch (id)
    {
      case XMLTOKEN_PRIORITIES:
        {
          rulebase->ClearPriorityTable ();
          csRef<iDocumentNodeIterator> child_it = child->GetNodes ();
          while (child_it->HasNext ())
          {
            csRef<iDocumentNode> child_child = child_it->Next ();
            if (child_child->GetType () != CS_NODE_ELEMENT) continue;
            const char* child_value = child_child->GetValue ();
            csStringID child_id = xmltokens.Request (child_value);
            if (child_id == XMLTOKEN_PRIORITY)
            {
              const char* name = child_child->GetAttributeValue ("name");
              rulebase->AddPriority (pl->FetchStringID (name));
            }
            else
            {
              synldr->ReportBadToken (child_child);
              return 0;
            }
          }
        }
        break;
      case XMLTOKEN_RULE:
        {
          const char* name = child->GetAttributeValue ("name");
          if (!name)
          {
            synldr->ReportError (
            	"cel.addons.ruledef",
            	child, "'name' attribute for the rule is missing!");
            return 0;
          }
          const char* priority = child->GetAttributeValue ("priority");
          const char* var = child->GetAttributeValue ("var");
          if (!var)
          {
            synldr->ReportError (
            	"cel.addons.ruledef",
            	child, "'var' attribute for rule '%s' is missing!", name);
            return 0;
          }
          const char* expr = child->GetAttributeValue ("expr");
          if (!expr)
          {
            synldr->ReportError (
            	"cel.addons.ruledef",
            	child, "'expr' attribute for rule '%s' is missing!", name);
            return 0;
          }
          iCelRule* rule = rulebase->CreateRule (name);
          rule->SetVariable (var);
          rule->SetPriority (pl->FetchStringID (priority));
          csRef<iCelExpression> expression = parser->Parse (expr);
          if (!expression)
          {
            synldr->ReportError (
            	"cel.addons.ruledef",
            	child, "Error parsing expression for rule '%s'!", name);
            return 0;
          }
          rule->SetExpression (expression);
        }
        break;
      default:
        synldr->ReportBadToken (child);
        return 0;
    }
  }
  // Just return something to indicate success.
  IncRef ();
  return csPtr<iBase> (this);
}

