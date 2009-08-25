/*
    Crystal Space Entity Layer
    Copyright (C) 2009 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_WIRE__
#define __CEL_PF_WIRE__

#include "cstypes.h"
#include "csutil/scf.h"

struct iCelExpression;

/**
 * This is a wire property class. A wire can connect two property classes.
 * Basically a wire listens to a certain message on a channel and transforms
 * it into either another message (to possibly another channel) or else it performs
 * an 'action'.
 *
 * This property class supports the following actions:
 * - AddInput: parameters 'mask' (string), optional 'entity' (string), ...
 * - AddOutput: parameters 'msgid' (string), optional 'entity' (string), ...
 * - AddAction: parameters 'actionid' (string), optional 'entity' (string), 'pc' (string), ...
 * - MapParameter: parameters 'id' (long), 'source' (string), 'dest' (string), optional 'expression' (string).
 */
struct iPcWire : public virtual iBase
{
  SCF_INTERFACE (iPcWire, 0, 0, 1);

  /**
   * Add a message to react too.
   * If the channel is not given then the message channel from the current entity
   * will be used (the entity containing this property class).
   * It is legal to call AddInput() multiple times. In that case the
   * wire will react to multiple messages from possible multiple sources.
   * \param msg_mask is a mask which will be used to subscribe to the given
   * message channel. It can be a fully qualified message name or something shorter
   * in which case more messages will be catched by this.
   */
  virtual void AddInput (const char* msg_mask, iMessageChannel* channel = 0) = 0;

  /**
   * Add an output message.
   * If the channel is not given then the message channel from the current entity
   * will be used (the entity containing this property class).
   * The resulting parameter block for the output message will be a combination of
   * the parameter block from the input message overwritten by the parameters given
   * here (if given).
   * This function returns a unique identifier that you can use to give to the
   * functions that affect the parameter modifications.
   * \param msgid is a fully qualified message id which will be used to send out.
   */
  virtual size_t AddOutput (const char* msgid, iMessageChannel* channel = 0,
      iCelParameterBlock* extra_params = 0) = 0;

  /**
   * Add an output action.
   * The resulting parameter block for the output message will be a combination of
   * the parameter block from the input message overwritten by the parameters given
   * here (if given).
   * This function returns a unique identifier that you can use to give to the
   * functions that affect the parameter modifications.
   */
  virtual size_t AddOutputAction (csStringID actionID, iCelPropertyClass* pc,
      iCelParameterBlock* extra_params = 0) = 0;

  /**
   * Map a parameter from one name to another directly. With an optional
   * expression the value can be modified.
   * \param id is the id as return by AddOutput() or AddOutputAction(). This
   * function will affect the parameter as given to that specific output.
   * \param source is the name of the parameter in the source.
   * \param dest is the name of the parameter as it should become in the destination.
   * \param expression is an optional expression that can be used to modify
   * the value in the destination. The expression will have access to the
   * combined parameter block (parameters from original message as well as the
   * extra parameters from the given output).
   */
  virtual void MapParameter (size_t id, const char* source, const char* dest,
      iCelExpression* expression = 0) = 0;
};

#endif // __CEL_PF_WIRE__

