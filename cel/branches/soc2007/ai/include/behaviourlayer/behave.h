/*
    Crystal Space Entity Layer
    Copyright (C) 2001 by Jorrit Tyberghein
  
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

#ifndef __CEL_BL_BEHAVE__
#define __CEL_BL_BEHAVE__

#include <stdarg.h>
#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/strhash.h"

#include "physicallayer/datatype.h"

struct iCelBlLayer;
struct iCelPropertyClass;

SCF_VERSION (iCelParameterBlock, 0, 0, 1);

/**
 * This interface is a parameter block that can be used to pass parameters
 * through SendMessage() in a behaviour.
 * Parameter ids are constructed from the following string:
 *     cel.parameter."name"
 */
struct iCelParameterBlock : public iBase
{
  /**
   * Get number of parameters.
   */
  virtual size_t GetParameterCount () const = 0;

  /**
   * Get parameter with index. Returns the name and sets 'id'
   * and 'type' to the appropriate values.
   */
  virtual const char* GetParameter (size_t idx, csStringID& id,
  	celDataType& t) const = 0;

  /**
   * Get parameter given an id.
   */
  virtual const celData* GetParameter (csStringID id) const = 0;

  /**
   * Get parameter given an index.
   */
  virtual const celData* GetParameterByIndex (size_t idx) const = 0;
};

/**
 * This is an entity in the CEL layer at the BL (behaviour layer) side.
 */
struct iCelBehaviour : public virtual iBase
{
  SCF_INTERFACE (iCelBehaviour, 0, 0, 2);

  /**
   * Get the name of this behaviour (was used in the call to
   * iCelBlLayer::CreateBehaviour()).
   */
  virtual const char* GetName () const = 0;

  /**
   * Get the behaviour layer that created this behaviour.
   */
  virtual iCelBlLayer* GetBehaviourLayer () const = 0;

  /**
   * Send a message to this entity. Returns true if the
   * message was understood and handled by the entity.
   * The 'ret' parameter can be used to return values.
   */
  virtual bool SendMessage (const char* msg_id,
  	iCelPropertyClass* pc,
  	celData& ret, iCelParameterBlock* params, ...) = 0;

  /**
   * Send a message to this entity. Returns true if the
   * message was understood and handled by the entity.
   */
  virtual bool SendMessageV (const char* msg_id,
  	iCelPropertyClass* pc,
  	celData& ret, iCelParameterBlock* params, va_list arg) = 0;

  /**
   * This is a function intended for implementations of behaviour layers.
   * It returns a pointer to an internal object. For example, in case of a
   * Python behaviour layer it would return a pointer to the actual Python
   * object handle (PyObject pointer). Some behaviour layers may not need
   * this and return 0 in that case. User-code should NEVER call this
   * function or depend on it!
   */
  virtual void* GetInternalObject () = 0;
};

#endif // __CEL_BL_BEHAVE__

