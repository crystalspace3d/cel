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

#include "physicallayer/pl.h"
#include "plugins/network/tcp/tcpgame.h"

CS_PLUGIN_NAMESPACE_BEGIN(celTCPNetwork)
{

SCF_IMPLEMENT_IBASE_EXT (celTCPGame)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iCelGame)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celTCPGame::TCPGame)
  SCF_IMPLEMENTS_INTERFACE (iCelGame)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celTCPGame::celTCPGame (celNetworkGameType game_type, celGameInfo* game_info)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiCelGame);
  server = 0;
  client = 0;
  celTCPGame::game_type = game_type;
  celTCPGame::game_info = *game_info;
}

celTCPGame::~celTCPGame ()
{
  delete client;
  delete server;
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiCelGame);
}

}
CS_PLUGIN_NAMESPACE_END(celTCPNetwork)

