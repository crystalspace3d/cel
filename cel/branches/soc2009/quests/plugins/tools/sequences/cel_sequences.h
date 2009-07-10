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

#ifndef __CEL_TOOLS_SEQUENCES__
#define __CEL_TOOLS_SEQUENCES__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "tools/sequences.h"

//#include "tools/questmanager.h"
#include "plugins/tools/quests/quests.h"

struct iObjectRegistry;
struct iEvent;

/**
 * Sequence operation.
 */
struct celSeqOp_NEW
{
  csRef<iSeqOp> seqop;
  size_t idx;
  csTicks start;
  csTicks end;
};

/**
 * A sequence.
 */
class celSequence : public scfImplementation2<celSequence,
	iCelSequence,iCelTimerListener>
{
private:
  csArray<celSeqOp_NEW> seqops;
  csArray<celSeqOp_NEW> ops_in_progress;
  csString name;
  iCelPlLayer* pl;
  iVirtualClock* vc;
  size_t idx;
  csTicks start_time;
  csTicks total_time;

  csRefArray<iCelSequenceCallback> callbacks;

  // Perform the sequence upto relative time 'rel'.
  void Perform (csTicks rel);

  // Fire the sequence callbacks.
  void FireSequenceCallbacks ();

public:
  celSequence (const char* name, iCelPlLayer* pl,
  	iVirtualClock* vc);
  virtual ~celSequence ();

  /**
   * Add a new seqop. Warning! Seqops must be added in ascending order
   * based on 'start'.
   */
  void AddSeqOp (iSeqOp* seqop, csTicks start, csTicks end);

  /**
   * Set total time.
   */
  void SetTotalTime (csTicks total_time)
  {
    celSequence::total_time = total_time;
  }

  /**
   * Save state of this sequence.
   */
  virtual void SaveState (iCelDataBuffer* databuf);
  /**
   * Load state of this sequence.
   */
  bool LoadState (iCelDataBuffer* databuf);

  // --- For iCelSequence -------------------------------
  virtual const char* GetName () const { return name; }
  virtual bool Start (csTicks delay);
  virtual void Finish ();
  virtual void Abort ();
  virtual bool IsRunning ();
  virtual void AddSequenceCallback (iCelSequenceCallback* cb);
  virtual void RemoveSequenceCallback (iCelSequenceCallback* cb);

  // --- For iCelTimerListener ----------------------------
  virtual void TickEveryFrame ();
  virtual void TickOnce () { }
};

/**
 * Sequence operation factory.
 */
struct celSeqOpFact_NEW
{
  csRef<iSeqOpFactory> seqop;	// If 0 this is a delay.
  csString duration;			// Duration or parameter.
};

/**
 * A sequence factory.
 */
class celSequenceFactory : public scfImplementation2<
	celSequenceFactory, iCelSequenceFactory, iComponent>
{
private:
  iObjectRegistry* object_reg;
  csRef<iQuestFactory> parent_factory;
  csString name;
  // The array of sequence operations will be sorted due to the
  // presence of the csComparator specialization above.
  csArray<celSeqOpFact_NEW> seqops;

public:
  celSequenceFactory (iBase* parent);
  virtual ~celSequenceFactory () { }

  // From iComponent.
  virtual bool Initialize (iObjectRegistry*);

  // From iCelSequenceFactory
  virtual csPtr<iCelSequence> CreateSequence (const celParams& params);
  virtual const char* GetName () const { return name; }
  virtual void SetName (const char* name);
  virtual bool Load (iDocumentNode* node);
  virtual void AddSeqOpFactory (iSeqOpFactory* seqopfact,
  	const char* duration);
  virtual void AddDelay (const char* delay);
};


#endif // __CEL_TOOLS_SEQUENCES

