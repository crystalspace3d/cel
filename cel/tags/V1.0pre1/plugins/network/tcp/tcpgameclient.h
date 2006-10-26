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

#ifndef __CEL_GAMECLIENT_TCP__
#define __CEL_GAMECLIENT_TCP__

#include "nl.h"
#include "physicallayer/nettypes.h"
#include "physicallayer/network.h"
#include "plugins/network/tcp/tcputils.h"

struct iVirtualClock;

CS_PLUGIN_NAMESPACE_BEGIN(celTCPNetwork)
{

struct celTCPGame;
struct celTCPGameFactory;
struct NetworkLinkClientData;

class celTCPGameClient : public csObject
{
private:
  iObjectRegistry* object_reg;

  celTCPGame* game;
  celTCPGameFactory* factory;
  celGameClientManager* manager;
  csRef<iVirtualClock> vc;

  celTCPCachedSocket* socket_cache;
  csArray<celClientEventData*> client_events;
  csHash<iCelEntity*, uint> linked_entities;
  csHash<NetworkLinkClientData*, uint> controlled_entities;

  NLsocket client_socket;
  celPlayer player;
  NLaddress server_address;
  celServerNetworkState connection_state;
  csTicks server_timeout;
  // 1 not connected, 2 connect validated, 3 really connected and validating data, 
  // 4 loading level, 5 ready, (6 awaiting snapshot), 6 playing, -1 no more try to connect
  char connecting_state;
  csTicks packet_send_time;
  int32 server_time_delta;
  csTicks connection_latency;

  csTicks server_time1;
  csTicks server_time2;
  csTicks client_time1;
  csTicks client_time2;

public:
  SCF_DECLARE_IBASE_EXT (csObject);

  celTCPGameClient (iObjectRegistry* object_reg, celTCPGameFactory* factory);
  virtual ~celTCPGameClient ();

  bool InitializeClient (celTCPGame* game, celPlayer* player);
  void UpdateNetwork ();
  void UpdateConnecting ();
  void ReadServerSocket (csTicks snapshot_time);
  void WriteServerSocket (csTicks snapshot_time);
  bool GetClientIPAddress (csString& hostname, uint16& port_nb);

  virtual void RegisterClientManager (celGameClientManager* manager)
    { celTCPGameClient::manager = manager; }
  virtual celPlayer* GetPlayer ()
    { return &player; }
  virtual void UpdatePlayer (celPlayer* player);
  virtual csTicks ConvertServerClock (csTicks server_time) const;
  virtual uint GetMappedID (uint entity_id) const;
  virtual iCelEntity* GetMappedEntity (uint entity_id) const;
  virtual void SetReady ();
  virtual void LaunchClientEvent (celClientEventData &event_data);
  virtual void SetNetworkPeriod (csTicks period);
  virtual void SetMaximumBandwidth (size_t width);
  virtual void SetServerTimeOut (csTicks timeout);
  virtual void GetNetworkStats (celNetworkServerStats& stats) const;

  struct TCPGameClient : public iCelGameClient
  {
    SCF_DECLARE_EMBEDDED_IBASE (celTCPGameClient);
    virtual void RegisterClientManager (celGameClientManager* manager)
    { scfParent->RegisterClientManager (manager); }
    virtual celPlayer* GetPlayer ()
    { return scfParent->GetPlayer (); }
    virtual void UpdatePlayer (celPlayer* player)
    { scfParent->UpdatePlayer (player); }
    virtual csTicks ConvertServerClock (csTicks server_time) const
    { return scfParent->ConvertServerClock (server_time); }
    virtual uint GetMappedID (uint entity_id) const
    { return scfParent->GetMappedID (entity_id); }
    virtual iCelEntity* GetMappedEntity (uint entity_id) const
    { return scfParent->GetMappedEntity (entity_id); }
    virtual void SetReady ()
    { scfParent->SetReady (); }
    virtual void LaunchClientEvent (celClientEventData &event_data)
    { scfParent->LaunchClientEvent (event_data); }
    virtual void SetNetworkPeriod (csTicks period)
    { scfParent->SetNetworkPeriod (period); }
    virtual void SetMaximumBandwidth (size_t width)
    { scfParent->SetMaximumBandwidth (width); }
    virtual void SetServerTimeOut (csTicks timeout)
    { scfParent->SetServerTimeOut (timeout); }
    virtual void GetNetworkStats (celNetworkServerStats& stats) const
    { scfParent->GetNetworkStats (stats); }
  } scfiCelGameClient;
  friend struct TCPGameClient;
};

class NetworkLinkClientData
{
 public:
  iCelEntity* linked_entity;
  uint server_entity_id;
  csTicks update_period;
  csTicks last_update;
};

}
CS_PLUGIN_NAMESPACE_END(celTCPNetwork)

#endif // __CEL_GAMECLIENT_TCP__
