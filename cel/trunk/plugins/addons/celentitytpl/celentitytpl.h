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

#ifndef __CEL_ADDON_CELENTITYTPL__
#define __CEL_ADDON_CELENTITYTPL__

#include "iutil/comp.h"
#include "imap/reader.h"
#include "csutil/strhash.h"
#include "csutil/weakref.h"
#include "tools/entitytplloader.h"

struct iObjectRegistry;
struct iDocumentNode;
struct iLoaderContext;
struct iSyntaxService;
struct iCelPlLayer;

/**
 * This is an add-on to allow adding of cel entities through a standard
 * CS map file.
 */
class celAddOnCelEntityTemplate :
	public iLoaderPlugin,
	public iEntityTemplateLoader
{
private:
  iObjectRegistry* object_reg;
  csWeakRef<iSyntaxService> synldr;
  csWeakRef<iCelPlLayer> pl;
  csStringHash xmltokens;

  const char* GetAttributeValue (iDocumentNode* child,
	const char* propname);
  csStringID GetAttributeID (iDocumentNode* child,
	const char* prefix, const char* propname);
  bool ParseProperties (iCelPropertyClassTemplate* pc, iDocumentNode* child);

public:
  SCF_DECLARE_IBASE;
 
  celAddOnCelEntityTemplate (iBase* parent);
  virtual ~celAddOnCelEntityTemplate ();
  /**
   * Initialize this plugin. Register XML tokens for behaviour, propclass
   * and property.
   */
  bool Initialize (iObjectRegistry* object_reg);
  /**
   * Parses a document/script and assigns behaviour and/or property class to
   * an entity. If the context is not a mesh, a standalone entity will be
   * created. For meshes an additional pcmesh property class will be assigned.
   */
  virtual csPtr<iBase> Parse (iDocumentNode* node, iLoaderContext* ldr_context,
  	iBase* context);

  virtual iCelEntityTemplate* Load (iDocumentNode* node);

  struct Component : public iComponent
  {
    SCF_DECLARE_EMBEDDED_IBASE (celAddOnCelEntityTemplate);
    virtual bool Initialize (iObjectRegistry* p)
    { return scfParent->Initialize (p); }
  } scfiComponent;
};

#endif // __CEL_ADDON_CELENTITYTPL__

