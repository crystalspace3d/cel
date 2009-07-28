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

struct iCelSequence;

/**
 * This callback is fired when the sequences finished running properly.
 * It is not called it the sequence is aborted!
 */
struct iCelSequenceCallback : public virtual iBase
{
  SCF_INTERFACE (iCelSequenceCallback, 0, 0, 1);

  /// Sequence finishes.
  virtual void SequenceFinished (iCelSequence* sequence) = 0;
};

/**
 * A sequence.
 */
struct iCelSequence : public virtual iBase
{
  SCF_INTERFACE (iCelSequence, 0, 0, 1);

  /**
   * Get the name of this sequence.
   */
  virtual const char* GetName () const = 0;

  /**
   * Save state of this sequence.
   */
  virtual void SaveState (iCelDataBuffer* databuf) = 0;
  /**
   * Load state of this sequence.
   */
  virtual bool LoadState (iCelDataBuffer* databuf) = 0;

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
  virtual void AddSequenceCallback (iCelSequenceCallback* cb) = 0;
  /**
   * Remove a callback.
   */
  virtual void RemoveSequenceCallback (iCelSequenceCallback* cb) = 0;
};

/**
 * A representation of a sequence factory.
 * A sequence factory is basically a sequence of sequence operation factories.
 */
struct iCelSequenceFactory : public virtual iBase
{
  SCF_INTERFACE (iCelSequenceFactory, 0, 0, 1);

  virtual csPtr<iCelSequence> CreateSequence (const celParams& params) = 0;

  /**
   * Get the name of this factory.
   */
  virtual const char* GetName () const = 0;
  
  /**
   * Set the name of this facotry.
   */
  virtual void SetName (const char* name) = 0;

  /**
   * Add a new operation factory at the specified time.
   * \param seqopfact is the factory to add.
   * \param duration is the duration of this operation. This can be
   * a string representing a numerical value or a parameter ($x notation).
   * The duration can be 0 in which case we have a single-stop event.
   */
  virtual void AddSeqOpFactory (iSeqOpFactory* seqopfact,
  	const char* duration) = 0;

  /**
   * Add a delay.
   * \param delay is a the delay or a parameter.
   */
  virtual void AddDelay (const char* delay) = 0;
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
 * The predefined name of this seqop type is 'cel.seqops.debugprint'.
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
 * This interface is implemented by the seqop that animates light colors.
 * You can query this interface from the seqop factory if
 * you want to manually control this factory as opposed to loading
 * its definition from an XML document.
 *
 * The predefined name of this seqop type is 'cel.seqops.light'.
 *
 * In XML, factories recognize the following attributes on the 'op' node:
 * - <em>entity</em>: the name of the entity containing the pclight
 *   property class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 * - <em>relcolor</em>: relative color animation vector.
 *   This node has 'red', 'green, and 'blue' attributes. Each of these
 *   attributes can be a parameter.
 * - <em>abscolor</em>: absolute color.
 *   This node has 'red', 'green, and 'blue' attributes. Each of these
 *   attributes can be a parameter.
 */
struct iLightSeqOpFactory : public virtual iBase
{
  SCF_INTERFACE (iLightSeqOpFactory, 0, 0, 1);

  /**
   * Set the entity containing the pclight (either entity name
   * or a parameter if it starts with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;

  /**
   * Set the relative color animation vector.
   */
  virtual void SetRelColorParameter (const char* red, const char* green,
  	const char* blue) = 0;

  /**
   * Set the absolute color.
   */
  virtual void SetAbsColorParameter (const char* red, const char* green,
  	const char* blue) = 0;
};

/**
 * This interface is implemented by the seqop that moves meshes along
 * a path. You can query this interface from the seqop factory if
 * you want to manually control this factory as opposed to loading
 * its definition from an XML document.
 *
 * The predefined name of this seqop type is 'cel.seqops.movepath'.
 *
 * In XML, factories recognize the following attributes on the 'op' node:
 * - <em>entity</em>: the name of the entity containing the pcmesh
 *   property class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 * - several <em>pathnode</em> tags: one for every node with the following
 *   attributes: sector, node, and time. All of those can be a parameter.
 *   The 'node' refers to a node object in the given sector from which
 *   the position, up ('yvector'), and forward ('zvector') will be used.
 *   'time' should be an increasing time value.
 */
struct iMovePathSeqOpFactory : public virtual iBase
{
  SCF_INTERFACE (iMovePathSeqOpFactory, 0, 0, 1);

  /**
   * Set the entity containing the pcmesh (either entity name
   * or a parameter if it starts with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;

  /**
   * Add a node.
   * \param sectorname is the name of the sector in which the node can
   * be found. Warning! In the current implementation the sector should
   * be the same for all nodes.
   * \param node is the name of the node (position, yvector, and zvector
   * are used).
   * \param time is an increasing time value.
   */
  virtual void AddPathNode (const char* sectorname, const char* node,
  	const char* time) = 0;
};

/**
 * This interface is implemented by the seqop that transforms property
 * class properties. Assuming it is read and write, and of the appropriate type
 * any property can be controlled using this sequence.
 *
 * You can query this interface from the seqop factory if
 * you want to manually control this factory as opposed to loading
 * its definition from an XML document.
 *
 * The predefined name of this seqop type is 'cel.seqops.property'.
 *
 * In XML, factories recognize the following attributes on the 'op' node:
 * - <em>entity</em>: the name of the entity containing the affected property
 *   class.
 * - <em>pc</em>: name for the property class holding the property.
 * - <em>tag</em>: optional tag used to find the right property class 
 *   from the entity.
 * - <em>relative</em>: whether the sequence is relative (default false).
 * - <em>float</em>: optional end value as a float.
 * - <em>long</em>: optional end value as an integer.
 * And the following sub nodes:
 * - <em>v</em>: optional end value as a vector.
 *   This node has 'x', 'y, and (optionally for vector3) 'z' attributes. 
 *
 * Note you must only provide one of 'v', 'float' and 'long' parameters to the
 * sequence, otherwise the result will be undefined. Also, the parameter
 * must fit the actual property type.
 */
struct iPropertySeqOpFactory : public virtual iBase
{
  SCF_INTERFACE (iPropertySeqOpFactory, 0, 0, 1);

  /**
   * Set the entity containing the property (either entity name
   * or a parameter if it starts with '$').
   */
  virtual void SetEntityParameter (const char* entity) = 0;

  /**
   * Set the property class and tag to search for.
   * \param pc is the property class name or a parameter (starts with a '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetPCParameter (const char* pc, const char* tag = 0) = 0;

  /**
   * Set the property name for this sequence.
   * \param property_name is the property name (like cel.property.gravity).
   * It can also be a parameter if it starts with '$'.
   */
  virtual void SetPropertyParameter (const char* property_name) = 0;

  /**
   * Set the end value for the property as a float.
   * \param pfloat is the value to be set.
   * It can also be a parameter if it starts with '$'.
   */
  virtual void SetFloatParameter (const char* pfloat) = 0;

  /**
   * Set the end value for the property as a long.
   * \param plong is the value to be set.
   * It can also be a parameter if it starts with '$'.
   */
  virtual void SetLongParameter (const char* plong) = 0;

  /**
   * Set the end value for the property as a vector2.
   * \param pvectorx is the x component for the vector.
   * \param pvectory is the y component for the vector.
   * Both pvectorx and pvectory can be parameters if they start with '$'.
   */
  virtual void SetVector2Parameter (const char* vectorx, 
	const char* vectory) = 0;

  /**
   * Set the end value for the property as a vector3.
   * \param pvectorx is the x component for the vector.
   * \param pvectory is the y component for the vector.
   * \param pvectorz is the z component for the vector.
   * Both pvectorx, pvectory and pvectorz can be a parameters if they 
   * start with '$'.
   */
  virtual void SetVector3Parameter (const char* vectorx, const char* vectory,
        const char* vectorz) = 0;

   /**
   * Set whether the sequence will be relative:
   *  (end value = specified value + starting value)
   * or absolute.
   *  (end value = specified value)
   * \param is_relative whether the sequence is relative. can't be a parameter.
   */
  virtual void SetRelative (bool is_relative) = 0;
};


/**
 * This interface is implemented by the seqop that transforms meshes.
 * You can query this interface from the seqop factory if
 * you want to manually control this factory as opposed to loading
 * its definition from an XML document.
 *
 * The predefined name of this seqop type is 'cel.seqops.transform'.
 *
 * In XML, factories recognize the following attributes on the 'op' node:
 * - <em>entity</em>: the name of the entity containing the pcmesh
 *   property class.
 * - <em>entity_tag</em>: optional tag used to find the right
 *   property class from the entity.
 * - <em>v</em>: optional movement vector.
 *   This node has 'x', 'y, and 'z' attributes. Each of these attributes
 *   can be a parameter.
 * - <em>rotx</em>: optional rotation along x axis. This node has
 *   an 'angle' parameter in radians. Angle can be a parameter.
 * - <em>roty</em>: optional rotation along y axis. This node has
 *   an 'angle' parameter in radians. Angle can be a parameter.
 * - <em>rotz</em>: optional rotation along z axis. This node has
 *   an 'angle' parameter in radians. Angle can be a parameter.
 */
struct iTransformSeqOpFactory : public virtual iBase
{
  SCF_INTERFACE (iTransformSeqOpFactory, 0, 0, 1);

  /**
   * Set the entity containing the pcmesh (either entity name
   * or a parameter if it starts with '$').
   * \param tag is the optional tag of the entity or a parameter (starts
   * with '$').
   */
  virtual void SetEntityParameter (const char* entity, const char* tag = 0) = 0;

  /**
   * Set the relative movement vector parameter.
   */
  virtual void SetVectorParameter (const char* vectorx, const char* vectory,
  	const char* vectorz) = 0;

  /**
   * Set the relative rotation parameter.
   * \param rot_axis is 0, 1, or 2 for x, y, or z axis.
   * \param rot_angle the amount of rotation.
   */
  virtual void SetRotationParameter (int rot_axis, const char* rot_angle) = 0;
};
//-------------------------------------------------------------------------

/**
 * Convenience to declare a new sequence operation type class.
 */
#define CEL_DECLARE_SEQOPTYPE(name,id)					\
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
#define CEL_IMPLEMENT_SEQOPTYPE(name)					\
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