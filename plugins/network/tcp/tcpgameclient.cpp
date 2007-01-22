/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Christian Van Brussel

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

//CS includes
#include "iutil/virtclk.h"

//CEL includes
#include "physicallayer/pl.h"
#include "include/celtool/persisthelper.h"
#include "plugins/network/tcp/tcpgame.h"
#include "plugins/network/tcp/tcpgamefactory.h"
#include "plugins/network/tcp/tcpgameclient.h"

CS_PLUGIN_NAMESPACE_BEGIN(celTCPNetwork)
{

SCF_IMPLEMENT_IBASE_EXT (celTCPGameClient)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iCelGameClient)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celTCPGameClient::TCPGameClient)
  SCF_IMPLEMENTS_INTERFACE (iCelGameClient)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

// default value to be used for the server timeout set with SetServerTimeOut
#define DEFAULT_SERVER_TIMEOUT 60000

celTCPGameClient::celTCPGameClient (iObjectRegistry* object_reg, celTCPGameFactory* factory)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiCelGameClient);
  celTCPGameClient::object_reg = object_reg;
  celTCPGameClient::factory = factory;
  manager = 0;
  client_socket = 0;
  socket_cache = 0;
  connection_state = CEL_NET_SERVER_NOT_CONNECTED;
  connecting_state = 1;
  server_timeout = DEFAULT_SERVER_TIMEOUT;
  connection_latency = 0;

  // TODO: use virtual clock to send time
  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  if (!vc) ReportError (object_reg, "iVirtualClock Missing!");
}

celTCPGameClient::~celTCPGameClient ()
{
  // close client socket
  // TODO: this does not function correctly and the socket is not always really closed cleanly
  nlEnable (NL_BLOCKING_IO);
  if (client_socket)
    nlClose (client_socket);
  nlDisable (NL_BLOCKING_IO);

  size_t i;
  for (i = 0; i < client_events.Length (); i++)
    delete client_events[i];
  client_events.DeleteAll ();

  linked_entities.DeleteAll ();

  delete socket_cache;

  SCF_DESTRUCT_EMBEDDED_IBASE (scfiCelGameClient);
}

bool celTCPGameClient::InitializeClient (celTCPGame* game, celPlayer* player)
{
  // copy game data
  celTCPGameClient::game = game;
  if (!player) return ReportError (object_reg, "No player defined in client initialization");
  celTCPGameClient::player = *player;

  // TODO: check if we need to read the port number from the IPv6 address of the player

  // read the ip address of the server
  if (!IsAddressEmpty (&game->game_info.ip_address[0]))
    CopyAddress (&game->game_info.ip_address[0], server_address);
  else if (!game->game_info.hostname.IsEmpty ())
  {
    if (!nlGetAddrFromName (game->game_info.hostname.GetData (), &server_address)
	|| !server_address.valid)
    {
      celServerNetworkState previous_state = connection_state;
      connection_state = CEL_NET_SERVER_INVALID_HOSTNAME;
      factory->manager->ServerNetworkStateChanged (CEL_NET_SERVER_INVALID_HOSTNAME, 
						   previous_state, game->game_info.hostname);
      return false;
    }      
  }
  else return ReportError (object_reg, "The IP address of the server is not defined");

  // set the port number
  nlSetAddrPort (&server_address, game->game_info.port_nb);

  return true;
}

void celTCPGameClient::UpdateNetwork ()
{
  csTicks snapshot_time = csGetTicks ();

  // check if we are in default loop
  if (connecting_state >= 6)
  {
    int update_socket_result = socket_cache->UpdateSocket ();

    // if connection is closed
    if (update_socket_result == 0)
    {
      // TODO: close game
      delete socket_cache;
      socket_cache = 0;
      nlClose (client_socket);
      connecting_state = -1;

      // notify connection state to the factory manager
      celServerNetworkState previous_state = connection_state;
      connection_state = CEL_NET_SERVER_DISCONNECTED;
      factory->manager->ServerNetworkStateChanged (connection_state, previous_state);
      return;
    }

    // TODO: if connection is broken and timed out

    // if connection is broken
    if (update_socket_result == -1)
    {
      delete socket_cache;
      socket_cache = 0;
      nlEnable (NL_BLOCKING_IO);
      nlClose (client_socket);
      nlDisable (NL_BLOCKING_IO);
      connecting_state = 1;

      // TODO: we should wait for the client socket to be released before re-allocating it
      // the next line fixes the problem but the player should be able to fix the port of the client
      player.port_nb = 0;

      // notify connection state to the factory manager
      celServerNetworkState previous_state = connection_state;
      connection_state = CEL_NET_SERVER_UNREACHABLE;
      factory->manager->ServerNetworkStateChanged (connection_state, previous_state);
      return;
    }

    // read socket
    ReadServerSocket (snapshot_time);

    // write socket
    if (connecting_state != -1)
    {
      WriteServerSocket (snapshot_time);
      socket_cache->UpdateSocket ();
    }

    return;
  }

  // if connection is definitively lost
  // TODO: if connecting state == -1 for the first time, then close client
  if (connecting_state == -1)
    return;

  // if connecting
  UpdateConnecting ();
}

void celTCPGameClient::UpdateConnecting ()
{
  // establish the tcp connection from the client to the server if needed
  if (connecting_state == 1)
  {
    // notify connection state to the factory manager
    if (connection_state != CEL_NET_SERVER_TRYING_CONNECTION)
    {
      celServerNetworkState previous_state = connection_state;
      connection_state = CEL_NET_SERVER_TRYING_CONNECTION;
      factory->manager->ServerNetworkStateChanged (CEL_NET_SERVER_TRYING_CONNECTION, 
						   previous_state);
    }

    // TODO: wait some time before retrying connection

    // open socket
    client_socket = nlOpen (player.port_nb, NL_RELIABLE);
    if (client_socket == NL_INVALID)
    {
      connecting_state = -1;
      csString txt = "Failed to open a socket on port number ";
      txt += player.port_nb;
      ReportNetworkError (object_reg, txt);
      return;
    }

    // launch connection
    if (!nlConnect (client_socket, &server_address))
    {
      if (nlGetError () == NL_SYSTEM_ERROR)
      {
	ReportNetworkError (object_reg,
			    "System error while trying to establish connection to the server");
	return;
      }
      else return;
    }
    else connecting_state = 2;
  }

  if (connecting_state == 2)
  {
    // test if connection is still pending
    NLbyte buffer[128];
    NLint size = nlRead (client_socket, buffer, 1);
    if (size == NL_INVALID)
    {
      if (nlGetError () == NL_CON_PENDING) return;
      else
      {
	// connection no more valid
	nlEnable (NL_BLOCKING_IO);
	nlClose (client_socket);
	nlDisable (NL_BLOCKING_IO);
	connecting_state = 1;
	return;
      }
    }

    // notify connection state to the factory manager
    celServerNetworkState previous_state = connection_state;
    connection_state = CEL_NET_SERVER_CONNECTING;
    factory->manager->ServerNetworkStateChanged (CEL_NET_SERVER_CONNECTING, 
						 previous_state);

    connecting_state = 3;

    socket_cache = new celTCPCachedSocket (client_socket);

    // send own data
    celNetworkBuffer* context = new celNetworkBuffer ();
    context->Write (factory->game_name);
    context->Write (factory->protocol_version);
    context->Write (factory->game->game_info.password);
    context->Write (player.player_id);
    context->Write (player.player_name);
    // TODO: send player custom data
    socket_cache->SendPacket (context);

    // mark the time at which the packet is sent
    packet_send_time = csGetTicks ();
    //packet_send_time = vc->GetElapsedTicks ();
  }

  // check if connection is still valid
  if (socket_cache->UpdateSocket () < 1)
  {
    delete socket_cache;
    socket_cache = 0;
    nlEnable (NL_BLOCKING_IO);
    nlClose (client_socket);
    nlDisable (NL_BLOCKING_IO);
    connecting_state = 1;

    // TODO: we should wait for the client socket to be released before re-allocating it
    // this fixes the problem but the player should be able to fix the port of the client
    player.port_nb = 0;

    // notify connection state to the factory manager
    celServerNetworkState previous_state = connection_state;
    connection_state = CEL_NET_SERVER_LOST;
    factory->manager->ServerNetworkStateChanged (CEL_NET_SERVER_LOST, 
						 previous_state);

    return;
  }

  if (connecting_state == 3)
  {
    if (!socket_cache->IsReceivedPacket ()) return;
    celNetworkBuffer* packet = socket_cache->GetReceivedPacket ();
    uint8 answer_type;
    packet->Read (answer_type);
    
    bool to_delete = false;
    csString reason = "";
    celGameInfo temp_game_info;
    int i;
    csTicks packet_receive_time = csGetTicks ();
    //csTicks packet_receive_time = vc->GetElapsedTicks ();
    switch (answer_type)
    {
    case CEL_NET_SERVER_REJECTED_BAD_GAME:
    case CEL_NET_SERVER_REJECTED_BAD_PROTOCOL:
    case CEL_NET_SERVER_REJECTED_UNAUTHORIZED:
      to_delete = true;
      packet->Read (reason);
      break;
    case CEL_NET_SERVER_REJECTED_BAD_PASSWORD:
    case CEL_NET_SERVER_REJECTED_SINGLEPLAYER:
    case CEL_NET_SERVER_REJECTED_MAX_PLAYERS:
      to_delete = true;
      break;
    case CEL_NET_SERVER_LOADING_DATA:
      // read server time and compute client-server time delta
      csTicks server_time;
      packet->Read (server_time);
      connection_latency = (packet_receive_time - packet_send_time) / 2;
      server_time_delta = (int32) server_time - (int32) packet_receive_time - (int32) connection_latency;

      server_time1 = server_time;
      client_time1 = packet_receive_time;

      // read packet
      packet->Read (player.player_id);
      packet->Read (player.player_name);
      packet->Read (player.hostname);
      i = 0;
      for ( ; i < 32; i++)
	packet->Read (player.ip_address[i]);
      packet->Read (player.port_nb);

      packet->Read (temp_game_info.game_id);
      packet->Read (temp_game_info.game_name);
      packet->Read (temp_game_info.hostname);
      packet->Read (temp_game_info.port_nb);
      packet->Read (temp_game_info.max_players);
      packet->Read (temp_game_info.custom_data);
      if (!game->IsServerAvailable ())
	game->game_info = temp_game_info;

      connecting_state = 4;
      
      // TODO: if the connection was broken, we do not need to re-load the level, should need to clean all network links
      // init the manager of the game factory
      if (!factory->manager->InitClient (&game->scfiCelGame))
      {
	to_delete = true;
	ReportError (object_reg, "The client manager failed its initialization");
      }

      // call the client manager with the validated data of the player
      if (manager) manager->PlayerValidated (&player);
      break;

    default:
      ReportError (object_reg, "Answer from the server not recognized!");
      to_delete = true;
    }

    if (to_delete)
    {
      delete socket_cache;
      socket_cache = 0;
      //nlClose (client_socket);
      connecting_state = -1;
    }

    // notify connection state to the factory manager
    celServerNetworkState previous_state = connection_state;
    connection_state = (celServerNetworkState) answer_type;
    factory->manager->ServerNetworkStateChanged ((celServerNetworkState) answer_type, 
						 previous_state, reason);
    delete packet;
    return;
  }

  if (connecting_state == 5)
  {
    // notify the server that the client is ready
    // TODO: and awaiting for snapshot?
    celNetworkBuffer* context = new celNetworkBuffer ();
    uint8 answer_type = CEL_NET_PLAYER_PLAYING;
    context->Write (answer_type);
    socket_cache->SendPacket (context);
    socket_cache->UpdateSocket ();
    connecting_state = 6;

    // notify connection state to the factory manager
    celServerNetworkState previous_state = connection_state;
    connection_state = CEL_NET_SERVER_PLAYING;
    factory->manager->ServerNetworkStateChanged (connection_state, previous_state);
  }
}

void celTCPGameClient::ReadServerSocket (csTicks snapshot_time)
{
  while (socket_cache->IsReceivedPacket ())
  {
    celNetworkBuffer* new_packet = socket_cache->GetReceivedPacket ();

    // read server snapshot time and compute latency
    csTicks server_snapshot_time;
    csTicks converted_server_snapshot_time = ConvertServerClock (server_snapshot_time);

    new_packet->Read (server_snapshot_time);
    server_time2 = server_snapshot_time;
    client_time2 = csGetTicks ();
    //client_time2 = vc->GetElapsedTicks ();
    connection_latency += ((int32)client_time2 - (int32)client_time1) - ((int32)server_time2 - (int32)server_time1);
    server_time1 = server_time2;
    client_time1 = client_time2;

    while (new_packet->IsReadDataRemaining ())
    {
      uint8 data_type;
      new_packet->Read (data_type);

      int i = 0;
      celClientEventType event_type;
      csTicks event_time;
      csRef<iCelDataBuffer> data_buffer;
      celStandardPersistentDataList* data_list;
      csString reason;
      celServerNetworkState previous_state;
      uint8 links_nb;

      switch (data_type)
      {
      case SERVER_DATA_KICKED:
	new_packet->Read (reason);
	
	// notify connection state to the factory manager
	previous_state = connection_state;
	connection_state = CEL_NET_SERVER_KICKED;
	factory->manager->ServerNetworkStateChanged (connection_state, 
						     previous_state, reason);

	connecting_state = -1;
	return;

      case SERVER_DATA_EVENT:
	uint8 events_nb;
	new_packet->Read (events_nb);

	for ( i = 0; i < events_nb; i++)
	{
	  // read event
	  new_packet->Read (event_type);
	  new_packet->Read (event_time);
	  new_packet->Read (data_buffer);

	  // call manager
	  if (manager) manager->HandleServerEvent (event_type, event_time, 
						   data_buffer);
	}
	break;

      case SERVER_DATA_LINK_INIT:
	new_packet->Read (links_nb);

	for ( i = 0; i < links_nb; i++)
	{
	  // read link data
	  bool player_controlled;
	  new_packet->Read (player_controlled);
	  csTicks update_period;
	  if (player_controlled)
	    new_packet->Read (update_period);
	  uint8 link_type;
	  new_packet->Read (link_type);
	  uint entity_id;
	  new_packet->Read (entity_id);
	  data_list = new celStandardPersistentDataList ();
	  new_packet->Read (data_list);

	  // call manager
	  iCelEntity* entity = 0;
	  if (manager)
	    entity = manager->CreateNetworkLinkTarget (link_type, 0, 
						       data_list, player_controlled);

	  // add to list of entities
	  if (player_controlled)
	  {
	    NetworkLinkClientData* link_data = new NetworkLinkClientData ();
	    link_data->linked_entity = entity;
	    link_data->server_entity_id = entity_id;
	    link_data->update_period = update_period;
	    link_data->last_update = snapshot_time;
	    controlled_entities.Put (entity_id, link_data);
	  }
	  linked_entities.Put (entity_id, entity);

	  delete data_list;
	}
	break;

      case SERVER_DATA_LINK_UPDATE:
	new_packet->Read (links_nb);

	for ( i = 0; i < links_nb; i++)
	{
	  // read link data
	  uint entity_id;
	  new_packet->Read (entity_id);
	  data_list = new celStandardPersistentDataList ();
	  new_packet->Read (data_list);

	  // update entity
	  if (!factory->game->IsServerAvailable ())
	  {
	    iCelEntity* entity = linked_entities.Get (entity_id, 0);
	    if (entity)
	      if (SetEntityPersistentData (entity, 
					   converted_server_snapshot_time, 
					   data_list, 
					   CEL_PERSIST_TYPE_SERVER_CONTROL)
		  == CEL_PERSIST_RESULT_ERROR)
		ReportError (object_reg, 
			     "Error while setting persistent data of entity %d", 
			     entity->GetID ());
	  }
	  delete data_list;
	}
	break;

      case SERVER_DATA_LINK_CONTROL:
	new_packet->Read (links_nb);

	for ( i = 0; i < links_nb; i++)
	{
	  // read link data
	  uint entity_id;
	  new_packet->Read (entity_id);
	  data_list = new celStandardPersistentDataList ();
	  new_packet->Read (data_list);

	  // update entity
	  if (!factory->game->IsServerAvailable ())
	  {
	    iCelEntity* entity = linked_entities.Get (entity_id, 0);
	    if (entity)
	      SetEntityPersistentData (entity, 
				       converted_server_snapshot_time, 
				       data_list, CEL_PERSIST_TYPE_SERVER_FORCING);
	  }
	  delete data_list;
	}
	break;

      case SERVER_DATA_LINK_REMOVE:
	new_packet->Read (links_nb);

	for ( i = 0; i < links_nb; i++)
	{
	  // read link data
	  uint entity_id;
	  new_packet->Read (entity_id);

	  // remove entity
	  iCelEntity* entity = linked_entities.Get (entity_id, 0);
	  linked_entities.DeleteAll (entity_id);
	  controlled_entities.DeleteAll (entity_id);

	  // call client manager
	  if (manager)
	    manager->NetworkLinkRemoved (server_snapshot_time, entity);
	}
	break;

      default:
	ReportError (object_reg, "Invalid data type received from server");
	return;
      }
    }

    delete new_packet;
  }
}

void celTCPGameClient::WriteServerSocket (csTicks snapshot_time)
{
  // TODO: check frequency
  // TODO: do not allocate a new buffer each time, re-use another one
  celNetworkBuffer* packet = new celNetworkBuffer ();

  // write client events
  uint8 list_size = client_events.Length ();
  if (list_size > 0)
  {
    uint8 data_type = CLIENT_DATA_EVENT;
    packet->Write (data_type);
    packet->Write (list_size);

    csArray<celClientEventData*>::Iterator events_it = client_events.GetIterator ();
    while (events_it.HasNext ())
    {
      celClientEventData* event = events_it.Next ();
      packet->Write (event->event_type);
      packet->Write (event->event_time);
      packet->Write (event->event_data);
      delete event;
    }
  }
  client_events.DeleteAll ();

  // prepare network links controlled by the player to be updated at this snapshot
  csArray<NetworkLinkClientData*> network_links;
  csHash<NetworkLinkClientData*, uint>::GlobalIterator links_it = controlled_entities.GetIterator ();
  if (!factory->game->IsServerAvailable ())
    while (links_it.HasNext ())
    {
      NetworkLinkClientData* link_data = links_it.Next ();
      if (snapshot_time - link_data->last_update >= link_data->update_period)
	network_links.Push (link_data);
    }

  // write network links controlled by the player
  list_size = network_links.GetSize ();
  if (list_size > 0)
  {
    uint8 data_type = CLIENT_DATA_CONTROLLED_LINK;
    packet->Write (data_type);
    packet->Write (list_size);

    csArray<NetworkLinkClientData*>::Iterator links_it = network_links.GetIterator ();
    while (links_it.HasNext ())
    {
      NetworkLinkClientData* link_data = links_it.Next ();
      packet->Write (link_data->server_entity_id);
      csBitArray bit_array;
      csRef<iCelPersistentDataList> data_list = GetEntityPersistentData (link_data->linked_entity, 
									 &bit_array, 
									 CEL_PERSIST_TYPE_CLIENT_CONTROL);
      packet->Write (data_list);

      // mark last update time
      link_data->last_update = snapshot_time;
    }
  }

  // send packet
  if (packet->GetSize () > 0)
    socket_cache->SendPacket (packet);
  else delete packet;
}

bool celTCPGameClient::GetClientIPAddress (csString& hostname, uint16& port_nb)
{
  NLaddress client_address;
  if (!nlGetLocalAddr (client_socket, &client_address))
    return false;
  else
  {
    if (!ReadAddressData (client_address, hostname, port_nb))
      return false;
  }
  return true;
}

void celTCPGameClient::UpdatePlayer (celPlayer* player)
{
}

csTicks celTCPGameClient::ConvertServerClock (csTicks server_time) const
{
  return server_time - server_time_delta;
}

uint celTCPGameClient::GetMappedID (uint entity_id) const
{
  iCelEntity* entity = linked_entities.Get (entity_id, 0);
  if (!entity) return 0;
  return entity->GetID ();
}

iCelEntity* celTCPGameClient::GetMappedEntity (uint entity_id) const
{
  iCelEntity* entity = linked_entities.Get (entity_id, 0);
  return entity;
}

void celTCPGameClient::SetReady ()
{
  if (connecting_state == 4)
    connecting_state = 5;
}

void celTCPGameClient::LaunchClientEvent (celClientEventData &event_data)
{
  celClientEventData* new_event = new celClientEventData (event_data);
  client_events.Push (new_event);
}

void celTCPGameClient::SetNetworkPeriod (csTicks period)
{
  // @@@ TODO
}

void celTCPGameClient::SetMaximumBandwidth (size_t width)
{
  // @@@ TODO
}

void celTCPGameClient::SetServerTimeOut (csTicks timeout)
{
  server_timeout = timeout;
}

void celTCPGameClient::GetNetworkStats (celNetworkServerStats& stats) const
{
  if (connection_state != CEL_NET_SERVER_PLAYING)
    stats.latency = 999;
  else
    stats.latency = connection_latency > 999 ? 999 : connection_latency;
  // TODO: other stats
}

}
CS_PLUGIN_NAMESPACE_END(celTCPNetwork)

