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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __CEL_PL_PERSIST__
#define __CEL_PL_PERSIST__

#include "cstypes.h"

struct iCelPropertyClass;
struct iCelEntity;

enum celDataType
{
  CEL_DATA_NONE = 0,
  CEL_DATA_BYTE,
  CEL_DATA_WORD,
  CEL_DATA_LONG,
  CEL_DATA_UBYTE,
  CEL_DATA_UWORD,
  CEL_DATA_ULONG,
  CEL_DATA_FLOAT,
  CEL_DATA_STRING,
  CEL_DATA_PCLASS,
  CEL_DATA_ENTITY
};

struct celData
{
  celDataType type;
  char* name;
  union
  {
    int8 b;
    uint8 ub;
    int16 w;
    uint16 uw;
    int32 l;
    uint32 ul;
    float f;
    char* s;
    iCelPropertyClass* pclass;
    iCelEntity* entity;
  } value;

  celData () : type (CEL_DATA_NONE), name (NULL) { }
  ~celData()
  {
    delete[] name;
    if (type == CEL_DATA_STRING) delete[] value.s;
  }
};

SCF_VERSION (iCelDataBuffer, 0, 0, 1);

/**
 * This interface describes persistable data.
 */
struct iCelDataBuffer : public iBase
{
  /**
   * Set the number of data entries.
   */
  virtual void SetDataCount (int cnt) = 0;

  /**
   * Get the number of data entries.
   */
  virtual int GetDataCount () const = 0;

  /**
   * Get a specific data entry.
   */
  virtual celData* GetData (int idx) const = 0;
};

#endif // __CEL_PL_PERSIST__

