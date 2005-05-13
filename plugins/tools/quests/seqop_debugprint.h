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

#ifndef __CEL_TOOLS_QUESTS_SEQOP_DEBUGPRINT__
#define __CEL_TOOLS_QUESTS_SEQOP_DEBUGPRINT__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/questmanager.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard seqop type that just prints a message on standard
 * output. This can be useful for debugging purposes.
 * This seqop type listens to the name 'cel.questseqop.debugprint'.
 */
CEL_DECLARE_SEQOPTYPE(DebugPrint,"cel.questseqop.debugprint")

/**
 * The 'debugprint' seqop factory.
 */
class celDebugPrintSeqOpFactory :
	public iQuestSeqOpFactory,
	public iDebugPrintQuestSeqOpFactory
{
private:
  celDebugPrintSeqOpType* type;
  char* msg_par;

public:
  celDebugPrintSeqOpFactory (celDebugPrintSeqOpType* type);
  virtual ~celDebugPrintSeqOpFactory ();

  SCF_DECLARE_IBASE;

  virtual csPtr<iQuestSeqOp> CreateSeqOp (
      const celQuestParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iDebugPrintQuestSeqOpFactory -----------------------
  virtual void SetMessageParameter (const char* msg);
};

/**
 * The 'printdebug' seqop.
 */
class celDebugPrintSeqOp :
	public iQuestSeqOp
{
private:
  celDebugPrintSeqOpType* type;
  char* msg;

public:
  celDebugPrintSeqOp (celDebugPrintSeqOpType* type,
  	const celQuestParams& params,
	const char* msg_par);
  virtual ~celDebugPrintSeqOp ();

  SCF_DECLARE_IBASE;

  virtual void Do (float time);
};

#endif // __CEL_TOOLS_QUESTS_SEQOP_DEBUGPRINT__

