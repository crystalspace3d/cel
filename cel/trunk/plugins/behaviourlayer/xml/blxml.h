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
#include "csutil/strhash.h"
#include "iutil/comp.h"
#include "behaviourlayer/bl.h"
#include "behaviourlayer/blgen.h"
#include "plugins/behaviourlayer/xml/xmlscript.h"

struct iObjectRegistry;
struct iCelEntity;
struct iSyntaxService;
struct iCelPlLayer;
class celXmlScript;
class celXmlScriptEventHandler;

/**
 * This is the Behaviour Layer itself.
 */
class celBlXml : public iCelBlLayer, public iCelBlLayerGenerate
{
private:
  iObjectRegistry* object_reg;
  csRef<iSyntaxService> synldr;
  csRef<iCelPlLayer> pl;
  csPDelArray<celXmlScript> scripts;
  csHash<celXmlScript*,csStrKey,csConstCharHashKeyHandler> scripts_hash;
  csStringHash xmltokens;
  csStringHash functions;

  const char* GetAttributeValue (iDocumentNode* child,
	const char* propname);
  const char* GetAttributeString (iDocumentNode* child,
	const char* attrname, const char* parentname);
  bool ParseEventHandler (celXmlScriptEventHandler* h, iDocumentNode* node,
  	celXmlScript* script);

  bool SkipComma (const char*& input, iDocumentNode* child,
	const char* name);
  bool ParseID (const char*& input, iDocumentNode* child,
	celXmlScriptEventHandler* h,
	const char* name, char* str, csStringID fun_id);
  bool ParseExpression (iDocumentNode* child,
	celXmlScriptEventHandler* h, const char* attrname, const char* name,
	int optional_type = CEL_DATA_NONE);
  bool ParseFunction (const char*& input, const char* pinput,
  	iDocumentNode* child, celXmlScriptEventHandler* h, const char* name);
  bool ParseExpression (const char*& input, iDocumentNode* child,
  	celXmlScriptEventHandler* h, const char* name, int stoppri);
  celXmlScriptEventHandler* FindEventHandler (celXmlScript* script,
  	const char* eventname);

public:
  celBlXml (iBase* parent);
  virtual ~celBlXml ();
  bool Initialize (iObjectRegistry* object_reg);

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "blxml"; }
  virtual iCelBehaviour* CreateBehaviour (iCelEntity* entity, const char* name);

  virtual bool CreateBehaviourScriptFromDoc (const char* name,
  	iDocumentNode* node);
  virtual bool CreateBehaviourScriptFromString (const char* name,
  	const char* string);
  virtual bool CreateBehaviourScriptFromFile (const char* name,
  	const char* filename);

  struct Component : public iComponent
  {
    SCF_DECLARE_EMBEDDED_IBASE (celBlXml);
    virtual bool Initialize (iObjectRegistry* p)
    { return scfParent->Initialize (p); }
  } scfiComponent;
};

#endif // __CEL_BLXML_BL__

