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

#ifndef __CEL_GAME_TCP__
#define __CEL_GAME_TCP__

#include "csutil/csobject.h"

#include "physicallayer/nettypes.h"
#include "physicallayer/network.h"
#include "plugins/network/tcp/tcpgameclient.h"
#include "plugins/network/tcp/tcpgameserver.h"

class celTCPGame : public csObject
{
  friend class celTCPGameFactory;
  friend class celTCPGameClient;
  friend class celTCPGameServer;

 protected:
  celNetworkGameType game_type;
  celGameInfo game_info;
  celTCPGameServer* server;
  celTCPGameClient* client;

 public:
  SCF_DECLARE_IBASE_EXT (csObject);

  celTCPGame (celNetworkGameType game_type, celGameInfo* game_info);
  virtual ~celTCPGame ();

  virtual celNetworkGameType GetGameType () const
    { return game_type; }
  virtual celGameInfo* GetGameInfo ()
    { return &game_info; }
  virtual bool IsServerAvailable () const
    { return server != 0; }
  virtual iCelGameServer* GetGameServer () const
    { return server ? &server->scfiCelGameServer : 0; }
  virtual bool IsClientAvailable () const
    { return client != 0; }
  virtual iCelGameClient* GetGameClient () const
    { return client ? &client->scfiCelGameClient : 0; }

  struct TCPGame : public iCelGame
  {
    SCF_DECLARE_EMBEDDED_IBASE (celTCPGame);
    virtual celNetworkGameType GetGameType () const
    { return scfParent->GetGameType (); }
    virtual celGameInfo* GetGameInfo ()
    { return scfParent->GetGameInfo (); }
    virtual bool IsServerAvailable () const
    { return scfParent->IsServerAvailable (); }
    virtual iCelGameServer* GetGameServer () const
    { return scfParent->GetGameServer (); }
    virtual bool IsClientAvailable () const
    { return scfParent->IsClientAvailable (); }
    virtual iCelGameClient* GetGameClient () const
    { return scfParent->GetGameClient (); }

  } scfiCelGame;
  friend struct TCPGame;
};

#endif // __CEL_GAME_TCP__
