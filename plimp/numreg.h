
#include "cstypes.h"

/**
 * This class stores a list of Objects and assignes a unique ID to them
 *  NOte: This class does nearly no error checking  and should be used with
 *  care
 */
class NumReg
{
public:
  NumReg(int limit=100000, int freelistsize=100, int startsize=300);
  ~NumReg();

  CS_ID Register (void* obj);
  bool Remove (CS_ID id);

  CS_ID GetCount()	{ return listend; }
  void* Get(CS_ID id)	{ return list[id]; }
  
protected:
  void** list;
  CS_ID* freelist;
  unsigned int buflen;
  unsigned int limit;
  unsigned int listend;
  unsigned int freelistend;
  unsigned int freelistlimit;
};

