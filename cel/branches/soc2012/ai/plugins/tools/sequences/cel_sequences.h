/*
    Crystal Space Entity Layer
    Copyright (C) 2009 by Jorrit Tyberghein
	Copyright (C) 2009 by Sam Devlin
  
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
struct celSeqOp
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
  csArray<celSeqOp> seqops;
  csArray<celSeqOp> ops_in_progress;
  csString name;
  csWeakRef<iCelPlLayer> pl;
  csRef<iVirtualClock> vc;
  size_t idx;
  csTicks start_time;
  csTicks total_time;
  csTicks deactivationTime;	// Sequence was deactivated at this point.
  iCelParameterBlock* params;

  csRefArray<iCelSequenceCallback> callbacks;

  // Perform the sequence upto relative time 'rel'.
  void Perform (csTicks rel);

  // Fire the sequence callbacks.
  void FireSequenceCallbacks ();

public:
  celSequence (const char* name, 
	csPtr<iCelPlLayer> pl,
  	csPtr<iVirtualClock> vc);
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

  // --- For iCelSequence -------------------------------
  virtual const char* GetName () const { return name; }
  virtual bool Start (csTicks delay, iCelParameterBlock* params);
  virtual void Finish ();
  virtual void Abort ();
  virtual bool IsRunning ();
  virtual void AddSequenceCallback (iCelSequenceCallback* cb);
  virtual void RemoveSequenceCallback (iCelSequenceCallback* cb);

  virtual void Activate ();
  virtual void Deactivate ();

  // --- For iCelTimerListener ----------------------------
  virtual void TickEveryFrame ();
  virtual void TickOnce () { }
};

/**
 * Sequence operation factory.
 */
struct celSeqOpFact
{
  csRef<iSeqOpFactory> seqop;	// If 0 this is a delay.
  csString duration;			// Duration or parameter.
};

/**
 * A sequence factory.
 */
class celSequenceFactory : public scfImplementation1<
	celSequenceFactory, iCelSequenceFactory>
{
private:
  iObjectRegistry* object_reg;
  csString name;
  // The array of sequence operations will be sorted due to the
  // presence of the csComparator specialization above.
  csArray<celSeqOpFact> seqops;

public:
  celSequenceFactory (iObjectRegistry* object_reg);
  virtual ~celSequenceFactory () { }

  // From iCelSequenceFactory
  virtual csPtr<iCelSequence> CreateSequence (iQuest* q, iCelParameterBlock* params);
  virtual const char* GetName () const { return name; }
  virtual void SetName (const char* name);
  virtual void AddSeqOpFactory (iSeqOpFactory* seqopfact,
  	const char* duration);
  virtual void UpdateSeqOpFactory (size_t idx, iSeqOpFactory* seqopfact,
      const char* duration);
  virtual size_t GetSeqOpFactoryCount () const { return seqops.GetSize (); }
  virtual iSeqOpFactory* GetSeqOpFactory (size_t idx) const { return seqops[idx].seqop; }
  virtual const char* GetSeqOpFactoryDuration (size_t idx) const { return seqops[idx].duration; }
  virtual void RemoveSeqOpFactory (size_t idx) { seqops.DeleteIndex (idx); }
  virtual void AddDelay (const char* delay);
};

/**
 * A sequence factory generator.
 */
class celSequenceFactoryGenerator : public scfImplementation2<
	celSequenceFactoryGenerator, iCelSequenceFactoryGenerator, iComponent>
{
private:
  iObjectRegistry* object_reg;

public:
  celSequenceFactoryGenerator (iBase* parent);
  virtual ~celSequenceFactoryGenerator () { }

  // From iComponent.
  virtual bool Initialize (iObjectRegistry*);

  // From iCelSequenceFactory
  virtual csPtr<iCelSequenceFactory> CreateSequenceFactory ();
};


#endif // __CEL_TOOLS_SEQUENCES

