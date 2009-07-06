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

#ifndef __CEL_TOOLS_SEQOP_DEBUGPRINT__
#define __CEL_TOOLS_SEQOP_DEBUGPRINT__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/sequences.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard seqop type that just prints a message on standard
 * output. This can be useful for debugging purposes.
 * This seqop type listens to the name 'cel.questseqop.debugprint'.
 */
CEL_DECLARE_SEQOPTYPE_NEW(DebugPrint,"cel.seqop.debugprint")

/**
 * The 'debugprint' seqop factory.
 */
class celDebugPrintSeqOpFactory : public scfImplementation2<
	celDebugPrintSeqOpFactory, iSeqOpFactory,
	iDebugPrintSeqOpFactory>
{
private:
  celDebugPrintSeqOpType* type;
  csString msg_par;

public:
  celDebugPrintSeqOpFactory (celDebugPrintSeqOpType* type);
  virtual ~celDebugPrintSeqOpFactory ();

  virtual csPtr<iSeqOp> CreateSeqOp (
      const celParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iDebugPrintSeqOpFactory -----------------------
  virtual void SetMessageParameter (const char* msg);
};

/**
 * The 'printdebug' seqop.
 */
class celDebugPrintSeqOp : public scfImplementation1<
	celDebugPrintSeqOp, iSeqOp>
{
private:
  celDebugPrintSeqOpType* type;
  csRef<iParameterManager> pm;
  csString msg;

public:
  celDebugPrintSeqOp (celDebugPrintSeqOpType* type,
  	const celParams& params,
	const char* msg_par);
  virtual ~celDebugPrintSeqOp ();

  virtual bool Load (iCelDataBuffer*) { return true; }
  virtual void Save (iCelDataBuffer*) { }
  virtual void Init () { }
  virtual void Do (float time);
};

#endif // __CEL_TOOLS_SEQOP_DEBUGPRINT__

