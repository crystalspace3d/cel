/*
    Crystal Space Entity Layer
    Copyright (C) 2003 by Jorrit Tyberghein
  
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

#ifndef __CEL_ADDON_XMLSCRIPTS__
#define __CEL_ADDON_XMLSCRIPTS__

#include "iutil/comp.h"
#include "imap/reader.h"
#include "csutil/strhash.h"

struct iObjectRegistry;
struct iDocumentNode;
struct iLoaderContext;
struct iSyntaxService;
struct iCelBlLayer;
struct iCelBlLayerGenerate;

/**
 * This is an add-on to allow adding of cel entities through a standard
 * CS map file.
 */
class celAddOnXmlScripts : public iLoaderPlugin
{
private:
  iObjectRegistry* object_reg;
  csRef<iSyntaxService> synldr;
  csRef<iCelBlLayer> bl;
  csRef<iCelBlLayerGenerate> blgen;
  csStringHash xmltokens;

  void GetBlGen (iDocumentNode* child);

public:
  SCF_DECLARE_IBASE;
 
  celAddOnXmlScripts (iBase* parent);
  virtual ~celAddOnXmlScripts ();
  /**
   * Initialize this plugin. Register XML tokens for script, layer and pcfactory.
   */
  bool Initialize (iObjectRegistry* object_reg);
  /**
   * Parses a document/script for addons defining a pcfactory, a behaviour
   * layer and/or behaviour xmlscripts.
   */
  virtual csPtr<iBase> Parse (iDocumentNode* node, iLoaderContext* ldr_context,
  	iBase* context);

  struct Component : public iComponent
  {
    SCF_DECLARE_EMBEDDED_IBASE (celAddOnXmlScripts);
    virtual bool Initialize (iObjectRegistry* p)
    { return scfParent->Initialize (p); }
  } scfiComponent;
};

#endif // __CEL_ADDON_XMLSCRIPTS__

