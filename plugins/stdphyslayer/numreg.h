#ifndef PLUGINS_STDPHYSLAYER_NUMREG_H
#define PLUGINS_STDPHYSLAYER_NUMREG_H

#include "cstypes.h"
#include "csutil/hash.h"

#include "physicallayer/numreg.h"

/**
 * This class stores a list of Objects and assignes a unique ID to them
 *  Note: This class does nearly no error checking  and should be used with
 *  care
 */
class NumRegLists : public iNumReg
{
public:
  /** 
   *  Constructor, limit is the maximum size this array will grow,
   *   freelistsize is the size of the list holding last freed items (the
   *   bigger this list is the better your performance if you remove/add new
   *   elements often.
   *   startsize is the initial size of the array
   */
  NumRegLists(int limit=100000, int freelistsize=100, int startsize=300);
  /// The destructor frees all memory
  virtual ~NumRegLists();

  SCF_DECLARE_IBASE;

  /** 
   *  Registers an object in the registry and returns the new ID, in error
   * case ID 0 is returned
   */
  virtual CS_ID Register (void* obj);
  /**
   *  Registers an object in the registry with an ID. You shouldn't use this
   * function with this implementation. It should work though, but slow.
   */
  virtual void RegisterWithID (void* obj, CS_ID id);
  /** Removes an registered object from registry */
  virtual bool Remove (CS_ID id);
  /** Removes an registered object from registry (Note: this is slow you
   * should use Remove(id) if possible
   */
  virtual bool Remove (void* obj);
  
  /** Removes all objects from the list */
  virtual void Clear();

  /** Returns the object with ID id from the list */
  virtual void* Get(CS_ID id)
  { 
    if (id>=listsize)
      return 0;
    
    return list[id];
  }

  // Returns the size of the buffer (This is NOT the count of objects in the
  // registry)
  virtual unsigned int Length()
  {
    return listsize;
  }
  
protected:
  inline CS_ID intern_Register(void* obj);
  
  void** list;
  CS_ID* freelist;
  CS_ID listsize;
  CS_ID limit;
  unsigned int freelistend;
  unsigned int freelistsize;
};

/**
 * Another implementation of iNumReg. You should use it for client 
 * applications when IDs are allocated on server.
 */
class NumRegHash : public iNumReg
{
public:
  NumRegHash();
  virtual ~NumRegHash();

  SCF_DECLARE_IBASE;

  virtual CS_ID Register(void* obj);
  virtual void RegisterWithID(void* obj, CS_ID id);
  
  virtual bool Remove(CS_ID id);
  virtual bool Remove(void* obj);

  virtual void Clear();

  virtual void* Get(CS_ID id);
  virtual unsigned int Length();

private:
  csHash<void*,CS_ID> reg;
  int max_id;
};

#endif
