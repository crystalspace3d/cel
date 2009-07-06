/*
    Crystal Space Entity Layer
    Copyright (C) 2004-2006 by Jorrit Tyberghein
  
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

#ifndef __CEL_SEQUENCES__
#define __CEL_SEQUENCES__

#include "behaviourlayer/behave.h"
#include "tools/parameters.h"

//-------------------------------------------------------------------------
// Sequence operations
//-------------------------------------------------------------------------

/**
 * This is a sequence operation. A sequence operation represents
 * an operation that has a duration or an operation that is a single-shot.
 * You can combine different sequence operations in a sequence and then use
 * a reward to fire of the sequence as result of a trigger.
 */
struct iSeqOp : public virtual iBase
{
  SCF_INTERFACE (iSeqOp, 0, 0, 1);
  
  /**
   * Init the operation. This is called before the operation is actually
   * performed the first time. This function is not called in case the
   * operation is loaded from persisted data. In that case Load()
   * is called instead.
   */
  virtual void Init () = 0;

  /**
   * Load the sequence operation from persisted data.
   */
  virtual bool Load (iCelDataBuffer* databuf) = 0;

  /**
   * Save the sequence operation to persisted data.
   */
  virtual void Save (iCelDataBuffer* databuf) = 0;

  /**
   * Do the operation. The parameter is a value between 0 and 1 which
   * will be interpolated over a specified time (specified in the sequence).
   * In case this is a single-shot operation the value will always be 1.
   */
  virtual void Do (float time) = 0;
};

/**
 * A factory to create sequence operators. Factories are created
 * by an iSeqOpType instance.
 */
struct iSeqOpFactory : public virtual iBase
{
  SCF_INTERFACE (iSeqOpFactory, 0, 0, 1);

  /**
   * Create a sequence operation.
   * \param params are the parameters with which this reward is
   * instantiated.
   */
  virtual csPtr<iSeqOp> CreateSeqOp (const celParams& params) = 0;

  /**
   * Load this factory from a document node.
   * \param node is the node for the operation.
   * \return false on error (reporter is used to report).
   */
  virtual bool Load (iDocumentNode* node) = 0;
};

/**
 * The seqop type is responsible for the creation of seqop factories.
 * If you want to define a new type of sequence operation then you must
 * implement iSeqOpType, iSeqOpFactory, and iSeqOp.
 */
struct iSeqOpType : public virtual iBase
{
  SCF_INTERFACE (iSeqOpType, 0, 0, 1);

  /**
   * Return the name for this sequence operation type.
   */
  virtual const char* GetName () const = 0;
  
  /**
   * Create a sequence operation factory.
   */
  virtual csPtr<iSeqOpFactory> CreateSeqOpFactory () = 0;
};

struct iSequence;

/**
 * This callback is fired when the sequences finished running properly.
 * It is not called it the sequence is aborted!
 */
struct iSequenceCallback : public virtual iBase
{
  SCF_INTERFACE (iSequenceCallback, 0, 0, 1);

  /// Sequence finishes.
  virtual void SequenceFinished (iSequence* sequence) = 0;
};

/**
 * A sequence.
 */
struct iSequence : public virtual iBase
{
  SCF_INTERFACE (iSequence, 0, 0, 1);

  /**
   * Get the name of this sequence.
   */
  virtual const char* GetName () const = 0;

  /**
   * Fire this sequence.
   * \param delay is a delay before the sequence will really start.
   * \return false if the sequence is already running.
   */
  virtual bool Start (csTicks delay) = 0;

  /**
   * Finish this sequence. The sequence will be executed at end condition
   * which means that all operations that have not yet fully executed
   * will be executed at relative time index '1'. So the end result
   * will be as if the sequence completed normally.
   */
  virtual void Finish () = 0;

  /**
   * Abort this sequence. This will simply abort the sequence without
   * doing any further operation. This can leave the objects on which
   * this sequence operates in undefined states.
   */
  virtual void Abort () = 0;

  /**
   * Return true if this sequence is currently running.
   */
  virtual bool IsRunning () = 0;

  /**
   * Register a callback that is fired when this sequence finishes.
   */
  virtual void AddSequenceCallback (iSequenceCallback* cb) = 0;
  /**
   * Remove a callback.
   */
  virtual void RemoveSequenceCallback (iSequenceCallback* cb) = 0;
};

//-------------------------------------------------------------------------
// Specific sequence operation implementations.
//-------------------------------------------------------------------------

/**
 * This interface is implemented by the seqop that prints
 * debug messages on standard output. You can query this interface
 * from the seqop factory if you want to manually control
 * this factory as opposed to loading its definition from an XML
 * document.
 *
 * The predefined name of this seqop type is 'cel.seqop.debugprint'.
 *
 * In XML, factories recognize the following attributes on the 'op' node:
 * - <em>message</em>: the message to print.
 */
struct iDebugPrintSeqOpFactory : public virtual iBase
{
  SCF_INTERFACE (iDebugPrintSeqOpFactory, 0, 0, 1);

  /**
   * Set the message parameter to print (either a message string
   * or a parameter if it starts with '$').
   */
  virtual void SetMessageParameter (const char* msg) = 0;
};

/**
 * Convenience to declare a new sequence operation type class.
 */
#define CEL_DECLARE_SEQOPTYPE_NEW(name,id)					\
class cel##name##SeqOpType : public scfImplementation2<			\
	cel##name##SeqOpType,iSeqOpType,iComponent>				\
{									\
public:									\
  iObjectRegistry* object_reg;						\
  csWeakRef<iCelPlLayer> pl;						\
  cel##name##SeqOpType (iBase* parent);			\
  virtual ~cel##name##SeqOpType () { }					\
  virtual bool Initialize (iObjectRegistry*)	;		\
  virtual const char* GetName () const { return id; }			\
  virtual csPtr<iSeqOpFactory> CreateSeqOpFactory ();		\
};

/**
 * Convenience to implement a new sequence operation type class.
 */
#define CEL_IMPLEMENT_SEQOPTYPE_NEW(name)					\
cel##name##SeqOpType::cel##name##SeqOpType (				\
	iBase* parent) : scfImplementationType (this, parent),	\
	object_reg(0)											\
{															\
}															\
bool cel##name##SeqOpType::Initialize (					\
	iObjectRegistry* object_reg)							\
{															\
  cel##name##SeqOpType::object_reg = object_reg;			\
  pl = csQueryRegistry<iCelPlLayer> (object_reg);			\
  return true;												\
}									\
csPtr<iSeqOpFactory> cel##name##SeqOpType::CreateSeqOpFactory ()	\
{									\
  cel##name##SeqOpFactory* fact = new cel##name##SeqOpFactory (this);	\
  return fact;								\
}


#endif // __CEL_SEQUENCES__