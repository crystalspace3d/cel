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
#include "csutil/objreg.h"
#include "csutil/dirtyaccessarray.h"
#include "csutil/util.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/document.h"
#include "iutil/plugin.h"
#include "ivaria/reporter.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "propclass/camera.h"

#include "plugins/tools/sequences/seqop_debugprint.h"

//---------------------------------------------------------------------------
CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celDebugPrintSeqOpType)
CEL_IMPLEMENT_SEQOPTYPE_NEW(DebugPrint)

//---------------------------------------------------------------------------

celDebugPrintSeqOpFactory::celDebugPrintSeqOpFactory (
	celDebugPrintSeqOpType* type) : scfImplementationType (this)
{
  celDebugPrintSeqOpFactory::type = type;
}

celDebugPrintSeqOpFactory::~celDebugPrintSeqOpFactory ()
{
}

csPtr<iSeqOp> celDebugPrintSeqOpFactory::CreateSeqOp (
    const celParams& params)
{
  celDebugPrintSeqOp* seqop = new celDebugPrintSeqOp (type,
  	params, msg_par);
  return seqop;
}

bool celDebugPrintSeqOpFactory::Load (iDocumentNode* node)
{
  msg_par = node->GetAttributeValue ("message");

  if (!msg_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.seqop.debugprint",
      "'message' attribute is missing for the debugprint seqop!");
    return false;
  }
  return true;
}

void celDebugPrintSeqOpFactory::SetMessageParameter (const char* msg)
{
  msg_par = msg;
}

//---------------------------------------------------------------------------

celDebugPrintSeqOp::celDebugPrintSeqOp (
	celDebugPrintSeqOpType* type,
  	const celParams& params,
	const char* msg_par) : scfImplementationType (this)
{
  celDebugPrintSeqOp::type = type;
  //csRef<iQuestManager> qm = csQueryRegistry<iQuestManager> (type->object_reg);
  
  csRef<iPluginManager> plugin_mgr = 
   csQueryRegistry<iPluginManager> (type->object_reg);
  csRef<iParameterManager> pm = csLoadPlugin<iParameterManager> 
    (plugin_mgr, "cel.parameters.manager");

  msg = pm->ResolveParameter (params, msg_par);
}

celDebugPrintSeqOp::~celDebugPrintSeqOp ()
{
}

void celDebugPrintSeqOp::Do (float time)
{
  printf ("REFACTORED SEQUENCE: %s (time=%g)\n", (const char*)msg, time);
  fflush (stdout);
}

//---------------------------------------------------------------------------

