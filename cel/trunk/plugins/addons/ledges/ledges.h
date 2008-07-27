/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein
  
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

#ifndef __CEL_ADDON_LEDGES__
#define __CEL_ADDON_LEDGES__

#include "iutil/comp.h"
#include "imap/reader.h"
#include "csutil/strhash.h"
#include "csutil/weakref.h"
#include "tools/ledges.h"

struct iObjectRegistry;
struct iDocumentNode;
struct iLoaderContext;
struct iSyntaxService;

/**
 * This is an add-on to allow loading of ledges through a standard
 * CS map file.
 */
class celAddOnLedges : public scfImplementation2<celAddOnLedges, iLoaderPlugin, iComponent>
{
public:
  celAddOnLedges (iBase* parent);
  virtual ~celAddOnLedges ();

  bool Initialize (iObjectRegistry* object_reg);

  csPtr<iBase> Parse (iDocumentNode* node, iStreamSource*, iLoaderContext* ldr_context, iBase* context);
  csPtr<iLedgeGroup> Load (iDocumentNode* node);

private:
  void LoadLedge (iDocumentNode* node, iLedgeGroup* ledges);

  iObjectRegistry* object_reg;
  csWeakRef<iSyntaxService> synldr;
};

#endif // __CEL_ADDON_LEDGES__
