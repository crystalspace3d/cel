/*
    Crystal Space Entity Layer
    Copyright (C) 2004 by Jorrit Tyberghein

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

#include "plugins/tools/quests/reward_debugprint.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celDebugPrintRewardType)
  SCF_IMPLEMENTS_INTERFACE (iQuestRewardType)
SCF_IMPLEMENT_IBASE_END

celDebugPrintRewardType::celDebugPrintRewardType (
	iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (0);
  celDebugPrintRewardType::object_reg = object_reg;
}

celDebugPrintRewardType::~celDebugPrintRewardType ()
{
  SCF_DESTRUCT_IBASE ();
}

csPtr<iQuestRewardFactory> celDebugPrintRewardType::CreateRewardFactory ()
{
  celDebugPrintRewardFactory* fact = new
  	celDebugPrintRewardFactory (this);
  return fact;
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celDebugPrintRewardFactory)
  SCF_IMPLEMENTS_INTERFACE (iQuestRewardFactory)
  SCF_IMPLEMENTS_INTERFACE (iDebugPrintQuestRewardFactory)
SCF_IMPLEMENT_IBASE_END

celDebugPrintRewardFactory::celDebugPrintRewardFactory (
	celDebugPrintRewardType* type)
{
  SCF_CONSTRUCT_IBASE (0);
  celDebugPrintRewardFactory::type = type;
  msg_par = 0;
}

celDebugPrintRewardFactory::~celDebugPrintRewardFactory ()
{
  delete[] msg_par;
  SCF_DESTRUCT_IBASE ();
}

csPtr<iQuestReward> celDebugPrintRewardFactory::CreateReward (
    const csHash<csStrKey,csStrKey,csConstCharHashKeyHandler>& params)
{
  celDebugPrintReward* trig = new celDebugPrintReward (type,
  	params, msg_par);
  return trig;
}

bool celDebugPrintRewardFactory::Load (iDocumentNode* node)
{
  delete[] msg_par; msg_par = 0;
  msg_par = node->GetAttributeValue ("message");
  if (!msg_par)
  {
    csReport (type->object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.questreward.debugprint",
      "'message' attribute is missing for the debugprint reward!");
    return false;
  }
  return true;
}

void celDebugPrintRewardFactory::SetMessageParameter (const char* msg)
{
  if (msg_par == msg) return;
  delete[] msg_par;
  msg_par = csStrNew (msg);
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celDebugPrintReward)
  SCF_IMPLEMENTS_INTERFACE (iQuestReward)
SCF_IMPLEMENT_IBASE_END

celDebugPrintReward::celDebugPrintReward (
	celDebugPrintRewardType* type,
  	const csHash<csStrKey,csStrKey,csConstCharHashKeyHandler>& params,
	const char* msg_par)
{
  SCF_CONSTRUCT_IBASE (0);
  celDebugPrintReward::type = type;
  csRef<iQuestManager> qm = CS_QUERY_REGISTRY (type->object_reg, iQuestManager);
  msg = csStrNew (qm->ResolveParameter (params, msg_par));
}

celDebugPrintReward::~celDebugPrintReward ()
{
  delete[] msg;
  SCF_DESTRUCT_IBASE ();
}

void celDebugPrintReward::Reward ()
{
  printf ("%s\n", msg);
  fflush (stdout);
}

//---------------------------------------------------------------------------

