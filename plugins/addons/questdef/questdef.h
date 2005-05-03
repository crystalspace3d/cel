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

#ifndef __CEL_ADDON_QUESTDEF__
#define __CEL_ADDON_QUESTDEF__

#include "iutil/comp.h"
#include "imap/reader.h"
#include "csutil/strhash.h"
#include "csutil/weakref.h"

#include "tools/questmanager.h"

struct iObjectRegistry;
struct iDocumentNode;
struct iLoaderContext;
struct iSyntaxService;
struct iQuestManager;

/**
 * This is an add-on to allow loading of quests through a standard
 * CS map file.
 */
class celAddOnQuestDef : public iLoaderPlugin
{
private:
  iObjectRegistry* object_reg;
  csWeakRef<iQuestManager> questmgr;

public:
  SCF_DECLARE_IBASE;
 
  celAddOnQuestDef (iBase* parent);
  virtual ~celAddOnQuestDef ();

  /**
   * Initialize this plugin.
   */
  bool Initialize (iObjectRegistry* object_reg);

  /**
   * Parses the quests.
   */
  virtual csPtr<iBase> Parse (iDocumentNode* node, iLoaderContext* ldr_context,
  	iBase* context);

  struct Component : public iComponent
  {
    SCF_DECLARE_EMBEDDED_IBASE (celAddOnQuestDef);
    virtual bool Initialize (iObjectRegistry* p)
    { return scfParent->Initialize (p); }
  } scfiComponent;
};

#endif // __CEL_ADDON_QUESTDEF__

