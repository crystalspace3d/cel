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

NumReg::NumReg(int limit, int freelistsize, int startsize)
{
  list = (void**) malloc(sizeof(void*)*startsize);
  memset ((void*) list, 0, sizeof(void*)*startsize);
  freelist = new CS_ID[freelistsize];
  listsize = startsize;
  NumReg::limit = limit;
  freelistend = 0;
  NumReg::freelistsize = freelistsize;
}

NumReg::~NumReg()
{
  if(list)
    free(list);
  if (freelist)
    delete[] freelist;
}

#define ADDSIZE	100

CS_ID NumReg::intern_Register(void* obj)
{
  freelistend--;
  CS_ASSERT (freelistend < freelistsize);
  CS_ASSERT (freelist[freelistend] < listsize);      
  list[freelist[freelistend]] = obj;

  return freelist[freelistend];  
}    

CS_ID NumReg::Register (void* obj)
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

bool NumReg::Remove (CS_ID num)
{
  CS_ASSERT(num<listsize);
  CS_ASSERT(list[num] != 0);
  
  if (freelistend < freelistsize-1)
    freelist[freelistend++] = num;

  list[num]=0;

  return true;
}

bool NumReg::Remove (void* obj)
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

void NumReg::Clear()
{
  for (CS_ID i=0;i<listsize;i++)
  {
    list[i]=0;
  }
  freelistend=0;
} 

