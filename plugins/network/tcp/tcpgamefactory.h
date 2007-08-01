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

#ifndef __CEL_GAMEFACT_TCP__
#define __CEL_GAMEFACT_TCP__

#include "csutil/eventnames.h"
#include "iutil/comp.h"
#include "iutil/event.h"
#include "iutil/eventh.h"
#include "iutil/objreg.h"

#include "physicallayer/nettypes.h"
#include "physicallayer/network.h"
#include "plugins/network/tcp/tcpgame.h"

CS_PLUGIN_NAMESPACE_BEGIN(celTCPNetwork)
{

/**
 * Implementation of the game manager using the classic 
 * client-server architecture over the TCP protocol. It may be 
 * used with a low number of players and in a local network. 
 * It is not designed to be used over the Internet, although it 
 * can work if the game is not too regarding to the network 
 * transmission quality. Its qualities are that it is really simple, 
 * and that it should handle easily the gateways and firewalls.
 */
class celTCPGameFactory : public iCelGameFactory
{
  friend class celTCPGameClient;
  friend class celTCPGameServer;

 protected:
  celGameFactoryManager* manager;
  csString game_name;
  csString protocol_version;
  celTCPGame* game;

 private:
  iObjectRegistry* object_reg;

 public:
  SCF_DECLARE_IBASE;

  celTCPGameFactory (iBase* parent);
  virtual ~celTCPGameFactory ();
  bool Initialize (iObjectRegistry* object_reg);

  bool HandleEvent (iEvent& ev);

  virtual void SetGameName (csString game_name)
    { celTCPGameFactory::game_name = game_name; }
  virtual csString GetGameName () const
    { return game_name; }
  virtual void SetProtocolVersion (const csString protocol_version)
    { celTCPGameFactory::protocol_version = protocol_version; }
  virtual csString GetProtocolVersion () const
    { return protocol_version; }
  virtual void RegisterGameFactoryManager (
        celGameFactoryManager* manager)
    { celTCPGameFactory::manager = manager; }
  virtual void StartSearchForGameList (celNetworkGameType game_type, 
        celGameInfo* filter = 0);
  virtual void StopSearchForGameList ();
  virtual iCelGame* GetCurrentGame () const;
  virtual bool CreateNewGame (celNetworkGameType game_type, 
	celGameInfo* game_info, celPlayer* game_admin = 0);
  virtual bool ConnectGame (celGameInfo* game_info, 
        celPlayer* player);
  virtual bool CloseGame ();
  virtual void StartRecordingGame (csString filename);
  virtual void StopRecordingGame ();
  virtual void StartPlayBackGame (csString filename, 
				  csTicks start_time, csTicks stop_time);

  struct Component : public iComponent
  {
    SCF_DECLARE_EMBEDDED_IBASE (celTCPGameFactory);
    virtual bool Initialize (iObjectRegistry* p)
    { return scfParent->Initialize (p); }
  } scfiComponent;

  CS_DECLARE_EVENT_SHORTCUTS;

  // Not an embedded interface to avoid circular references!!!
  class EventHandler : public iEventHandler
  {
  private:
    celTCPGameFactory* parent;

  public:
    EventHandler (celTCPGameFactory* parent)
    {
      SCF_CONSTRUCT_IBASE (0);
      EventHandler::parent = parent;
    }
    virtual ~EventHandler ()
    {
      SCF_DESTRUCT_IBASE ();
    }

    SCF_DECLARE_IBASE;
    virtual bool HandleEvent (iEvent& ev)
    {
      return parent->HandleEvent (ev);
    }
    CS_EVENTHANDLER_NAMES("cel.network.tcp");
    CS_EVENTHANDLER_NIL_CONSTRAINTS;
  } *scfiEventHandler;
};

}
CS_PLUGIN_NAMESPACE_END(celTCPNetwork)

#endif // __CEL_GAMEFACT_TCP__
