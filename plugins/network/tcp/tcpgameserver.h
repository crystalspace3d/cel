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

#ifndef __CEL_GAMESERVER_TCP__
#define __CEL_GAMESERVER_TCP__

#include "iutil/comp.h"

#include "nl.h"

#include "physicallayer/nettypes.h"
#include "physicallayer/network.h"

struct iVirtualClock;

CS_PLUGIN_NAMESPACE_BEGIN(celTCPNetwork)
{

struct celTCPGame;
struct celTCPGameFactory;
struct celIncomingPlayer;
struct celPlayerData;
struct KickedPlayer;

class celPlayerList : public scfImplementation1<
	celPlayerList, iCelPlayerList>
{
private:
  csArray<celPlayer*> players;

public:
  celPlayerList ();
  virtual ~celPlayerList ();

  virtual size_t GetCount () const;
  virtual celPlayer* Get (size_t index) const;
  virtual size_t Add (celPlayer* player);
  virtual bool Remove (celPlayer* player);
  virtual bool Remove (size_t n);
  virtual void RemoveAll ();
  virtual size_t Find (celPlayer* player) const;
};

class celTCPGameServer : public scfImplementationExt1<
	celTCPGameServer, csObject, iCelGameServer>
{
private:
  iObjectRegistry* object_reg;

  celTCPGame* game;
  celTCPGameFactory* factory;
  celGameServerManager* manager;
  csRef<iVirtualClock> vc;

  csArray<celIncomingPlayer*> incoming_players;
  csHash<celPlayerData*, uint32> players_data;
  csArray<KickedPlayer> players_kicked;
  celPlayerList player_list;

  NLsocket server_socket;
  bool is_game_admin;
  celPlayer game_admin;
  csTicks client_timeout;
  csTicks network_update_period;
  csTicks last_update;

public:
  celTCPGameServer (iObjectRegistry* object_reg, celTCPGameFactory* factory);
  virtual ~celTCPGameServer ();

  bool InitializeServer (celTCPGame* game, celPlayer* game_admin);
  void UpdateNetwork ();
  void UpdateConnectingPlayers (csTicks snapshot_time);
  void UpdatePlayerSockets (csTicks snapshot_time);
  void ReadPlayerSockets (csTicks snapshot_time);
  void WritePlayerSockets (csTicks snapshot_time);
  void KickPlayers (csTicks snapshot_time);

  virtual void RegisterServerManager (celGameServerManager* manager)
    { celTCPGameServer::manager = manager; }
  virtual void SetAdministrator (celPlayer* new_admin);
  virtual celPlayer* GetAdministrator ()
    { return &game_admin; }
  virtual iCelPlayerList* GetPlayerList ()
    { return &player_list; }
  virtual celPlayerNetworkState GetPlayerState (celPlayer* player);
  virtual void KickPlayer (celPlayer* player, csString reason);
  virtual iCelPlayerList* CreateChannel (csStringID channel_id);
  virtual void RemoveChannel (csStringID channel_id);
  virtual void LaunchServerEvent (celPlayer* player, 
        celServerEventData &event_data);
  virtual void LaunchServerEvent (csStringID channel_id, 
        celServerEventData &event_data);
  virtual void SetNetworkLink (celPlayer* player, 
        celNetworkLinkData &link_data, bool player_controlled);
  virtual void ChangeLinkControl (iCelEntity* entity, celPlayer* player, 
        bool player_controlled);
  virtual void SetNetworkLink (csStringID channel_id, 
        celNetworkLinkData &link_data);
  virtual void RemoveNetworkLink (iCelEntity* entity, celPlayer* player);
  virtual void RemoveNetworkLink (iCelEntity* entity, csStringID channel_id);
  virtual void ChangeLevel (celGameInfo* new_game);
  virtual void SetNetworkPeriod (csTicks period)
    { network_update_period = period; }
  virtual void SetMaximumBandwidth (size_t width);
  virtual void SetClientTimeOut (csTicks timeout)
    { client_timeout = timeout; }
  virtual void GetNetworkPlayerStats (celPlayer* player, 
        celNetworkPlayerStats& stats) const;
  virtual void GetNetworkTotalStats (celNetworkPlayerTotalStats& stats) const;
};

class celIncomingPlayer
{
 public:
  celTCPCachedSocket* socket_cache;
  csTicks connection_start;
  bool to_delete;

  celIncomingPlayer () : 
    socket_cache (0),
    connection_start (0),
    to_delete (false)
    {}

  celIncomingPlayer (NLsocket socket)
  {
    socket_cache = new celTCPCachedSocket (socket);
    connection_start = 0;
    to_delete = false;
  }

  ~celIncomingPlayer ()
  {
    delete socket_cache;
  }
};

class NetworkLinkServerData
{
 public:
  // 0: new link, 1: pending link, 2: to be removed
  char link_state;
  bool player_controlled;
  celNetworkLinkData link_data;
  csTicks last_update;
};

class celPlayerData
{
 public:
  celPlayer player;
  celTCPCachedSocket* socket_cache;
  celPlayerNetworkState network_state;
  size_t max_bandwidth;
  csTicks unreachable_start;
  csArray<celServerEventData*> server_events;
  csHash<NetworkLinkServerData*, uint> network_links;

  celPlayerData ()
  {
    socket_cache = 0;
    max_bandwidth = 0;
    unreachable_start = 0;
  }
  ~celPlayerData ();

  /// Prepare the network links that have to be sent at this frame
  void GetNetworkLinks (csTicks snapshot_time, 
			csArray<NetworkLinkServerData*>& new_links, 
			csArray<NetworkLinkServerData*>& pending_links, 
			csArray<NetworkLinkServerData*>& controlled_links, 
			csArray<NetworkLinkServerData*>& removed_links);
};

class KickedPlayer
{
 public:
  celPlayer* player;
  csString reason;
};

}
CS_PLUGIN_NAMESPACE_END(celTCPNetwork)

#endif // __CEL_GAMESERVER_TCP__
