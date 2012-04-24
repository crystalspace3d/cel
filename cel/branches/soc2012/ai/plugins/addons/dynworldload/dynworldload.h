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

#ifndef __CEL_ADDON_DYNWORLDLOAD__
#define __CEL_ADDON_DYNWORLDLOAD__

#include "csutil/scf.h"
#include "csutil/scf_implementation.h"
#include "iutil/comp.h"
#include "iengine/engine.h"
#include "imap/reader.h"
#include "imap/writer.h"
#include "csutil/strhash.h"

#include "physicallayer/pl.h"
#include "propclass/dynworld.h"
#include "tools/dynworldload.h"

struct iDynamicFactory;

class celAddOnDynamicWorldLoader : public scfImplementation3<celAddOnDynamicWorldLoader,
  iLoaderPlugin, iSaverPlugin, iComponent>
{
private:
  iObjectRegistry* object_reg;
  csRef<iCelPlLayer> pl;
  csRef<iPcDynamicWorld> dynworld;
  csRef<iSyntaxService> synldr;
  csRef<iEngine> engine;
  csStringHash xmltokens;
  csRefArray<iDynamicWorldLoaderExtension> extensions;

  bool ParseFactory (iDocumentNode* node);
  bool WriteBodies (iDocumentNode* factNode, iDynamicFactory* fact);
  bool WritePivots (iDocumentNode* factNode, iDynamicFactory* fact);
  bool WriteJoints (iDocumentNode* factNode, iDynamicFactory* fact);

public:
  celAddOnDynamicWorldLoader (iBase *iParent);
  virtual ~celAddOnDynamicWorldLoader ();
  virtual bool Initialize (iObjectRegistry *object_reg);

  virtual csPtr<iBase> Parse (iDocumentNode* node,
  	iStreamSource* ssource, iLoaderContext* ldr_context,
  	iBase* context);

  virtual bool IsThreadSafe() { return false; }

  virtual bool WriteDown (iBase* obj, iDocumentNode* parent,
  	iStreamSource* ssource);
};


#endif // __CEL_ADDON_DYNWORLDLOAD__

