/*
    Crystal Space Entity Layer
    Copyright (C) 2003 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_COMMON_PARAMS__
#define __CEL_PF_COMMON_PARAMS__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/strhash.h"
#include "csutil/util.h"
#include "behaviourlayer/behave.h"

/**
 * Generic parameter block implementation.
 */
class celGenericParameterBlock : public iCelParameterBlock
{
private:
  int count;
  csStringID* ids;
  celData* data;
  char** names;

public:
  celGenericParameterBlock (int count)
  {
    SCF_CONSTRUCT_IBASE (0);
    celGenericParameterBlock::count = count;
    ids = new csStringID[count];
    data = new celData[count];
    names = new char*[count];
    memset (names, 0, sizeof (char*)*count);
  }
  virtual ~celGenericParameterBlock ()
  {
    delete[] ids;
    delete[] data;
    int i;
    for (i = 0 ; i < count ; i++)
      delete[] names[i];
    delete[] names;
  }

  void SetParameterDef (int idx, csStringID id, const char* parname,
  	celDataType type)
  {
    ids[idx] = id;
    data[idx].type = type;
    delete[] names[idx];
    names[idx] = csStrNew (parname);
  }
  celData& GetParameter (int idx) { return data[idx]; }

  SCF_DECLARE_IBASE;

  virtual int GetParameterCount () const { return count; }
  virtual const char* GetParameter (int idx, csStringID& id,
  	celDataType& t) const
  {
    if (idx < 0 || idx >= count)
    {
      id = csInvalidStringID;
      t = CEL_DATA_NONE;
      return 0;
    }
    id = ids[idx];
    t = data[idx].type;
    return names[idx];
  }
  virtual const celData* GetParameter (csStringID id) const
  {
    int i;
    for (i = 0 ; i < count ; i++)
      if (id == ids[i])
        return &data[i];
    return 0;
  }
};

/**
 * Parameter block implementation if only one parameter is desired.
 */
class celOneParameterBlock : public iCelParameterBlock
{
private:
  csStringID id;
  celData data;
  char* name;

public:
  celOneParameterBlock ()
  {
    SCF_CONSTRUCT_IBASE (0);
    name = 0;
  }
  virtual ~celOneParameterBlock ()
  {
    delete[] name;
  }

  void SetParameterDef (csStringID id, const char* parname,
  	celDataType type)
  {
    celOneParameterBlock::id = id;
    data.type = type;
    delete[] name;
    name = csStrNew (parname);
  }
  celData& GetParameter (int) { return data; }

  SCF_DECLARE_IBASE;

  virtual int GetParameterCount () const { return 1; }
  virtual const char* GetParameter (int idx, csStringID& id,
  	celDataType& t) const
  {
    if (idx != 0)
    {
      id = csInvalidStringID;
      t = CEL_DATA_NONE;
      return 0;
    }
    id = celOneParameterBlock::id;
    t = data.type;
    return name;
  }
  virtual const celData* GetParameter (csStringID id) const
  {
    if (id != celOneParameterBlock::id) return 0;
    return &data;
  }
};

#endif // __CEL_PF_COMMON_PARAMS__

