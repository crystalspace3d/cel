/*
    Crystal Space Entity Layer
    Copyright (C) 2004 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_SPAWNFACT__
#define __CEL_PF_SPAWNFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "csutil/weakref.h"
#include "csutil/parray.h"
#include "csutil/weakref.h"
#include "csutil/hash.h"
#include "csutil/hashhandlers.h"
#include "csutil/stringarray.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/spawn.h"

struct iCelEntity;
struct iCelPlLayer;
struct iObjectRegistry;
struct iVirtualClock;

/**
 * Factory for tools.
 */
CEL_DECLARE_FACTORY(Spawn)

// Spawn info.
struct SpawnInfo
{
  float chance;
  char* name;
  iCelBlLayer* bl;
  char* behaviour;
  csStringArray pcs;

  ~SpawnInfo ()
  {
    delete[] name;
    delete[] behaviour;
  }
};
	
/**
 * This is a spawn property class.
 */
class celPcSpawn : public celPcCommon
{
private:
  csWeakRef<iCelPlLayer> pl;
  csRef<iVirtualClock> vc;
  bool enabled;
  csTicks mindelay, maxdelay, delay_todo;
  bool repeat;
  bool random;
  size_t sequence_cur;

  csArray<SpawnInfo> spawninfo;
  float total_chance;

  static csStringID id_entity;
  static csStringID id_behaviour;
  celGenericParameterBlock* params;

public:
  celPcSpawn (iObjectRegistry* object_reg);
  virtual ~celPcSpawn ();

  bool HandleEvent (iEvent& ev);
  void SetEnabled (bool e) { enabled = e; }
  bool IsEnabled () const { return enabled; }
  void AddEntityType (float chance, const char* name, iCelBlLayer* bl,
		  const char* behaviour, va_list behaviours);
  void ClearEntityList ();
  void SetTiming (bool repeat, bool random,
		  csTicks mindelay, csTicks maxdelay);
  void Reset ();

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcspawn"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params);

  struct PcSpawn : public iPcSpawn
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcSpawn);
    virtual void SetEnabled (bool e)
    {
      scfParent->SetEnabled (e);
    }
    virtual bool IsEnabled () const
    {
      return scfParent->IsEnabled ();
    }
    virtual void AddEntityType (float chance, const char* name, iCelBlLayer* bl,
		  const char* behaviour, ...)
    {
      va_list arg;
      va_start (arg, behaviour);
      scfParent->AddEntityType (chance, name, bl, behaviour, arg);
      va_end (arg);
    }
    virtual void ClearEntityList ()
    {
      scfParent->ClearEntityList ();
    }
    virtual void SetTiming (bool repeat, bool random,
		  csTicks mindelay, csTicks maxdelay)
    {
      scfParent->SetTiming (repeat, random, mindelay, maxdelay);
    }
    virtual void Reset ()
    {
      scfParent->Reset ();
    }
  } scfiPcSpawn;

  // Not an embedded interface to avoid circular references!!!
  class EventHandler : public iEventHandler
  {
  private:
    celPcSpawn* parent;

  public:
    EventHandler (celPcSpawn* parent)
    {
      SCF_CONSTRUCT_IBASE (0);
      EventHandler::parent = parent;
    }
    virtual ~EventHandler () { }

    SCF_DECLARE_IBASE;
    virtual bool HandleEvent (iEvent& ev)
    {
      return parent->HandleEvent (ev);
    }
  } *scfiEventHandler;
};

#endif // __CEL_PF_SPAWNFACT__

