
#include "cstypes.h"

/**
 * This class stores a list of Objects and assignes a unique ID to them
 *  Note: This class does nearly no error checking  and should be used with
 *  care
 */
class NumReg
{
public:
  /** Constructor, limit is the maximum size this array will grow,
   *   freelistsize is the size of the list holding last freed items (the
   *   bigger this list is the better your performance if you remove/add new
   *   elements often.
   *   startsize is the initial size of the array
   */
  NumReg(int limit=100000, int freelistsize=100, int startsize=300);
  /// The destructor frees all memory
  ~NumReg();

  /** Registers an object in the registry and returns the new ID, in error
   * case ID 0 is returned
   */
  CS_ID Register (void* obj);
  /** Removes an registered object from registry */
  bool Remove (CS_ID id);
  /** Removes an registered object from registry (Note: this is slow you
   * should use Remove(id) if possible
   */
  bool Remove (void* obj);
  
  /** Removes all objects from the list */
  void Clear();

  /** Returns the object with ID id from the list */
  void* Get(CS_ID id)
  { 
    if (id>=listsize)
      return NULL;
    
    return list[id];
  }

  // Returns the size of the buffer (This is NOT the count of objects in the
  // registry)
  unsigned int Length()
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

