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

#ifndef __CEL_TCPUTILS__
#define __CEL_TCPUTILS__

#include "iutil/objreg.h"
#include "csutil/list.h"
#include "csutil/bitarray.h"
#include "ivaria/reporter.h"

#include "nl.h"

#include "physicallayer/datatype.h"
#include "physicallayer/persist.h"
#include "physicallayer/entity.h"

CS_PLUGIN_NAMESPACE_BEGIN(celTCPNetwork)
{

// TODO: to be fixed
#define MAX_PACKET_LENGTH 2048

// messages from client to server
enum
{
  CLIENT_DATA_BANDWIDTH = 1,
  CLIENT_DATA_EVENT,
  CLIENT_DATA_CONTROLLED_LINK
};

// messages from server to client
enum
{
  SERVER_DATA_EVENT = 1,
  SERVER_DATA_LINK_INIT,
  SERVER_DATA_LINK_UPDATE,
  SERVER_DATA_LINK_CONTROL,
  SERVER_DATA_LINK_REMOVE,
  SERVER_DATA_KICKED
};

bool IsAddressEmpty (uint8* address);
void CopyAddress (uint8* source, NLaddress &destination);
void CopyAddress (NLaddress& source, uint8* destination);
//bool CompareAddress (uint8* addr1, uint8* addr2);
bool ReadAddressData (NLaddress& address, csString& hostname, uint16& port_nb);

bool ReportError (iObjectRegistry* object_reg, const char* msg, ...);
bool ReportNetworkError (iObjectRegistry* object_reg, const char* msg, ...);

class BufferReporter
{
 public:
  static void SetRegistry (iObjectRegistry* object_reg);
  static bool ReportWrite (csString txt = "");
  static bool ReportRead (csString txt = "");

};

csPtr<iCelPersistentDataList> GetEntityPersistentData (iCelEntity* entity, 
        csBitArray* needed_properties, celPersistenceType persistence_type);

celPersistenceResult SetEntityPersistentData (iCelEntity* entity, 
        csTicks data_time, iCelPersistentDataList* data_list, 
        celPersistenceType persistence_type);

/// a buffer reading and writing endian swaped data
class celNetworkBuffer
{
 private:
  NLint position;

 public:
  NLbyte buffer[MAX_PACKET_LENGTH];
  size_t data_size;

  celNetworkBuffer ();
  ~celNetworkBuffer ();

  bool Read (bool& b);
  bool Read (int8& b);
  bool Read (uint8& ub);
  bool Read (int16& w);
  bool Read (uint16& uw);
  bool Read (int32& l);
  bool Read (uint32& ul);
  bool Read (float& f);
  bool Read (csString& str);
  bool Read (char*& str);
  bool Read (csRef<iCelDataBuffer>& db);
  bool Read (celData* cd);
  bool Read (iCelPersistentDataList* data_list);

  bool Write (bool v);
  bool Write (int8 v);
  bool Write (uint8 v);
  bool Write (int16 v);
  bool Write (uint16 v);
  bool Write (int32 v);
  bool Write (uint32 v);
  bool Write (float f);
  bool Write (const char* str);
  bool Write (iCelDataBuffer* db);
  bool Write (celData* data);
  bool Write (iCelPersistentDataList* data_list);

  uint GetSize ();
  bool IsReadDataRemaining ();
  void Clear ();
};

/// a socket cache that read and write entire packets of data
class celTCPCachedSocket
{
 public:
  NLsocket socket;

 private:
  csArray<celNetworkBuffer*> packets_received;
  csArray<celNetworkBuffer*> packets_to_be_sent;
  celNetworkBuffer* current_packet_received;
  celNetworkBuffer* current_packet_sent;
  size_t current_receive_pos;
  size_t current_send_pos;
  size_t current_size_to_receive;

 public:
  // the socket must be already connected
  celTCPCachedSocket (NLsocket &socket);
  ~celTCPCachedSocket ();

  // read and write more data on the socket. Return 1 if connection is still 
  // available, 0 if it has been shut down, -1 if it has been broken
  int UpdateSocket ();

  // Return 1 if connection is still available, 0 if it has been shut down, -1 if it has been broken
  // don't return before all the packets have been read
  int FlushReadSocket ();

  // Return 1 if connection is still available, 0 if it has been shut down, -1 if it has been broken
  // don't return before all the packets have been written
  int FlushWriteSocket ();

  bool IsReceivedPacket ();
  bool IsRemainingSentPacket ();

  celNetworkBuffer* GetReceivedPacket ();

  void SendPacket (celNetworkBuffer* packet);

  // to reuse buffers
  //static celNetworkBuffer* GetNewBufferContext ();
};

}
CS_PLUGIN_NAMESPACE_END(celTCPNetwork)

#endif //__CEL_TCPUTILS__
