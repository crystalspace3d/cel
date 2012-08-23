/*
    Crystal Space Entity Layer
    Copyright (C) 2011 by Jorrit Tyberghein

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

#ifndef __CEL_ADDON_BTLOAD__
#define __CEL_ADDON_BTLOAD__

#include "iutil/comp.h"
#include "imap/reader.h"
#include "csutil/strhash.h"
#include "csutil/weakref.h"

#include "tools/behaviourtree.h"

struct iObjectRegistry;
struct iDocumentNode;
struct iLoaderContext;
struct iSyntaxService;

/**
 * This is an add-on to allow loading of behaviour trees from XML.
 */
class celAddOnBehaviourTreeLoader : public scfImplementation2<
	celAddOnBehaviourTreeLoader, iLoaderPlugin, iComponent>
{
private:
  iObjectRegistry* object_reg;
  csRef<iPluginManager> plugin_mgr;
  csRef<iCelParameterBlock> params;

public:
  celAddOnBehaviourTreeLoader(iBase* parent);
  virtual ~celAddOnBehaviourTreeLoader ();

  /**
   * Initialize this plugin.
   */
  virtual bool Initialize (iObjectRegistry* object_reg);

  /**
   * Parses the behaviour tree.
   */
  virtual csPtr<iBase> Parse (iDocumentNode* node,
  	iStreamSource*, iLoaderContext* ldr_context,
  	iBase* context);

  virtual bool IsThreadSafe() { return false; }
};



#endif // __CEL_ADDON_BTLOAD__

