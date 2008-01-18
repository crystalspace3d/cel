/*
    Crystal Space Entity Layer
    Copyright (C) 2008 by Jorrit Tyberghein
  
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

#ifndef __CEL_PL_MESSAGING
#define __CEL_PL_MESSAGING

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/set.h"
#include "csutil/scfarray.h"
#include "iutil/strset.h"
#include "physicallayer/datatype.h"

struct iCelParameterBlock;
struct iMessageDispatcher;
struct iMessageReceiver;
struct iMessageSender;

/**
 * This is an message channel. Objects interested in receiving
 * messages from this channel can implement iMessageReceiver
 * and register themselves to this channel. Objects interested
 * in sending messages to this channel can implement iMessageSender.
 *
 * Messages in this system are hierarchical. For example
 * 'cel.object.mesh.select.down' could be the message from the
 * mesh select property class.
 */
struct iMessageChannel : public virtual iBase
{
  SCF_INTERFACE (iMessageChannel, 0, 0, 1);

  /**
   * Create a message dispatcher. If you are a message sender
   * (iMessageSender) then you can use this method to get handles
   * for sending messages for a specific message ID. This ID should
   * represent a fully qualified message ID (and not a message ID mask).
   * For example 'cel.object.mesh.select.down'.
   */
  virtual csRef<iMessageDispatcher> CreateMessageDispatcher (
      iMessageSender* sender, csStringID msg_id) = 0;

  /**
   * Remove a message dispatcher. Use this if you are no longer
   * interested in sending messages. Note that it is safe to not
   * remove a message dispatcher. If the channel or sender goes
   * away message dispatcher are cleaned up automatically.
   */
  virtual void RemoveMessageDispatcher (iMessageDispatcher* msgdest) = 0; 

  /**
   * Subscribe to this channel for all messages that match with the given
   * mask.
   */
  virtual void Subscribe (iMessageReceiver* receiver, const char* mask) = 0;

  /**
   * Unsubscribe from this channel.
   * \param mask if this mask is given then all subscriptions that
   * match with this mask will be unsubscribed. Otherwise all subscriptions
   * for this receiver will be unsubscribed.
   */
  virtual void Unsubscribe (iMessageReceiver* receiver,
    const char* mask = 0) = 0;
};

/**
 * Implement this interface if you are interested in receiving messages.
 */
struct iMessageReceiver : public virtual iBase
{
  SCF_INTERFACE (iMessageReceiver, 0, 0, 1);

  /**
   * Receive a message for a specific message id.
   * \param msg_id this is the message ID representing the message.
   * \param sender is an optional sender. If the sender doesn't want to
   * be identified this field can be 0.
   * \param ret gives the possibility to return information to the sender
   * of this message.
   * \param params contains the parameters for this message.
   * \return false if this message was not handled or recognized by this
   * receiver. Otherwise true.
   */
  virtual bool ReceiveMessage (csStringID msg_id, iMessageSender* sender,
      celData& ret, iCelParameterBlock* params) = 0;
};

/**
 * This interface represents the sender of a message.
 */
struct iMessageSender : public virtual iBase
{
  SCF_INTERFACE (iMessageSender, 0, 0, 1);

  /**
   * This method is called by the channel if some message dispatcher is
   * removed.
   */
  virtual void MessageDispatcherRemoved (
      iMessageDispatcher* dispatcher) = 0;
};

/**
 * This is a message dispatcher. If you want to send messages you create
 * an instance of this interface through the message channel. Then you use
 * that instance to send messages to all interested parties.
 */
struct iMessageDispatcher : public virtual iBase
{
  SCF_INTERFACE (iMessageDispatcher, 0, 0, 1);

  /**
   * Send a message for a specific message id.
   * \param ret if this is not 0 then it can be used to collect information
   * from the receivers. If 0 then information from the receivers is simply
   * ignored.
   * \param params contains the parameters for this message.
   * \return false if this message was not handled or recognized by any
   * message receiver. If it was handled by at least one receiver then
   * this will return true.
   */
  virtual bool SendMessage (iCelDataArray* ret, iCelParameterBlock* params) = 0;
};


#endif // __CEL_PL_MESSAGING

