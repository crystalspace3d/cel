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
#include "cssysdef.h"

#include "numreg.h"

NumReg::NumReg(int limit, int freelistsize, int startsize)
{
  list = (void**) malloc(sizeof(void*)*startsize);
  memset ((void*) list, 0, sizeof(void*)*startsize);
  freelist = new CS_ID[freelistsize];
  buflen = startsize;
  NumReg::limit = limit;
  listend = 1;
  freelistend = 0;
  freelistlimit = freelistsize;
}

NumReg::~NumReg()
{
  if(list)
    free(list);
  if (freelist)
    delete[] freelist;
}

#define ADDSIZE	400

CS_ID NumReg::Register (void* obj)
{
  if (obj==NULL)
    return 0;
  
  // 1. try to fill up just released positions
  if (freelistend>0)
  {
    freelistend--;
    list[freelist[freelistend]] = obj;
    return freelistend;
  }

  // 2. extend list and append
  if (listend>=buflen)
  {
    //increase buffer size if limit isn't reached
    if (buflen<limit) {
      int newlen;
      if (buflen>=limit-ADDSIZE)
	newlen=limit;
      else
	newlen=buflen+ADDSIZE;
      
      void** newlist = (void**) realloc((void*)list, newlen*sizeof(void*));
      //no memory left? stay with the old buffer
      if (!newlist)
	limit=buflen;
      else
      {
	memset (list+buflen,0,newlen-buflen);
	buflen = newlen;
	list = newlist;
      }
    }
  }
  if (listend<buflen)
  {
    list[listend]=obj;
    listend++;
    return listend-1;
  }
  
  // 3. find wholes and fill freelist again
  for (CS_ID i=1;i<buflen;i++) {
    if (list[i]==NULL)
    {
      freelist[freelistend]=i;
      freelistend++;
    }
  }
  if (freelistend>0)
  {
    freelistend--;
    list[freelist[freelistend]] = obj;
    return freelistend;
  }
  
  return 0;
}

bool NumReg::Remove (CS_ID num)
{
#ifdef CEL_DEBUG
  if (list[num]==NULL)
    return false;
#endif
  
  if (freelistend<=freelistlimit)
  {
    freelist[freelistend]=num;
    freelistend++;
  }
  list[num]=NULL;

  return true;
}

