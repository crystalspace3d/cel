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

#ifndef __CEL_ADDON_RULEDEF__
#define __CEL_ADDON_RULEDEF__

#include "iutil/comp.h"
#include "imap/reader.h"
#include "csutil/strhash.h"

struct iObjectRegistry;
struct iDocumentNode;
struct iLoaderContext;
struct iSyntaxService;
struct iCelExpressionParser;

/**
 * This is an add-on to allow adding of rules through a standard
 * CS map file.
 */
class celAddOnRuleDef : public scfImplementation2<
	celAddOnRuleDef, iLoaderPlugin, iComponent>
{
private:
  iObjectRegistry* object_reg;
  csRef<iSyntaxService> synldr;
  csStringHash xmltokens;

  iCelExpressionParser* GetParser ();

public:
  celAddOnRuleDef (iBase* parent);
  virtual ~celAddOnRuleDef ();
  /**
   * Initialize this plugin. Register XML tokens.
   */
  virtual bool Initialize (iObjectRegistry* object_reg);

  virtual csPtr<iBase> Parse (iDocumentNode* node,
  	iStreamSource*, iLoaderContext* ldr_context,
  	iBase* context);
};

#endif // __CEL_ADDON_RULEDEF__

