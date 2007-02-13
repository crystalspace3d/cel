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
#include "csutil/weakref.h"

#include "physicallayer/pl.h"
#include "celtool/persisthelper.h"
#include "plugins/network/tcp/tcputils.h"

CS_PLUGIN_NAMESPACE_BEGIN(celTCPNetwork)
{

bool IsAddressEmpty (uint8* address)
{
  int i = 0;
  for ( ; i < 32; i++)
    if (address[i] != 0)
      return false;
  return true;
}

void CopyAddress (uint8* source, NLaddress &destination)
{
  int i = 0;
  for ( ; i < 32; i++)
    destination.addr[i] = source[i];
}

void CopyAddress (NLaddress &source, uint8* destination)
{
  int i = 0;
  for ( ; i < 32; i++)
    destination[i] = source.addr[i];
}
/*
bool CompareAddress (uint8* addr1, uint8* addr2)
{
  int i = 0;
  for ( ; i < 32; i++)
    if (addr1[i] != addr2[i])
      return false;
  return true;
}
*/

bool ReadAddressData (NLaddress& address, csString& hostname, uint16& port_nb)
{
  char temp_address[NL_MAX_STRING_LENGTH];
  if (!nlAddrToString (&address, temp_address))
  {
    hostname = "";
    port_nb = 0;
    return false;
  }
  else
  {
    hostname = temp_address;

    // remove ":port_nb" from hostname
    size_t pos = hostname.Find (":");
    if (pos != (size_t) -1)
      hostname = hostname.Slice (0, pos);
  }

  port_nb = nlGetPortFromAddr (&address);
  return true;
}

//---------------------------------------------------------------------------

bool ReportError (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	    "cel.network.tcp", msg, arg);

  va_end (arg);
  return false;
}

bool ReportNetworkError (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csString report;
  if (strlen (msg) != 0)
  {
    report += msg;
    report += "\n";
  }

  NLenum err = nlGetError ();
  if (err == NL_SYSTEM_ERROR) {
    csString error_str (nlGetSystemErrorStr (nlGetSystemError ()));
    report += "System error: " + error_str;
  }
  else {
    csString error_str (nlGetErrorStr (err));
    report += "HawkNL error: " + error_str;
  }

  csReportV (object_reg, CS_REPORTER_SEVERITY_ERROR,
	    "cel.network.tcp", report.GetData (), arg);

  va_end (arg);
  return false;
}

//---------------------------------------------------------------------------

static iObjectRegistry* buffer_object_reg;

void BufferReporter::SetRegistry (iObjectRegistry* object_reg)
{
   buffer_object_reg = object_reg;
}

bool BufferReporter::ReportWrite (csString txt)
{
  csString report = "Error while writing data to celNetworkBuffer";
  if (!txt.IsEmpty ()) report.AppendFmt (": %s", txt.GetData ());
  csReport (buffer_object_reg, CS_REPORTER_SEVERITY_ERROR,
            "cel.network.tcp", report);
  return false;
}

bool BufferReporter::ReportRead (csString txt)
{
  csString report = "Error while reading data from celNetworkBuffer";
  if (!txt.IsEmpty ()) report.AppendFmt (": %s", txt.GetData ());
  csReport (buffer_object_reg, CS_REPORTER_SEVERITY_ERROR,
            "cel.network.tcp", report);
  return false;
}

//---------------------------------------------------------------------------

csPtr<iCelPersistentDataList> GetEntityPersistentData (iCelEntity* entity, 
        csBitArray* needed_properties, celPersistenceType persistence_type)
{
  iCelPersistentDataList* data_list = new celStandardPersistentDataList  ();

  size_t i, count = entity->GetPropertyClassList ()->GetCount ();
  for (i = 0; i < count; i++)
  {
    iCelPropertyClass* pc = entity->GetPropertyClassList ()->Get (i);
    csRef<iCelDataBuffer> databuf = pc->GetPersistentData (persistence_type);
    if (databuf && databuf->GetDataCount () > 0)
    {
      csString pc_name = pc->GetName ();
      csString pc_tag = pc->GetTag ();
      data_list->AddPersistentData (databuf, pc_name, pc_tag);
    }
  }

  return csPtr<iCelPersistentDataList> (data_list);
}

celPersistenceResult SetEntityPersistentData (iCelEntity* entity, 
        csTicks data_time, iCelPersistentDataList* data_list, 
        celPersistenceType persistence_type)
{
  celPersistenceResult persist_code = CEL_PERSIST_RESULT_OK;

  csRef<iCelDataBuffer> databuf;
  csString pc_name, pc_tag;
  iCelPropertyClass* pc;
  size_t i, count = data_list->GetCount ();
  for (i = 0; i < count; i++)
  {
    data_list->GetPersistentData (i, databuf, pc_name, pc_tag);
    if (pc_tag)
      pc = entity->GetPropertyClassList ()->FindByNameAndTag (pc_name, pc_tag);
    else
      pc = entity->GetPropertyClassList ()->FindByName (pc_name);

    if (pc)
    {
      celPersistenceResult pc_persist_code = 
	pc->SetPersistentData (data_time, databuf, persistence_type);
      if (pc_persist_code > persist_code)
	persist_code = pc_persist_code;
    }
  }

  return persist_code;
}


//---------------------------------------------------------------------------

static csWeakRef<iCelPlLayer> pl;

celNetworkBuffer::celNetworkBuffer ()
{
  // TODO: can't we create iCelDataBuffers more easily without using the physical layer?
  if (!pl)
    pl = csQueryRegistry<iCelPlLayer> (buffer_object_reg);
  data_size = position = 0;
}

celNetworkBuffer::~celNetworkBuffer ()
{
}

bool celNetworkBuffer::Read (bool& b)
{
  if (((size_t) position) + 1 > data_size)
    return BufferReporter::ReportRead ("bool");
  NLbyte rb;
  readByte (buffer, position, rb);
  b = rb;
  return true;
}

bool celNetworkBuffer::Read (int8& b)
{
  if (((size_t) position) + 1 > data_size)
    return BufferReporter::ReportRead ("int8");
  readByte (buffer, position, b);
  return true;
}

bool celNetworkBuffer::Read (uint8& ub)
{
  if (((size_t) position) + 1 > data_size)
    return BufferReporter::ReportRead ("uint8");
  readByte (buffer, position, ub);
  return true;
}

bool celNetworkBuffer::Read (int16& w)
{
  if (((size_t) position) + 2 > data_size)
    return BufferReporter::ReportRead ("int16");
  readShort (buffer, position, w);
  return true;
}

bool celNetworkBuffer::Read (uint16& uw)
{
  if (((size_t) position) + 2 > data_size)
    return BufferReporter::ReportRead ("uint16");
  readShort (buffer, position, uw);
  return true;
}

bool celNetworkBuffer::Read (int32& l)
{
  if (((size_t) position) + 4 > data_size)
    return BufferReporter::ReportRead ("int32");
  readLong (buffer, position, l);
  return true;
}

bool celNetworkBuffer::Read (uint32& ul)
{
  if (((size_t) position) + 4 > data_size)
    return BufferReporter::ReportRead ("uint32");
  readLong (buffer, position, ul);
  return true;
}

bool celNetworkBuffer::Read (float& f)
{
  if (((size_t) position) + 4 > data_size)
    return BufferReporter::ReportRead ("float");
  readFloat (buffer, position, f);
  return true;
}

bool celNetworkBuffer::Read (csString& str)
{
  if (((size_t) position) + 2 > data_size)
    return BufferReporter::ReportRead ("csString");
  uint16 size;
  readShort (buffer, position, size);

  if (size)
  {
    if (((size_t) position) + size > data_size)
    {
      str.Empty ();
      return BufferReporter::ReportRead ();
    }

    char* newstr = new char[size + 1];
    readBlock (buffer, position, newstr, size);
    newstr[size] = 0;
    str = newstr;
    delete[] newstr;
  }
  else str.Empty ();

  return true;
}

bool celNetworkBuffer::Read (char*& str)
{
  if (((size_t) position) + 2 > data_size)
    return BufferReporter::ReportRead ("char*");
  uint16 size;
  readShort (buffer, position, size);

  if (size)
  {
    if (((size_t) position) + size > data_size)
    {
      str = 0;
      return BufferReporter::ReportRead ();
    }

    str = new char[size + 1];
    readBlock (buffer, position, str, size);
    str[size] = 0;
  }
  else str = 0;

  return true;
}

bool celNetworkBuffer::Read (csRef<iCelDataBuffer>& db)
{
  db = 0;
  if (((size_t) position) + 6 > data_size)
    return BufferReporter::ReportRead ("iCelDataBuffer");

  int32 serial_nb;
  readLong (buffer, position, serial_nb);
  db = pl->CreateDataBuffer (serial_nb);
  // TODO: use uint8 instead?
  uint16 data_count;
  readShort (buffer, position, data_count);

  int i = 0;
  for ( ; i < data_count; i++)
    if (!Read (db->AddData ()))
      return BufferReporter::ReportRead ("Error reading data entry");

  return true;
}

bool celNetworkBuffer::Read (celData* cd)
{
  if (!cd || ((size_t) position) + 1 > data_size)
    return BufferReporter::ReportRead ("celData");

  uint8 t;
  uint8 ub;
  int8 b;
  uint16 uw;
  int16 w;
  uint32 ul;
  int32 l;
  float f;
  csString s;

  if (!Read (t))
    return BufferReporter::ReportRead ("celData: type");
  switch (t)
  {
    case CEL_DATA_ACTION:
    case CEL_DATA_NONE:
      BufferReporter::ReportRead ("Bad type in persisted data!");
      CS_ASSERT (false);
    case CEL_DATA_BOOL:
      if (!Read (ub))
	return BufferReporter::ReportRead ("celData: bool");
      cd->Set ((bool)ub);
      break;
    case CEL_DATA_BYTE:
      if (!Read (b))
	return BufferReporter::ReportRead ("celData: int8");
      cd->Set (b);
      break;
    case CEL_DATA_WORD:
      if (!Read (w))
	return BufferReporter::ReportRead ("celData: int16");
      cd->Set (w);
      break;
    case CEL_DATA_LONG:
      if (!Read (l))
	return BufferReporter::ReportRead ("celData: int32");
      cd->Set (l);
      break;
    case CEL_DATA_UBYTE:
      if (!Read (ub))
	return BufferReporter::ReportRead ("celData: uint8");
      cd->Set (ub);
      break;
    case CEL_DATA_UWORD:
      if (!Read (uw))
	return BufferReporter::ReportRead ("celData: uint16");
      cd->Set (uw);
      break;
    case CEL_DATA_ULONG:
      if (!Read (ul))
	return BufferReporter::ReportRead ("celData: uint32");
      cd->Set (ul);
      break;
    case CEL_DATA_FLOAT:
      if (!Read (f))
	return BufferReporter::ReportRead ("celData: float");
      cd->Set (f);
      break;
    case CEL_DATA_STRING:
      if (!Read (s))
	return BufferReporter::ReportRead ("celData: string");
      cd->Set (s);
      // TODO: it will crash if the string is big enough and the next line is uncommented -> same bug in cpersist.cpp
      //delete[] s;
      break;
    case CEL_DATA_COLOR:
      {
        csColor v;
        if (!Read (v.red))
	  return BufferReporter::ReportRead ("celData: color.red");
        if (!Read (v.green))
	  return BufferReporter::ReportRead ("celData: color.green");
        if (!Read (v.blue))
	  return BufferReporter::ReportRead ("celData: color.blue");
        cd->Set (v);
      }
      break;
    case CEL_DATA_VECTOR2:
      {
        csVector2 v;
        if (!Read (v.x))
	  return BufferReporter::ReportRead ("celData: vector2d.x");
        if (!Read (v.y))
	  return BufferReporter::ReportRead ("celData: vector2d.y");
        cd->Set (v);
      }
      break;
    case CEL_DATA_VECTOR3:
      {
        csVector3 v;
        if (!Read (v.x))
	  return BufferReporter::ReportRead ("celData: vector3d.x");
        if (!Read (v.y))
	  return BufferReporter::ReportRead ("celData: vector3d.y");
        if (!Read (v.z))
	  return BufferReporter::ReportRead ("celData: vector3d.z");
        cd->Set (v);
      }
      break;
    case CEL_DATA_PCLASS:
      return BufferReporter::ReportRead ("celData: Could not read property classes");
      CS_ASSERT (false);
      break;
    case CEL_DATA_ENTITY:
      return BufferReporter::ReportRead ("celData: Could not read entities");
      CS_ASSERT (false);
      break;
    case CEL_DATA_IBASE:
      return BufferReporter::ReportRead ("celData: Could not read iBase");
      CS_ASSERT (false);
      break;
    default:
      return BufferReporter::ReportRead ("celData: Found unknown Data type!");
      CS_ASSERT (false);
  }

  return true;
}

bool celNetworkBuffer::Read (iCelPersistentDataList* data_list)
{
  if (!data_list || ((size_t) position) + 1 > data_size)
    return BufferReporter::ReportRead ("iCelPersistentDataList");
  uint8 count;
  readByte (buffer, position, count);
  
  csRef<iCelDataBuffer> databuf;
  csString pc_name, pc_tag;
  uint8 i;
  for (i = 0; i < count; i++)
  {
    if (!Read (pc_name))
      return BufferReporter::ReportRead ("iCelPersistentDataList: pc_name");
    if (!Read (pc_tag))
      return BufferReporter::ReportRead ("iCelPersistentDataList: pc_tag");
    if (!Read (databuf))
      return BufferReporter::ReportRead ("iCelPersistentDataList: databuf");
    data_list->AddPersistentData (databuf, pc_name, pc_tag);
  }

  return true;
}

bool celNetworkBuffer::Write (bool v)
{
  if (data_size + 1 > MAX_PACKET_LENGTH)
    return BufferReporter::ReportWrite ("bool");
  NLbyte wv = v;
  writeByte (buffer, position, wv);
  data_size += 1;
  return true;
}

bool celNetworkBuffer::Write (int8 v)
{
  if (data_size + 1 > MAX_PACKET_LENGTH)
    return BufferReporter::ReportWrite ("int8");
  writeByte (buffer, position, v);
  data_size += 1;
  return true;
}

bool celNetworkBuffer::Write (uint8 v)
{
  if (data_size + 1 > MAX_PACKET_LENGTH)
    return BufferReporter::ReportWrite ("uint8");
  writeByte (buffer, position, v);
  data_size += 1;
  return true;
}

bool celNetworkBuffer::Write (int16 v)
{
  if (data_size + 2 > MAX_PACKET_LENGTH)
    return BufferReporter::ReportWrite ("int16");
  writeShort (buffer, position, v);
  data_size += 2;
  return true;
}

bool celNetworkBuffer::Write (uint16 v)
{
  if (data_size + 2 > MAX_PACKET_LENGTH)
    return BufferReporter::ReportWrite ("uint16");
  writeShort (buffer, position, v);
  data_size += 2;
  return true;
}

bool celNetworkBuffer::Write (int32 v)
{
  if (data_size + 4 > MAX_PACKET_LENGTH)
    return BufferReporter::ReportWrite ("int32");
  writeLong (buffer, position, v);
  data_size += 4;
  return true;
}

bool celNetworkBuffer::Write (uint32 v)
{
  if (data_size + 4 > MAX_PACKET_LENGTH)
    return BufferReporter::ReportWrite ("uint32");
  writeLong (buffer, position, v);
  data_size += 4;
  return true;
}

bool celNetworkBuffer::Write (float f)
{
  if (data_size + 4 > MAX_PACKET_LENGTH)
    return BufferReporter::ReportWrite ("float");
  writeFloat (buffer, position, f);
  data_size += 4;
  return true;
}

bool celNetworkBuffer::Write (const char* str)
{
  if (!str)
  {
    if (position + 2 >= MAX_PACKET_LENGTH)
      return BufferReporter::ReportWrite ("char*");
    uint16 size = 0;
    writeShort (buffer, position, size);
    data_size += 2;
    return true;
  }

  if (position + strlen (str) + 2 >= MAX_PACKET_LENGTH)
    return BufferReporter::ReportWrite ("char* > 0");
  uint16 size = strlen (str);
  writeShort (buffer, position, size);
  writeBlock (buffer, position, str, size);
  data_size += size + 2;
  return true;
}

bool celNetworkBuffer::Write (iCelDataBuffer* db)
{
  if (data_size + 6 > MAX_PACKET_LENGTH)
    return BufferReporter::ReportWrite ("iCelDataBuffer");
  if (!db)
  {
    int32 serial_nb;
    writeLong (buffer, position, serial_nb);
    uint16 size = 0;
    writeShort (buffer, position, size);
    data_size += 6;
    return true;
  }
  int32 serial_nb = db->GetSerialNumber ();
  writeLong (buffer, position, serial_nb);
  uint16 size = db->GetDataCount ();
  writeShort (buffer, position, size);
  data_size += 6;

  size_t i = 0;
  for ( ; i < db->GetDataCount () ; i++)
  {
    if (!Write (db->GetData (i)))
      return BufferReporter::ReportWrite ("iCelDataBuffer: celData");
  }
  return true;
}

bool celNetworkBuffer::Write (celData* data)
{
  if (!data || data_size + 1 > MAX_PACKET_LENGTH)
    return BufferReporter::ReportWrite ("celData");
  uint8 type = data->type;
  writeByte (buffer, position, type);
  data_size += 1;

  switch (data->type)
  {
    case CEL_DATA_NONE:
    case CEL_DATA_ACTION:
      CS_ASSERT (false);
      break;
    case CEL_DATA_BOOL:
      if (data_size + 1 > MAX_PACKET_LENGTH)
	return BufferReporter::ReportWrite ("celData: bool");
      writeByte (buffer, position, (uint8) data->value.bo);
      data_size += 1;
      break;
    case CEL_DATA_BYTE:
      if (data_size + 1 > MAX_PACKET_LENGTH)
	return BufferReporter::ReportWrite ("celData: int8");
      writeByte (buffer, position, data->value.b);
      data_size += 1;
      break;
    case CEL_DATA_WORD:
      if (data_size + 2 > MAX_PACKET_LENGTH)
	return BufferReporter::ReportWrite ("celData: int16");
      writeShort (buffer, position, data->value.w);
      data_size += 2;
      break;
    case CEL_DATA_LONG:
      if (data_size + 4 > MAX_PACKET_LENGTH)
	return BufferReporter::ReportWrite ("celData: int32");
      writeLong (buffer, position, data->value.l);
      data_size += 4;
      break;
    case CEL_DATA_UBYTE:
      if (data_size + 1 > MAX_PACKET_LENGTH)
	return BufferReporter::ReportWrite ("celData: uint8");
      writeByte (buffer, position, data->value.ub);
      data_size += 1;
      break;
    case CEL_DATA_UWORD:
      if (data_size + 2 > MAX_PACKET_LENGTH)
	return BufferReporter::ReportWrite ("celData: uint16");
      writeShort (buffer, position, data->value.uw);
      data_size += 2;
      break;
    case CEL_DATA_ULONG:
      if (data_size + 4 > MAX_PACKET_LENGTH)
	return BufferReporter::ReportWrite ("celData: uint32");
      writeLong (buffer, position, data->value.ul);
      data_size += 4;
      break;
    case CEL_DATA_FLOAT:
      if (data_size + 4 > MAX_PACKET_LENGTH)
	return BufferReporter::ReportWrite ("celData: float");
      writeFloat (buffer, position, data->value.f);
      data_size += 4;
      break;
    case CEL_DATA_VECTOR2:
      if (data_size + 8 > MAX_PACKET_LENGTH)
	return BufferReporter::ReportWrite ("celData: vector2d");
      writeFloat (buffer, position, data->value.v.x);
      writeFloat (buffer, position, data->value.v.y);
      data_size += 8;
      break;
    case CEL_DATA_VECTOR3:
      if (data_size + 12 > MAX_PACKET_LENGTH)
	return BufferReporter::ReportWrite ("celData: vector3d");
      writeFloat (buffer, position, data->value.v.x);
      writeFloat (buffer, position, data->value.v.y);
      writeFloat (buffer, position, data->value.v.z);
      data_size += 12;
      break;
    case CEL_DATA_COLOR:
      if (data_size + 12 > MAX_PACKET_LENGTH)
	return BufferReporter::ReportWrite ("celData: color");
      writeFloat (buffer, position, data->value.col.red);
      writeFloat (buffer, position, data->value.col.green);
      writeFloat (buffer, position, data->value.col.blue);
      data_size += 12;
      break;
    case CEL_DATA_STRING:
      Write (*data->value.s);
      break;
    case CEL_DATA_PCLASS:
      return BufferReporter::ReportWrite ("celData: Could not send PC");
      CS_ASSERT (false);
      break;
    case CEL_DATA_ENTITY:
      return BufferReporter::ReportWrite ("celData: Could not send entities");
      CS_ASSERT (false);
      break;
    case CEL_DATA_IBASE:
      return BufferReporter::ReportWrite ("celData: Could not send iBase");
      CS_ASSERT (false);
      break;
    default:
      return BufferReporter::ReportWrite ("celData: Found an unknown data type");
      CS_ASSERT (false);
      break;
  }

  return true;
}

bool celNetworkBuffer::Write (iCelPersistentDataList* data_list)
{
  if (data_size + 1 > MAX_PACKET_LENGTH)
    return BufferReporter::ReportWrite ("iCelPersistentDataList");
  uint8 count = data_list->GetCount ();
  writeByte (buffer, position, count);
  data_size += 1;

  csRef<iCelDataBuffer> databuf;
  csString pc_name, pc_tag;
  uint8 i;
  for (i = 0; i < count; i++)
  {
    data_list->GetPersistentData (i, databuf, pc_name, pc_tag);
    if (!Write (pc_name))
      return BufferReporter::ReportWrite ("iCelPersistentDataList: pc_name");
    if (!Write (pc_tag))
      return BufferReporter::ReportWrite ("iCelPersistentDataList: pc_tag");
    if (!Write (databuf))
      return BufferReporter::ReportWrite ("iCelPersistentDataList: databuf");
  }

  return true;
}

uint celNetworkBuffer::GetSize () { return data_size; }

bool celNetworkBuffer::IsReadDataRemaining ()
{
  return ((size_t) position) < data_size;
}

void celNetworkBuffer::Clear ()
{
  data_size = position = 0;
}

//---------------------------------------------------------------------------

celTCPCachedSocket::celTCPCachedSocket (NLsocket &socket)
{
  celTCPCachedSocket::socket = socket;
  current_packet_received = new celNetworkBuffer ();
  current_packet_sent = 0;
  current_receive_pos = 0;
  current_send_pos = 0;
  current_size_to_receive = 0;
}

celTCPCachedSocket::~celTCPCachedSocket ()
{
  // close socket
  if (socket)
    nlClose (socket);

  // delete all packets
  size_t i;
  for (i = 0; i < packets_received.Length (); i++)
    delete packets_received[i];
  for (i = 0; i < packets_to_be_sent.Length (); i++)
    delete packets_to_be_sent[i];
  packets_received.DeleteAll ();
  packets_to_be_sent.DeleteAll ();
  delete current_packet_received;
  delete current_packet_sent;
}

int celTCPCachedSocket::UpdateSocket ()
{
  NLbyte temp_buf[16];
  // TODO: read and send more than one packet at each step

  // check if need a new send packet
  if (!current_packet_sent && !packets_to_be_sent.IsEmpty ())
  {
    current_packet_sent = packets_to_be_sent[0];
    packets_to_be_sent.DeleteIndex (0);
    current_send_pos = 0;

    // send the size of the packet
    NLint sent_size = 0;
    NLint count = 0;
    writeShort (temp_buf, count, current_packet_sent->data_size);
    while (sent_size == 0)
    {
      // TODO: bug if do not write the entire value in one step
      sent_size = nlWrite (socket, &temp_buf, sizeof(NLshort));
      if (sent_size == NL_INVALID)
      {
	if (nlGetError () == NL_MESSAGE_END) return 0;
	else return -1;
      }
    }
  }

  // send packet
  if (current_packet_sent)
  {
    NLint sent_size = nlWrite (socket, 
			       current_packet_sent->buffer + current_send_pos, 
			       current_packet_sent->data_size - current_send_pos);
    if (sent_size == NL_INVALID)
    {
      if (nlGetError () == NL_MESSAGE_END) return 0;
      else return -1;
    }
    current_send_pos += sent_size;

    if (current_send_pos >= current_packet_sent->data_size)
    {
      delete current_packet_sent;
      current_packet_sent = 0;
    }
  }

  // read the size of the packet to be received
  if (!current_size_to_receive)
  {
    // TODO: bug if do not read the entire value in one step
    NLint data_size = nlRead (socket, temp_buf, sizeof(NLshort));

    if (data_size == NL_INVALID)
    {
      if (nlGetError () == NL_MESSAGE_END) return 0;
      else return -1;
    }
    if (data_size > 0)
    {
      NLint count = 0;
      readShort (temp_buf, count, current_size_to_receive);
    }
  }

  // read packet
  if (current_size_to_receive)
  {
    NLint data_size = nlRead (socket, 
			      current_packet_received->buffer + current_receive_pos, 
			      current_size_to_receive - current_receive_pos);
    if (data_size == NL_INVALID)
    {
      if (nlGetError () == NL_MESSAGE_END) return 0;
      else return -1;
    }
    current_receive_pos += data_size;

    if (current_receive_pos >= current_size_to_receive)
    {
      current_packet_received->data_size = current_size_to_receive;
      packets_received.Push (current_packet_received);
      current_packet_received = new celNetworkBuffer ();
      current_receive_pos = 0;
      current_size_to_receive = 0;
    }
  }

  return 1;
}

int celTCPCachedSocket::FlushReadSocket ()
{
  // TODO
  return 0;
}

int celTCPCachedSocket::FlushWriteSocket ()
{
  // TODO
  return 0;
}

bool celTCPCachedSocket::IsReceivedPacket ()
{
  return !packets_received.IsEmpty ();
}

bool celTCPCachedSocket::IsRemainingSentPacket ()
{
  return !packets_to_be_sent.IsEmpty () || current_packet_sent != 0;;

}

celNetworkBuffer* celTCPCachedSocket::GetReceivedPacket ()
{
  if (packets_received.IsEmpty ()) return 0;
  celNetworkBuffer* packet = packets_received[0];
  packets_received.DeleteIndex (0);
  return packet;
}

void celTCPCachedSocket::SendPacket (celNetworkBuffer* packet)
{
  packets_to_be_sent.Push(packet);
}

}
CS_PLUGIN_NAMESPACE_END(celTCPNetwork)

