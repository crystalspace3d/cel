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

#ifndef __CEL_BLXML_BL__
#define __CEL_BLXML_BL__

#include "csutil/util.h"
#include "csutil/hash.h"
#include "csutil/hashhandlers.h"
#include "csutil/parray.h"
#include "csutil/csstring.h"
#include "iutil/comp.h"
#include "behaviourlayer/bl.h"
#include "behaviourlayer/blgen.h"

struct iObjectRegistry;
struct iCelEntity;
class celXmlScript;
class celXmlScriptEventHandler;

/**
 * This is the Behaviour Layer itself.
 */
class celBlXml : public iCelBlLayer, public iCelBlLayerGenerate
{
private:
  iObjectRegistry* object_reg;
  csPDelArray<celXmlScript> scripts;
  csHash<celXmlScript*,const char*,csConstCharHashKeyHandler> scripts_hash;
  csString last_error;

  void SetError (const char* error, ...);

  const char* ParseEventHandler (celXmlScriptEventHandler* h,
	iDocumentNode* node);

public:
  celBlXml (iBase* parent);
  virtual ~celBlXml ();
  bool Initialize (iObjectRegistry* object_reg);

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "blxml"; }
  virtual iCelBehaviour* CreateBehaviour (iCelEntity* entity, const char* name);

  virtual const char* CreateBehaviourScriptFromDoc (const char* name,
  	iDocumentNode* node);
  virtual const char* CreateBehaviourScriptFromString (const char* name,
  	const char* string);
  virtual const char* CreateBehaviourScriptFromFile (const char* name,
  	const char* filename);

  struct Component : public iComponent
  {
    SCF_DECLARE_EMBEDDED_IBASE (celBlXml);
    virtual bool Initialize (iObjectRegistry* p)
    { return scfParent->Initialize (p); }
  } scfiComponent;
};

#endif // __CEL_BLXML_BL__

