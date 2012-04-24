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
#include "csutil/randomgen.h"
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
  csStringID msg_id;
  csRef<iCelParameterBlock> params;
};

// Spawn position
struct SpawnPosition
{
  csVector3 pos;
  float yrot;
  csString node;
  csString sector;

  SpawnPosition ()
  {
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    yrot = 0.0f;
  }
};

/**
 * This is a spawn property class.
 */
class celPcSpawn : public scfImplementationExt1<
	celPcSpawn, celPcCommon, iPcSpawn>
{
private:
  csRandomGen rng;
  csRef<iVirtualClock> vc;
  csRef<iEngine> engine;
  bool enabled;
  csTicks mindelay, maxdelay, delay_todo;
  bool repeat;
  bool random;
  size_t position_index;
  size_t sequence_cur;
  csArray<SpawnInfo> spawninfo;
  csArray<SpawnPosition> spawnposition;
  float total_chance;
  int count;
  int inhibit_count;
  uint32 serialnr;
  bool do_name_counter;

  bool do_spawn_unique;
  csSafeCopyArray<csWeakRef<iCelEntity> > uniqueEntities;

  csRef<iCelSpawner> spawner;

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
  csRef<celVariableParameterBlock> params;
  static csStringID id_entity;
  static csStringID id_behaviour;

  csRef<iMessageDispatcher> dispatcher_new;

  enum actionids
  {
    action_addentitytpltype = 0,
    action_settiming,
    action_resettiming,
    action_setenabled,
    action_clearentitylist,
    action_inhibit,
    action_spawn,
    action_addspawnposition
  };
  // For properties.
  enum propids
  {
    propid_namecounter = 0,
    propid_spawnunique
  };

  static PropertyHolder propinfo;

  void CountUniqueEntities ();
  void UpdateFreeUniqueEntity (iCelEntity* entity);

  iCelEntity* CreateNewEntity (iCelEntityTemplate* tpl,
      const char* entityName, iCelParameterBlock* params,
      const csVector3& pos, float yrot, const char* sector,
      const char* node);

  size_t GetPositionIndex ();

  bool atBaseline;

public:
  celPcSpawn (iObjectRegistry* object_reg);
  virtual ~celPcSpawn ();

  virtual void SetEntity (iCelEntity* entity);

  virtual void Activate ();
  virtual void Deactivate ();

  virtual void SetEnabled (bool e);
  virtual bool IsEnabled () const { return enabled; }
  virtual void AddEntityTemplateType (float chance, const char* templ,
  	const char* name, const char* msg_id,
  	iCelParameterBlock* params);
  virtual void ClearEntityList ();
  virtual void SetTiming (bool repeat, bool random,
  	csTicks mindelay, csTicks maxdelay);
  virtual void ResetTiming ();
  virtual void InhibitCount (int number);
  virtual void Spawn ();
  virtual void AddSpawnPosition (const char* node, float yrot,
      const char* sector);
  virtual void AddSpawnPosition (const csVector3& pos, float yrot,
      const char* sector);
  virtual void SetEntityNameCounter (bool en) { do_name_counter = en; }
  virtual bool IsEntityNameCounterCounter () const { return do_name_counter; }
  virtual void EnableSpawnUnique (bool en);
  virtual bool IsSpawnUniqueEnabled () const { return do_spawn_unique; }

  virtual bool PerformActionIndexed (int idx, iCelParameterBlock* params,
      celData& ret);
  virtual bool SetPropertyIndexed (int idx, bool b);
  virtual bool GetPropertyIndexed (int, bool&);
  virtual void TickOnce ();
  virtual void SpawnEntityNr (size_t idx);
  virtual void Reset ();

  virtual void AddEntityType (float chance, const char* name, iCelBlLayer* bl,
    	const char* behaviour, const char* msg_id,
    	iCelParameterBlock* params, ...)
  {
    va_list arg;
    va_start (arg, params);
    AddEntityType (chance, name, bl, behaviour, msg_id, params, arg);
    va_end (arg);
  }

  virtual void MarkBaseline ()
  {
    atBaseline = true;
  }
  virtual bool IsModifiedSinceBaseline () const { return !atBaseline; }
  virtual void SaveModifications (iCelCompactDataBufferWriter* buf, iStringSet* strings);
  virtual void RestoreModifications (iCelCompactDataBufferReader* buf,
      const csHash<csString,csStringID>& strings);
};

#endif // __CEL_PF_SPAWNFACT__
