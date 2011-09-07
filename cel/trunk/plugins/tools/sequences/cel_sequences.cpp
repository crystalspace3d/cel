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

#include "plugins/tools/sequences/cel_sequences.h"

//---------------------------------------------------------------------------
//CS_IMPLEMENT_PLUGIN 
//---------------------------------------------------------------------------

celSequence::celSequence (const char* name,
	csPtr<iCelPlLayer> pl, csPtr<iVirtualClock> vc) : scfImplementationType (this)
{
  celSequence::name = name;
  celSequence::pl = pl;
  celSequence::vc = vc;
  idx = csArrayItemNotFound;
}

celSequence::~celSequence ()
{
  Abort ();
}

void celSequence::AddSeqOp (iSeqOp* seqop, csTicks start, csTicks end)
{
  celSeqOp seq;
  seq.seqop = seqop;
  seq.start = start;
  seq.end = end;
  seq.idx = seqops.GetSize ();
  seqops.Push (seq);
}

bool celSequence::Start (csTicks delay, iCelParameterBlock* params)
{
  if (IsRunning ()) return false;
  celSequence::params = params;
  idx = 0;
  pl->CallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
  start_time = vc->GetCurrentTicks () + delay;
  return true;
}

void celSequence::Finish ()
{
  if (!IsRunning ()) return;
  Perform (total_time+1);
}

void celSequence::Abort ()
{
  if (!IsRunning ()) return;
  pl->RemoveCallbackEveryFrame ((iCelTimerListener*)this, CEL_EVENT_PRE);
  idx = csArrayItemNotFound;
  ops_in_progress.Empty ();
}

bool celSequence::IsRunning ()
{
  return idx != csArrayItemNotFound;
}

void celSequence::Perform (csTicks rel)
{
  // Find all operations that have to be performed.
  while (idx < seqops.GetSize () && rel >= seqops[idx].start)
  {
    seqops[idx].seqop->Init (params);
    if (rel >= seqops[idx].end)
    {
      // Single shot operation or operation has already ended. Will not
      // be put in the progress array.
      seqops[idx].seqop->Do (1.0f, params);
    }
    else
    {
      ops_in_progress.Push (seqops[idx]);
    }
    idx++;
  }

  // Perform the operations that are still in progress.
  size_t i = 0;
  while (i < ops_in_progress.GetSize ())
  {
    if (rel >= ops_in_progress[i].end)
    {
      ops_in_progress[i].seqop->Do (1.0f, params);
      ops_in_progress.DeleteIndex (i);
    }
    else
    {
      float dt = float (rel - ops_in_progress[i].start)
      	/ float (ops_in_progress[i].end - ops_in_progress[i].start);
      ops_in_progress[i].seqop->Do (dt, params);
      i++;
    }
  }

  if (rel > total_time)
  {
    // Sequence has ended.
    FireSequenceCallbacks ();
    Abort ();
    return;
  }
}

void celSequence::TickEveryFrame ()
{
  csTicks current_time = vc->GetCurrentTicks ();
  if (current_time < start_time) return;
  csTicks rel = current_time - start_time;
  Perform (rel);
}

void celSequence::SaveState (iCelDataBuffer* databuf)
{
  databuf->Add ((uint32)(vc->GetCurrentTicks ()-start_time));

  // Save all operations that are still in progress.
  databuf->Add ((uint16)ops_in_progress.GetSize ());
  size_t i;
  for (i = 0 ; i < ops_in_progress.GetSize () ; i++)
  {
    databuf->Add ((uint32)ops_in_progress[i].idx);
    ops_in_progress[i].seqop->Save (databuf);
  }
}

bool celSequence::LoadState (iCelDataBuffer* databuf)
{
  // First start the sequence.
  // @@@
  // Params need to be saved and loaded for correct operation
  iCelParameterBlock* params = 0;
  Start (0, params);

  csTicks current_time = vc->GetCurrentTicks ();
  start_time = current_time - databuf->GetUInt32 ();
  //csTicks rel = current_time - start_time;

  // When loading state it is important to realize that we assume
  // that the objects on which this sequence operates will load
  // their own state on their own. So we don't have to actually
  // perform the already performed operations again here. We just
  // have to setup the right datastructures so that we can resume
  // de sequence where we left off.

  uint16 cnt_op = databuf->GetUInt16 ();
  size_t i;
  idx = 0;
  for (i = 0 ; i < cnt_op ; i++)
  {
    uint32 id = databuf->GetUInt32 ();
    if (id > idx) idx = id;
    if (!seqops[id].seqop->Load (databuf))
      return false;
    ops_in_progress.Push (seqops[id]);
  }
#if 0
  // Find all operations that have to be performed.
  idx = 0;
  while (idx < seqops.GetSize () && rel >= seqops[idx].start)
  {
    if (rel < seqops[idx].end)
      ops_in_progress.Push (seqops[idx]);
    idx++;
  }
#endif

  return true;
}

void celSequence::AddSequenceCallback (iCelSequenceCallback* cb)
{
  callbacks.Push (cb);
}

void celSequence::RemoveSequenceCallback (iCelSequenceCallback* cb)
{
  callbacks.Delete (cb);
}

void celSequence::FireSequenceCallbacks ()
{
  size_t i = callbacks.GetSize ();
  while (i > 0)
  {
    i--;
    callbacks[i]->SequenceFinished (this);
  }
}

SCF_IMPLEMENT_FACTORY(celSequenceFactoryGenerator)

celSequenceFactoryGenerator::celSequenceFactoryGenerator (iBase* parent) :
    scfImplementationType (this, parent),
	object_reg(0)

{
  //parent_factory = scfQueryInterface<iQuestFactory> (parent);
}

bool celSequenceFactoryGenerator::Initialize (iObjectRegistry* object_reg)
{
  celSequenceFactoryGenerator::object_reg = object_reg;
  return true;
}

csPtr<iCelSequenceFactory> celSequenceFactoryGenerator::CreateSequenceFactory ()
{
  celSequenceFactory* fact = new celSequenceFactory (object_reg);
  return fact;
}

celSequenceFactory::celSequenceFactory (iObjectRegistry* object_reg) : 	
    scfImplementationType (this),
	object_reg(object_reg)

{
  //parent_factory = scfQueryInterface<iQuestFactory> (parent);
}

void celSequenceFactory::SetName (const char *name)
{
  celSequenceFactory::name = name;
}


void celSequenceFactory::AddSeqOpFactory (iSeqOpFactory* seqopfact,
  	const char* duration)
{
  celSeqOpFact s;
  s.seqop = seqopfact;
  s.duration = duration;
  seqops.Push (s);
}

void celSequenceFactory::AddDelay (const char* delay)
{
  celSeqOpFact s;
  s.seqop = 0;
  s.duration = delay;
  seqops.Push (s);
}

static uint ToUInt (const char* s)
{
  if (!s) return 0;
  int f;
  sscanf (s, "%ud", &f);
  return f;
}

csPtr<iCelSequence> celSequenceFactory::CreateSequence (
	iQuest* q, const celParams& params)
{
  celSequence* seq = new celSequence (name, 
    csQueryRegistry<iCelPlLayer> (object_reg), 
	csQueryRegistry<iVirtualClock>(object_reg));
  size_t i;
  csTicks total_time = 0;
  csTicks max_time = 0;
  for (i = 0 ; i < seqops.GetSize () ; i++)
  {
    // @@@ Support dynamic parameters here?
    csRef<iParameterManager> pm = csQueryRegistryOrLoad<iParameterManager> 
      (object_reg, "cel.parameters.manager");

    csTicks duration = ToUInt (pm->ResolveParameter (params, seqops[i].duration));
    if (total_time + duration > max_time) max_time = total_time + duration;
    if (seqops[i].seqop)
    {
      // It is not a delay.
      csRef<iSeqOp> seqop = seqops[i].seqop->CreateSeqOp (params);
      seq->AddSeqOp (seqop, total_time, total_time+duration);
    }
    else
    {
      // A delay.
      total_time += duration;
    }
  }
  seq->SetTotalTime (max_time);
  return csPtr<iCelSequence> (seq);
}

//---------------------------------------------------------------------------

