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
#include "ivaria/reporter.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "propclass/camera.h"

#include "plugins/tools/quests/seqop_debugprint.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_SEQOPTYPE(DebugPrint)

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celDebugPrintSeqOpFactory)
  SCF_IMPLEMENTS_INTERFACE (iQuestSeqOpFactory)
  SCF_IMPLEMENTS_INTERFACE (iDebugPrintQuestSeqOpFactory)
SCF_IMPLEMENT_IBASE_END

celDebugPrintSeqOpFactory::celDebugPrintSeqOpFactory (
	celDebugPrintSeqOpType* type)
{
  SCF_CONSTRUCT_IBASE (0);
  celDebugPrintSeqOpFactory::type = type;
  msg_par = 0;
}

celDebugPrintSeqOpFactory::~celDebugPrintSeqOpFactory ()
{
  delete[] msg_par;

  SCF_DESTRUCT_IBASE ();
}

csPtr<iQuestSeqOp> celDebugPrintSeqOpFactory::CreateSeqOp (
    const csHash<csStrKey,csStrKey>& params)
{
  celDebugPrintSeqOp* seqop = new celDebugPrintSeqOp (type,
  	params, msg_par);
  return seqop;
}

bool celDebugPrintSeqOpFactory::Load (iDocumentNode* node)
{
  delete[] msg_par; msg_par = 0;
  msg_par = csStrNew (node->GetAttributeValue ("message"));

  if (!msg_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questreward.debugprint",
      "'message' attribute is missing for the debugprint reward!");
    return false;
  }
  return true;
}

void celDebugPrintSeqOpFactory::SetMessageParameter (const char* msg)
{
  if (msg_par == msg) 
    return;

  delete[] msg_par;
  msg_par = csStrNew (msg);
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celDebugPrintSeqOp)
  SCF_IMPLEMENTS_INTERFACE (iQuestSeqOp)
SCF_IMPLEMENT_IBASE_END

celDebugPrintSeqOp::celDebugPrintSeqOp (
	celDebugPrintSeqOpType* type,
  	const csHash<csStrKey,csStrKey>& params,
	const char* msg_par)
{
  SCF_CONSTRUCT_IBASE (0);
  celDebugPrintSeqOp::type = type;
  csRef<iQuestManager> qm = CS_QUERY_REGISTRY (type->object_reg, iQuestManager);
  msg = csStrNew (qm->ResolveParameter (params, msg_par));
}

celDebugPrintSeqOp::~celDebugPrintSeqOp ()
{
  delete[] msg;
  SCF_DESTRUCT_IBASE ();
}

void celDebugPrintSeqOp::Do (float time)
{
  printf ("%s (time=%g)\n", msg, time);
  fflush (stdout);
}

//---------------------------------------------------------------------------

