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
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
#include "cssysdef.h"

#include "numreg.h"

//----------------------- NumRegLists --------------------------------------

SCF_IMPLEMENT_IBASE(NumRegLists)
  SCF_IMPLEMENTS_INTERFACE(iNumReg)
SCF_IMPLEMENT_IBASE_END

NumRegLists::NumRegLists(int limit, int freelistsize, int startsize)
{
  list = (void**) malloc(sizeof(void*)*startsize);
  memset ((void*) list, 0, sizeof(void*)*startsize);
  freelist = new CS_ID[freelistsize];
  listsize = startsize;
  NumRegLists::limit = limit;
  freelistend = 0;
  NumRegLists::freelistsize = freelistsize;
}

NumRegLists::~NumRegLists()
{
  if(list)
    free(list);
  if (freelist)
    delete[] freelist;
}

#define ADDSIZE	100

CS_ID NumRegLists::intern_Register(void* obj)
{
  freelistend--;
  CS_ASSERT (freelistend < freelistsize);
  CS_ASSERT (freelist[freelistend] < listsize);      
  list[freelist[freelistend]] = obj;

  return freelist[freelistend];  
}    

CS_ID NumRegLists::Register (void* obj)
{
  CS_ASSERT(obj != 0);
  
  // 1. try to fill up just released positions
  if (freelistend>0)
    return intern_Register(obj);

  // 2. find holes and fill freelist again
  // note that id number 0 stands for error and is reserved
  for (CS_ID i=1; i<listsize && freelistend<freelistsize ;i++)
  {
    if (list[i]==0)
    {
      CS_ASSERT (freelistend < freelistsize);
      freelist[freelistend]=i;
      freelistend++;
    }
  }
  if (freelistend>0)
    return intern_Register(obj);

  // 3. extend list and append
  if (listsize<limit)
  {
    CS_ID newsize;
    if (listsize>=limit-ADDSIZE)
      newsize=limit;
    else
      newsize=listsize+ADDSIZE;
      
    void** newlist = (void**) realloc((void*)list, newsize*sizeof(void*));
    //no memory left? stay with the old buffer
    if (!newlist)
    {
      CS_ASSERT(false && "No Memory left");
      return 0;
    }

    list = newlist;

    memset (list+listsize,0,(newsize-listsize)*sizeof(void*));
    //fill freelist
    for (CS_ID i=listsize;i<newsize && freelistend<freelistsize;i++)
    {
      CS_ASSERT (freelistend < freelistsize);
      freelist[freelistend]=i;
      freelistend++;	
    }
    listsize = newsize;
  }
  if (freelistend>0)
    return intern_Register(obj);
  
  //list has reached limit and is full
  return 0;
}

void NumRegLists::RegisterWithID (void* obj, CS_ID id)
{
  CS_ASSERT(Get(id) == 0);

  // First, grow the list if the id is too big.
  if (id >= listsize)
  {
    CS_ID newsize;
    if (listsize >= limit - ADDSIZE)
      newsize = limit;
    else
      newsize = listsize + ADDSIZE;
      
    list = (void**) realloc((void*)list, newsize*sizeof(void*));
  }

  if (id >= listsize)
  {
    // Limit was on the road. Lets crash
    CS_ASSERT (0 && "Limit reached");
  }

  list[id] = obj;

  // Remove the spot from the freelist. 
  for (size_t i = 0; i < freelistend; i++)
  {
    if (freelist[i] == id)
      freelistend = i;
  }
}

bool NumRegLists::Remove (CS_ID num)
{
  CS_ASSERT(num<listsize);
  CS_ASSERT(list[num] != 0);
  
  if (freelistend < freelistsize-1)
    freelist[freelistend++] = num;

  list[num]=0;

  return true;
}

bool NumRegLists::Remove (void* obj)
{
  CS_ASSERT(obj != 0);
  
  CS_ID i;
  // start loop at 1 because 0 is reserved as error value
  for (i=1;i<listsize;i++)
  {
    if (list[i] == obj)
    {
      Remove(i);
      break;
    }
  }
  if (i>=listsize)
    return false;

  return true;
}

void NumRegLists::Clear()
{
  for (CS_ID i=0;i<listsize;i++)
  {
    list[i]=0;
  }
  freelistend=0;
} 

//------------------------ NumRegHash --------------------------------------
SCF_IMPLEMENT_IBASE(NumRegHash)
  SCF_IMPLEMENTS_INTERFACE(iNumReg)
SCF_IMPLEMENT_IBASE_END

NumRegHash::NumRegHash()
{
  max_id = 0;
}

NumRegHash::~NumRegHash()
{
  Clear();
}

CS_ID NumRegHash::Register(void* obj)
{
  CS_ID id = max_id;

  reg.Put(max_id, obj);
  max_id++;

  return id;
}

void NumRegHash::RegisterWithID(void* obj, CS_ID id)
{
  CS_ASSERT(Get(id) == 0);
  
  reg.Put(id, obj);
}

bool NumRegHash::Remove(CS_ID id)
{
  return reg.DeleteAll(id);
}

bool NumRegHash::Remove(void* obj)
{
  csHash<void*,CS_ID>::GlobalIterator it = reg.GetIterator();

  while (it.HasNext())
  {
    if (it.NextNoAdvance() == obj)
      reg.DeleteElement(it);

    it.Advance();
  }

  return true;
}

void NumRegHash::Clear()
{
  reg.DeleteAll();
}

void* NumRegHash::Get(CS_ID id)
{
  return reg.Get(id, 0);
}

unsigned int NumRegHash::Length()
{
  return reg.GetSize();
}
