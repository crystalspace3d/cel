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

#ifndef __CEL_CELTOOL_STDMSGCHANNEL__
#define __CEL_CELTOOL_STDMSGCHANNEL__

#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "csutil/weakrefarr.h"
#include "csutil/hash.h"
#include "physicallayer/pl.h"
#include "physicallayer/messaging.h"
#include "celtool/celtoolextern.h"

class celMessageSubscription
{
public:
  csWeakRef<iMessageReceiver> receiver;
  csString mask;
  inline friend bool operator< (const celMessageSubscription& sub1,
      const celMessageSubscription& sub2)
  {
    return sub1.mask < sub2.mask;
  }
};

class celMessageDispatcher : public scfImplementation1<celMessageDispatcher,
  iMessageDispatcher>
{
private:
  csStringID msg_id;
  csString message;
  csWeakRef<iMessageSender> sender;
  csWeakRefArray<iMessageReceiver> receivers;
  csRef<iMessageReceiverFilter> receiver_filter;

public:
  celMessageDispatcher (csStringID msg_id, const char* message,
      iMessageSender* sender, iMessageReceiverFilter* receiver_filter)
    	: scfImplementationType (this),
	  msg_id (msg_id),
	  message (message),
	  sender (sender),
	  receiver_filter (receiver_filter) { }
  virtual ~celMessageDispatcher () { }
  virtual bool SendMessage (iCelParameterBlock* params, iCelDataArray* ret)
  {
    size_t i = 0;
    bool handled = false;
    while (i < receivers.GetSize ())
    {
      if (receivers[i])
      {
	celData ret1;
	bool rc = receivers[i]->ReceiveMessage (msg_id, sender, ret1, params);
	if (rc)
	{
	  handled = true;
	  if (ret1.type != CEL_DATA_NONE && ret) ret->Push (ret1);
	}
	i++;
      }
      else
      {
	receivers.DeleteIndex (i);
      }
    }
    return handled;
  }

  csStringID GetMessageID () const { return msg_id; }
  const csString& GetMessage () const { return message; }

  void AddReceiver (iMessageReceiver* receiver);

  void RemoveReceiver (iMessageReceiver* receiver)
  {
    size_t i = 0;
    while (i < receivers.GetSize ())
    {
      if (receivers[i] == receiver)
	receivers.DeleteIndex (i);
      else
	i++;
    }
  }

  iMessageSender* GetSender () const { return sender; }
};

// SafeCopyElementHandler has to be used here, as otherwise the weakref's in
// the celMessageSubscription get memcopyed and thats not allowed.
typedef csHash<celMessageSubscription,csPtrKey<iMessageReceiver>,
	CS::Memory::AllocatorMalloc, 
  	csArraySafeCopyElementHandler< 
        	CS::Container::HashElement<celMessageSubscription,
		csPtrKey<iMessageReceiver> > > >
  celSubscriptions;

/**
 * Helper class for a message channel.
 */


class CEL_CELTOOL_EXPORT celMessageChannel : public scfImplementation1<
					     celMessageChannel, iMessageChannel>
{
private:
  csArray<celMessageSubscription,csArraySafeCopyElementHandler<celMessageSubscription> > subscriptionQueue;
  csArray<celMessageSubscription,csArraySafeCopyElementHandler<celMessageSubscription> > unsubscriptionQueue;
  csWeakRef<iCelPlLayer> pl;
  csRefArray<celMessageDispatcher> messageDispatchers;
  celSubscriptions messageSubscriptions;
  int sending;
public:
  celMessageChannel () : scfImplementationType (this) { sending = 0; }
  virtual ~celMessageChannel () { RemoveMessageDispatchers (); }

  virtual iMessageChannel* QueryMessageChannel ()
  {
    return static_cast<iMessageChannel*> (this);
  }
  /// Create a message dispatcher that supports a receiver filter.
  virtual csRef<iMessageDispatcher> CreateMessageDispatcher (
      iMessageSender* sender, const char* msg_id,
      iMessageReceiverFilter* receiver_filter = 0);
  virtual void RemoveMessageDispatcher (iMessageDispatcher* msgdisp);
  virtual void Subscribe (iMessageReceiver* receiver, const char* mask);
  virtual void Unsubscribe (iMessageReceiver* receiver, const char* mask = 0);
  virtual bool SendMessage (const char* msgid,
      iMessageSender* sender, iCelParameterBlock* params,
      iCelDataArray* ret = 0);
  void RemoveMessageDispatchers ();
  void SetPL (iCelPlLayer* pl) { celMessageChannel::pl = pl; }
};

#endif // __CEL_CELTOOL_STDMSGCHANNEL__

