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

#include "csutil/csstring.h"

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

NumRegHash::NumRegHash (int size)
{
  current_id = 1;
  limit = size;
}

NumRegHash::~NumRegHash ()
{
  Clear();
}

CS_ID NumRegHash::Register (void* obj)
{
  while (reg.Get(current_id, 0))
    current_id++;

  if (current_id >= limit)
  {
    current_id = 1;
    while (reg.Get(current_id, 0))
      current_id++;

    if (current_id >= limit)
    {
      fprintf (stderr, "Warning: ID scope full.");
      return 0;
    }
  }

  reg.Put(current_id, obj);

  return current_id;
}

void NumRegHash::RegisterWithID (void* obj, CS_ID id)
{
  CS_ASSERT (Get (id) == 0);
  CS_ASSERT (id < limit);
  
  reg.Put (id, obj);
}

bool NumRegHash::Remove (CS_ID id)
{
  return reg.DeleteAll (id);
}

bool NumRegHash::Remove (void* obj)
{
  csHash<void*,CS_ID>::GlobalIterator it = reg.GetIterator ();

  while (it.HasNext ())
  {
    if (it.NextNoAdvance () == obj)
      reg.DeleteElement(it);

    it.Advance ();
  }

  return true;
}

void NumRegHash::Clear ()
{
  reg.DeleteAll ();
}

void* NumRegHash::Get (CS_ID id)
{
  return reg.Get (id, 0);
}

unsigned int NumRegHash::Length ()
{
  return reg.GetSize ();
}

//------------------------- celIDRegistry ---------------------------------
celIDRegistry::celIDRegistry () :
  regs (2) // Most uses of registry will have either one or two scopes.
{
  DefaultScope = AddScope ("cel.numreg.lists", 100000);
}

celIDRegistry::~celIDRegistry ()
{
  for (size_t i = 0; i < regs.Length(); i++)
    delete regs[i].numreg;

  regs.DeleteAll ();
}

int celIDRegistry::GetScopeOfID (CS_ID id)
{
  for (size_t i = 0; i < regs.Length (); i++)
  {
    if (regs[i].start < id && id < regs[i].end)
      return i;
  }

  return -1;
}

int celIDRegistry::AddScope (csString impl, int size)
{
  int id = regs.Length ();
  int start;

  if (id == 0)
    start = 0;
  else
    start = regs[id-1].end;

  iNumReg* nr;
  if (impl == "cel.numreg.lists")
    nr = new NumRegLists (size);
  else if (impl == "cel.numreg.hash")
    nr = new NumRegHash (size);
  else
    fprintf (stderr, 
	     "celIDRegistry: %s: no such implementation", impl.GetData ());

  struct part p;
  p.numreg = nr;
  p.start = start;
  p.end = size + start;

  regs.Push (p);

  return id;
}

CS_ID celIDRegistry::Register (void* obj, int scope)
{
  CS_ID local_id = regs[scope].numreg->Register (obj);

  if (local_id == 0)
    return 0;
  else
    return local_id + regs[scope].start;
}

void celIDRegistry::RegisterWithID (void* obj, CS_ID id)
{
  int scope = GetScopeOfID (id);
  CS_ASSERT (scope != -1);

  regs[scope].numreg->RegisterWithID (obj, id - regs[scope].start);
}

bool celIDRegistry::Remove (CS_ID id)
{
  int scope = GetScopeOfID (id);
  if (scope == -1)
    return false;

  return regs[scope].numreg->Remove (id - regs[scope].start);
}

bool celIDRegistry::Remove (void* obj)
{
  for (size_t i = 0; i < regs.Length (); i++)
  {
    if (regs[i].numreg->Remove (obj))
      return true;
  }

  return false;
}

void celIDRegistry::Clear ()
{
  for (size_t i = 0; i < regs.Length (); i++)
    regs[i].numreg->Clear();
}

void* celIDRegistry::Get (CS_ID id)
{
  int scope = GetScopeOfID (id);
  if (scope == -1)
    return 0;

  return regs[scope].numreg->Get (id - regs[scope].start);
}
