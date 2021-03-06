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

#include "cssysdef.h"
#include "csutil/util.h"
#include "csutil/stringarray.h"
#include "csutil/set.h"
#include "celtool/stdmsgchannel.h"

//---------------------------------------------------------------------------

void celMessageDispatcher::AddReceiver (iMessageReceiver* receiver)
{
  if ((!receiver_filter) || receiver_filter->IsValidReceiver (receiver))
    receivers.Push (receiver);
}

//---------------------------------------------------------------------------

static bool Match (const csString& mask, const csString& msg)
{
  return msg.StartsWith (mask);
}

csRef<iMessageDispatcher> celMessageChannel::CreateMessageDispatcher (
      iMessageSender* sender, csStringID id,
      iMessageReceiverFilter* receiver_filter)
{
  if (!pl) return 0;
  csRef<celMessageDispatcher> msgdisp;
  csString message = pl->FetchString (id);;
  msgdisp.AttachNew (new celMessageDispatcher (id, message, sender,
	receiver_filter));
  messageDispatchers.Push (msgdisp);

  // Check all subscriptions and see if this new message dispatcher should
  // already get some receivers.
  celSubscriptions::GlobalIterator it = messageSubscriptions.GetIterator ();
  while (it.HasNext ())
  {
    celMessageSubscription& sub = it.Next ();
    if (sub.receiver && Match (sub.mask, message))
      msgdisp->AddReceiver (sub.receiver);
  }

  return msgdisp;
}

void celMessageChannel::RemoveMessageDispatcher (iMessageDispatcher* msgdisp)
{
  size_t i;
  for (i = 0 ; i < messageDispatchers.GetSize () ; i++)
    if (msgdisp == static_cast<iMessageDispatcher*> (messageDispatchers[i]))
    {
      iMessageSender* sender = messageDispatchers[i]->GetSender ();
      if (sender) sender->MessageDispatcherRemoved (msgdisp);
      messageDispatchers.DeleteIndex (i);
      return;
    }
}

void celMessageChannel::RemoveMessageDispatchers ()
{
  while (messageDispatchers.GetSize () > 0)
  {
    csRef<celMessageDispatcher> msgdisp = messageDispatchers.Pop ();
    iMessageSender* sender = msgdisp->GetSender ();
    if (sender) sender->MessageDispatcherRemoved (msgdisp);
  }
}

void celMessageChannel::Subscribe (iMessageReceiver* receiver, const char* mask)
{
  celMessageSubscription newsub;
  newsub.receiver = receiver;
  newsub.mask = mask;

  // First check all subscriptions for this receiver and see if we
  // are already subscribed.
  celSubscriptions::Iterator it = messageSubscriptions.GetIterator (
      receiver);
  while (it.HasNext ())
  {
    celMessageSubscription& sub = it.Next ();
    if (sub.mask == newsub.mask)
    {
      // There is a subscription that completely matches this subscription
      // so we do nothing.
      return;
    }
  }

  messageSubscriptions.Put (receiver, newsub);

  // Check all message dispatchers and see if this new subscription should
  // update some of those message dispatchers.
  size_t i;
  for (i = 0 ; i < messageDispatchers.GetSize () ; i++)
  {
    celMessageDispatcher* msgdisp = messageDispatchers[i];
    const csString& message = msgdisp->GetMessage ();
    if (Match (newsub.mask, message))
      msgdisp->AddReceiver (newsub.receiver);
  }
}

void celMessageChannel::Unsubscribe (iMessageReceiver* receiver,
    const char* mask)
{
  size_t i;
  csString maskStr = mask;
  csStringArray retained_masks;
  if (maskStr.IsEmpty ())
  {
    // If mask is empty then we can just remove all subscriptions for
    // this receiver.
    messageSubscriptions.DeleteAll (receiver);
  }
  else
  {
    // Array for holding subscriptions that we want to remove. We do it later
    // as it is not safe to delete elements while iterating.
    csArray<celMessageSubscription,csArraySafeCopyElementHandler<celMessageSubscription> > to_remove;
    // Keep track of subscription masks with this receiver that were not removed
    // (because this mask didn't match).
    celSubscriptions::Iterator it = messageSubscriptions.GetIterator(receiver);
    while(it.HasNext())
    {
       celMessageSubscription &sub = it.Next();
       if (sub.receiver == receiver)
       {
         if (mask == 0 || Match (maskStr, sub.mask))
           to_remove.Push(sub);
         else
           retained_masks.Push (sub.mask);
       }
    }
    for (i=0 ; i < to_remove.GetSize() ; i++)
    {
       messageSubscriptions.Delete (receiver, to_remove.Get(i));
    }
  }

  // It is a common case that there will be no more subscriptions left for
  // the given receiver. We optimize for this case by traversing all message
  // dispatchers and removing the receiver from it unconditionally.
  for (i = 0 ; i < messageDispatchers.GetSize () ; i++)
    messageDispatchers[i]->RemoveReceiver (receiver);

  // However, in case there are still some subscriptions left we need to
  // check the masks again and re-add them.
  for (i = 0 ; i < retained_masks.GetSize () ; i++)
  {
    csString rmask = retained_masks[i];

    size_t j;
    // Check all message dispatchers and see if this subscription should
    // update some of those message dispatchers.
    for (j = 0 ; j < messageDispatchers.GetSize () ; j++)
    {
      celMessageDispatcher* msgdisp = messageDispatchers[j];
      const csString& message = msgdisp->GetMessage ();
      if (Match (rmask, message))
        msgdisp->AddReceiver (receiver);
    }
  }
}

bool celMessageChannel::SendMessage (csStringID id,
      iMessageSender* sender, iCelParameterBlock* params,
      iCelDataArray* ret)
{
  csString msgid = pl->FetchString (id);

  // Two passes. In the first pass we will gather all receivers in a set
  // so that every receiver will only get this message once.
  csSet<csPtrKey<iMessageReceiver> > receivers;
  celSubscriptions::GlobalIterator it = messageSubscriptions.GetIterator ();
  while (it.HasNext ())
  {
    celMessageSubscription& sub = it.Next ();
    iMessageReceiver* receiver = sub.receiver;
    if (receiver && Match (sub.mask, msgid))
      receivers.Add (receiver);
  }

  // In the second pass we actually send the messages.
  bool handled = false;
  csSet<csPtrKey<iMessageReceiver> >::GlobalIterator rit = receivers.GetIterator ();
  while (rit.HasNext ())
  {
    iMessageReceiver* receiver = rit.Next ();
    celData ret1;
    bool rc = receiver->ReceiveMessage (id, sender, ret1, params);
    if (rc)
    {
      handled = true;
      if (ret1.type != CEL_DATA_NONE && ret) ret->Push (ret1);
    }
  }

  return handled;
}

