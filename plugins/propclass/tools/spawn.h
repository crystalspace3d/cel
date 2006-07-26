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
#include "iengine/engine.h"
#include "csutil/scf.h"
#include "csutil/parray.h"
#include "csutil/hash.h"
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
  csString templ;
  csString name;
  iCelBlLayer* bl;
  csString behaviour;
  csString msg_id;
  csRef<iCelParameterBlock> params;
  csStringArray pcs;
  csRef<iCelEntity> newent;
};

// Spawn position
struct SpawnPosition
{
  bool reserved;
  csVector3 pos;
  float yrot;
  csString node;
  csString sector;

  SpawnPosition ()
  {
    reserved = false;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    yrot = 0.0f;
  }
};

/**
 * This is a spawn property class.
 */
class celPcSpawn : public celPcCommon
{
private:
  csRef<iVirtualClock> vc;
  csRef<iEngine> engine;
  bool enabled;
  csTicks mindelay, maxdelay, delay_todo;
  bool repeat;
  bool random;
  size_t sequence_cur;
  csArray<SpawnInfo> spawninfo;
  csArray<SpawnPosition> spawnposition;
  float total_chance;
  int count;
  int inhibit_count;
  uint32 serialnr;

  static csStringID action_addentitytype;
  static csStringID action_addentitytpltype;
  static csStringID action_settiming;
  static csStringID action_resettiming;
  static csStringID action_setenabled;
  static csStringID action_clearentitylist;
  static csStringID action_inhibit;
  static csStringID action_spawn;
  static csStringID action_addspawnposition;
  static csStringID id_repeat_param;
  static csStringID id_random_param;
  static csStringID id_mindelay_param;
  static csStringID id_maxdelay_param;
  static csStringID id_chance_param;
  static csStringID id_entity_param;
  static csStringID id_template_param;
  static csStringID id_behaviour_param;
  static csStringID id_layer_param;
  static csStringID id_call_param;
  static csStringID id_enabled_param;
  static csStringID id_count_param;
  static csStringID id_sector_param;
  static csStringID id_position_param;
  static csStringID id_yrot_param;
  celGenericParameterBlock* params;
  static csStringID id_entity;
  static csStringID id_behaviour;

public:
  celPcSpawn (iObjectRegistry* object_reg);
  virtual ~celPcSpawn ();

  void SetEnabled (bool e) { enabled = e; }
  bool IsEnabled () const { return enabled; }
  void AddEntityType (float chance, const char* name, iCelBlLayer* bl,
  	const char* behaviour, const char* msg_id,
  	iCelParameterBlock* params, va_list pcclasses);
  void AddEntityTemplateType (float chance, const char* templ,
  	const char* name, const char* msg_id,
  	iCelParameterBlock* params);
  void ClearEntityList ();
  void SetTiming (bool repeat, bool random,
  	csTicks mindelay, csTicks maxdelay);
  void ResetTiming ();
  void InhibitCount (int number);
  void Spawn ();
  void AddSpawnPosition (const char* node, float yrot, const char* sector);
  void AddSpawnPosition (const csVector3& pos, float yrot, const char* sector);

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcspawn"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params,
      celData& ret);
  virtual void TickOnce ();
  virtual void SpawnEntityNr (size_t idx);
  virtual void Reset ();

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
    	const char* behaviour, const char* msg_id,
    	iCelParameterBlock* params, ...)
    {
      va_list arg;
      va_start (arg, params);
      scfParent->AddEntityType (chance, name, bl, behaviour, msg_id,
      	params, arg);
      va_end (arg);
    }
    virtual void AddEntityTemplateType (float chance, const char* templ,
    	const char* name, const char* msg_id,
    	iCelParameterBlock* params)
    {
      scfParent->AddEntityTemplateType (chance, templ, name, msg_id, params);
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
    virtual void ResetTiming ()
    {
      scfParent->ResetTiming ();
    }
    virtual void InhibitCount (int number)
    {
      scfParent->InhibitCount (number);
    }
    virtual void Spawn ()
    {
      scfParent->Spawn ();
    }
    virtual void AddSpawnPosition (const char* node, float yrot,
    	const char* sector)
    {
      scfParent->AddSpawnPosition (node, yrot, sector);
    }
    virtual void AddSpawnPosition (const csVector3& pos, float yrot,
    	const char* sector)
    {
      scfParent->AddSpawnPosition (pos, yrot, sector);
    }
  } scfiPcSpawn;
};

#endif // __CEL_PF_SPAWNFACT__
