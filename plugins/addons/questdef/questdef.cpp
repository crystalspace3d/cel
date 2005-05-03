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

#include "cssysdef.h"
#include "csutil/scanstr.h"
#include "iutil/objreg.h"
#include "iutil/document.h"
#include "iutil/object.h"
#include "iutil/plugin.h"
#include "ivaria/reporter.h"
#include "imap/services.h"
#include "iengine/mesh.h"

#include "physicallayer/pl.h"
#include "physicallayer/propclas.h"
#include "physicallayer/entity.h"
#include "behaviourlayer/bl.h"
#include "propclass/mesh.h"
#include "celtool/stdparams.h"

#include "questdef.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celAddOnQuestDef)

SCF_IMPLEMENT_IBASE (celAddOnQuestDef)
  SCF_IMPLEMENTS_INTERFACE (iLoaderPlugin)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celAddOnQuestDef::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celAddOnQuestDef::celAddOnQuestDef (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
  object_reg = 0;
}

celAddOnQuestDef::~celAddOnQuestDef ()
{
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiComponent);
  SCF_DESTRUCT_IBASE ();
}

bool celAddOnQuestDef::Initialize (iObjectRegistry* object_reg)
{
  celAddOnQuestDef::object_reg = object_reg;
  return true;
}

csPtr<iBase> celAddOnQuestDef::Parse (iDocumentNode* node,
	iLoaderContext* ldr_context, iBase* context)
{
  if (!questmgr)
  {
    questmgr = CS_QUERY_REGISTRY (object_reg, iQuestManager);
    if (!questmgr)
    {
      csRef<iPluginManager> plugin_mgr = CS_QUERY_REGISTRY (object_reg,
      	iPluginManager);
      questmgr = CS_LOAD_PLUGIN (plugin_mgr, "cel.manager.quests",
      	iQuestManager);
      if (!questmgr)
      {
	csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	  "cel.addons.questdef",
	  "Can't find quest manager plugin!");
        return 0;
      }
      if (!object_reg->Register (questmgr, "iQuestManager"))
      {
	csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	  "cel.addons.questdef",
	  "Couldn't register quest manager plugin!");
        return 0;
      }
    }
  }

  if (!questmgr->Load (node))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	  "cel.addons.questdef",
	  "Couldn't load quests!");
    return 0;
 }

  // IncRef because we return a csPtr. We have to return something
  // that is not 0 here.
  IncRef ();
  return (iLoaderPlugin*)this;
}

