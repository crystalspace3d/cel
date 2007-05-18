/*
    Crystal Space Entity Layer
    Copyright (C) 2001 by Jorrit Tyberghein
  
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
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
    MA 02111-1307, USA.
*/

#ifndef __CEL_PL_PERSIST__
#define __CEL_PL_PERSIST__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/util.h"
#include "csutil/array.h"

#include "physicallayer/datatype.h"

struct iCelPropertyClass;
struct iCelEntity;
struct iFile;
struct celData;

/**
 * The different types of persistence. The different values can be:
 * <UL>
 * <LI>CEL_PERSIST_TYPE_UNDEF: Undefined, should never be encountered</LI>
 * <LI>CEL_PERSIST_TYPE_RECORD: The state of the entity is saved and 
 * loaded from a record.</LI>
 * <LI>CEL_PERSIST_TYPE_RECORD_FIRST_PASS: The state of the entity is 
 * saved and loaded from a record for the first pass</LI>
 * <LI>CEL_PERSIST_TYPE_SERVER_CONTROL: This is a network link controlled 
 * on the server side. A property class will receive calls to 
 * GetPersistentData if we are on the server side, and calls to 
 * SetPersistentData if we are on the client side.</LI>
 * <LI>CEL_PERSIST_TYPE_CLIENT_CONTROL: This is a network link controlled 
 * on the client side. A property class will receive calls to 
 * GetPersistentData if we are on the client side, and calls to 
 * SetPersistentData if we are on the server side.</LI>
 * <LI>CEL_PERSIST_TYPE_SERVER_FORCING: This is a network link controlled 
 * on the client side but the data are forced authoritatively by the server. 
 * A property class will receive calls to GetPersistentData if we are on 
 * the server side, and calls to SetPersistentData if we are on the client 
 * side.</LI>
 * </UL>
 */
enum celPersistenceType
{
  CEL_PERSIST_TYPE_UNDEF = 0,
  CEL_PERSIST_TYPE_RECORD,
  CEL_PERSIST_TYPE_RECORD_FIRST_PASS,
  CEL_PERSIST_TYPE_SERVER_CONTROL,
  CEL_PERSIST_TYPE_CLIENT_CONTROL,
  CEL_PERSIST_TYPE_SERVER_FORCING
};

/**
 * The result while setting the persistence data of a property class. The 
 * different values can be:
 * <UL>
 * <LI>CEL_PERSIST_RESULT_UNDEF: Undefined, should never be encountered.</LI>
 * <LI>CEL_PERSIST_RESULT_OK: No problems and no cheat detected.</LI>
 * <LI>CEL_PERSIST_RESULT_CHEAT_SUSPICIOUS: Suspicious, the player may be 
 * cheating.</LI>
 * <LI>CEL_PERSIST_RESULT_CHEAT_CLEAR: The player is clearly cheating.</LI>
 * <LI>CEL_PERSIST_RESULT_ERROR: An error was encountered while setting 
 * the persistent data.</LI>
 * <LI>CEL_PERSIST_RESULT_UNKNOWN_PC: Trying to set the persistence data 
 * of an unknown property class.</LI>
 * </UL>
 */
enum celPersistenceResult
{
  CEL_PERSIST_RESULT_UNDEF = 0,
  CEL_PERSIST_RESULT_OK,
  CEL_PERSIST_RESULT_CHEAT_SUSPICIOUS,
  CEL_PERSIST_RESULT_CHEAT_CLEAR,
  CEL_PERSIST_RESULT_ERROR,
  CEL_PERSIST_RESULT_UNKNOWN_PC
};

SCF_VERSION (iCelDataBuffer, 0, 0, 2);

/**
 * This interface describes persistable data.
 */
struct iCelDataBuffer : public iBase
{
  /**
   * Get a serial number for this data. This can be used
   * to check validity of the data (i.e. to compare version
   * numbers).
   */
  virtual long GetSerialNumber () const = 0;

  /**
   * Get the number of data entries.
   */
  virtual size_t GetDataCount () const = 0;

  /**
   * Reset the internal read counter.
   */
  virtual void Reset () = 0;

  /**
   * Get the next data entry. See Reset().
   */
  virtual celData* GetData () = 0;

  /**
   * Get a specific data entry.
   */
  virtual celData* GetData (size_t idx) = 0;

  /**
   * Add a data entry.
   */
  virtual celData* AddData () = 0;

  /// Convenience methods to add specific types of data.
  virtual void Add (bool v) { AddData ()->Set (v); }
  virtual void Add (int8 v) { AddData ()->Set (v); }
  virtual void Add (int16 v) { AddData ()->Set (v); }
  virtual void Add (int32 v) { AddData ()->Set (v); }
  virtual void Add (uint8 v) { AddData ()->Set (v); }
  virtual void Add (uint16 v) { AddData ()->Set (v); }
  virtual void Add (uint32 v) { AddData ()->Set (v); }
  virtual void Add (float v) { AddData ()->Set (v); }
  virtual void Add (const csVector2& v) { AddData ()->Set (v); }
  virtual void Add (const csVector3& v) { AddData ()->Set (v); }
  virtual void Add (const csColor& v) { AddData ()->Set (v); }
  virtual void Add (const char* v) { AddData ()->Set (v); }
  virtual void Add (iCelPropertyClass* v) { AddData ()->Set (v); }
  virtual void Add (iCelEntity* v) { AddData ()->Set (v); }
  virtual void AddAction (const char* v) { AddData ()->SetAction (v); }
  virtual void AddIBase (iBase* v) { AddData ()->SetIBase (v); }

  /// Convenience methods to get specific types of data.
  virtual bool GetBool ()
  {
    celData* cd = GetData ();
    return (cd && cd->type == CEL_DATA_BOOL) ? cd->value.bo : false;
  }
  virtual int8 GetInt8 ()
  {
    celData* cd = GetData ();
    return (cd && cd->type == CEL_DATA_BYTE) ? cd->value.b : 0;
  }
  virtual uint8 GetUInt8 ()
  {
    celData* cd = GetData ();
    return (cd && cd->type == CEL_DATA_UBYTE) ? cd->value.ub : 0;
  }
  virtual int16 GetInt16 ()
  {
    celData* cd = GetData ();
    return (cd && cd->type == CEL_DATA_WORD) ? cd->value.w : 0;
  }
  virtual uint16 GetUInt16 ()
  {
    celData* cd = GetData ();
    return (cd && cd->type == CEL_DATA_UWORD) ? cd->value.uw : 0;
  }
  virtual int32 GetInt32 ()
  {
    celData* cd = GetData ();
    return (cd && cd->type == CEL_DATA_LONG) ? cd->value.l : 0;
  }
  virtual uint32 GetUInt32 ()
  {
    celData* cd = GetData ();
    return (cd && cd->type == CEL_DATA_ULONG) ? cd->value.ul : 0;
  }
  virtual float GetFloat ()
  {
    celData* cd = GetData ();
    return (cd && cd->type == CEL_DATA_FLOAT) ? cd->value.f : 0.0f;
  }
  virtual iString* GetString ()
  {
    celData* cd = GetData ();
    return (cd && cd->type == CEL_DATA_STRING) ? cd->value.s : (iString*)0;
  }
  virtual void GetVector2 (csVector2& v)
  {
    celData* cd = GetData ();
    if (cd && cd->type == CEL_DATA_VECTOR2)
    { v.x = cd->value.v.x; v.y = cd->value.v.y; }
    else v.x = v.y = 0;
  }
  virtual void GetVector3 (csVector3& v)
  {
    celData* cd = GetData ();
    if (cd && cd->type == CEL_DATA_VECTOR3)
    { v.x = cd->value.v.x; v.y = cd->value.v.y; v.z = cd->value.v.z; }
    else v.x = v.y = v.z = 0;
  }
  virtual void GetColor (csColor& v)
  {
    celData* cd = GetData ();
    if (cd && cd->type == CEL_DATA_COLOR)
    {
      v.red = cd->value.col.red;
      v.green = cd->value.col.green;
      v.blue = cd->value.col.blue;
    }
    else v.red = v.green= v.blue = 0;
  }
  virtual iCelPropertyClass* GetPC ()
  {
    celData* cd = GetData ();
    return (cd && cd->type == CEL_DATA_PCLASS)
    	? cd->value.pc
	: (iCelPropertyClass*)0;
  }
  virtual iCelEntity* GetEntity ()
  {
    celData* cd = GetData ();
    return (cd && cd->type == CEL_DATA_ENTITY)
    	? cd->value.ent
	: (iCelEntity*)0;
  }
  virtual iBase* GetIBase ()
  {
    celData* cd = GetData ();
    return (cd && cd->type == CEL_DATA_IBASE) ? cd->value.ibase : (iBase*)0;
  }
  virtual csString GetDebugInfo ()
  {
    if (GetDataCount () == 0) return "Data buffer: empty\n";

    csString txt = "Data buffer:\n";
    size_t i = 0, count = GetDataCount ();
    for ( ; i < count; i++)
      txt.AppendFmt("\t%s\n", GetData (i)->GetDebugInfo ().GetData ());

    return txt;
  }
};

SCF_VERSION (iCelPersistentDataList, 0, 0, 1);

/**
 * A list of persistent data for all the property classes of an entity.
 * The complete state of all the property classes of an entity can be specified
 * with this list. The list can contain one entry per property class. The name
 * and the tag of the property class is used to identify it.
 */
struct iCelPersistentDataList : public iBase
{
  /**
   * Return the number of entries in this list.
   */
  virtual size_t GetCount () const = 0;

  /**
   * To get the persistent data at the specified position.
   * \param idx The index of the entry.
   * \param databuf The persistent data.
   * \param pc_name The name of the property class associated to the 
   * persistent data.
   * \param pc_tag The tag of the property class associated to the 
   * persistent data.
   */
  virtual bool GetPersistentData (size_t idx, csRef<iCelDataBuffer>& databuf, 
        csString& pc_name, csString& pc_tag) const = 0;

  /**
   * To add a new persistent data buffer.
   * \param idx The index of the entry.
   * \param databuf The persistent data.
   * \param pc_name The name of the property class associated to the 
   * persistent data.
   * \param pc_tag The tag of the property class associated to the 
   * persistent data.
   */
  virtual void AddPersistentData (csRef<iCelDataBuffer>& databuf, 
        csString& pc_name, csString& pc_tag) = 0;
};

SCF_VERSION (iCelLocalEntitySet, 0, 0, 1);

/**
 * A local entity set define the set of entities that are considered
 * 'local' to the saved file. That means that all entities that are in
 * this list will be saved and all entities that are not in this list
 * (but are still referred too from entities in this set) will be
 * saved as references.
 */
struct iCelLocalEntitySet : public iBase
{
  /// Number of entities managed by this set.
  virtual size_t GetEntityCount () const = 0;
  
  /// Get the specified entity.
  virtual iCelEntity* GetEntity (size_t idx) const = 0;

  /// Add an entity to the local set.
  virtual void AddEntity (iCelEntity* entity) = 0;

  /// Test if an entity is local (is in this set).
  virtual bool IsLocal (iCelEntity* entity) = 0;
  /// Test if a property class is local (is in this set).
  virtual bool IsLocal (iCelPropertyClass* pc) = 0;

  /**
   * This routine will be called by the persistence layer whenever
   * it needs to save a non-local entity. So this routine should
   * return a databuffer that can identify a non-local entity.
   */
  virtual csPtr<iCelDataBuffer> SaveExternalEntity (iCelEntity* entity) = 0;

  /**
   * This routine will be called by the persistence layer whenever
   * it needs to find a non-local entity. Basically it should return
   * an already existing entity from the databuffer.
   */
  virtual iCelEntity* FindExternalEntity (iCelDataBuffer* databuf) = 0;

  /**
   * This routine will be called by the persistence layer whenever
   * it needs to save a non-local property class. So this routine should
   * return a databuffer that can identify a non-local property class.
   */
  virtual csPtr<iCelDataBuffer> SaveExternalPC (iCelPropertyClass* pc) = 0;

  /**
   * This routine will be called by the persistence layer whenever
   * it needs to find a non-local property class. Basically it should return
   * an already existing property class from the databuffer.
   */
  virtual iCelPropertyClass* FindExternalPC (iCelDataBuffer* databuf) = 0;
};

SCF_VERSION (iCelPersistence, 0, 1, 1);

/**
 * This interface describes a module that can
 * persist CEL data.
 */
struct iCelPersistence : public iBase
{
  /**
   * Load a local entity set. 'name' is a name relevant for the
   * given type of persistence. It can be a filename for example
   * (VFS).
   * \param set is an empty local entity set that will be filled
   * by this routine.
   * \param name identifies the saved data.
   * \return false on error. Reporting will be already done.
   */
  virtual bool Load (iCelLocalEntitySet* set, const char* name) = 0;

  /**
   * Save a local entity set. 'name' is a name relevant for the
   * given type of persistence. It can be a filename for example
   * (VFS).
   * \param set is a local entity set with entities to be saved.
   * \param name identifies the saved data.
   * \return false on error. Reporting will be already done.
   */
  virtual bool Save (iCelLocalEntitySet* set, const char* name) = 0;

  /**
   * Load a local entity set from an iFile.
   * \param set is an empty local entity set that will be filled
   * by this routine.
   * \param file the iFile to load the entity set from.
   * \return false on error. Reporting will be already done.
   */
  virtual bool Load (iCelLocalEntitySet* set, iFile* file) = 0;

  /**
   * Save a local entity set to an iFile.
   * \param set is a local entity set with entities to be saved.
   * \param file the iFile to save the entity set to.
   * \return false on error. Reporting will be already done.
   */
  virtual bool Save (iCelLocalEntitySet* set, iFile* file) = 0;
};

#endif // __CEL_PL_PERSIST__
