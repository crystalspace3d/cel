/*
    Crystal Space Entity Layer
    Copyright (C) 2006 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_TESTFACT__
#define __CEL_PF_TESTFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "iengine/engine.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/damage.h"
#include "propclass/mesh.h"

struct iCelEntity;
struct iObjectRegistry;
struct iSector;

/**
 * Factory for test.
 */
CEL_DECLARE_FACTORY (Damage)

/**
 * This is a test property class.
 */
class celPcDamage : public scfImplementationExt1<
	celPcDamage, celPcCommon, iPcDamage>
{
private:
  // For SendMessage parameters.
  static csStringID id_amount;
  static csStringID id_source;
  static csStringID id_sector;
  static csStringID id_position;
  static csStringID id_type;
  celVariableParameterBlock* params;

  // For PerformAction.
  static csStringID action_areadamage;
  static csStringID id_radius;
  static csStringID action_beamdamage;
  static csStringID id_direction;
  static csStringID id_maxdist;
  static csStringID action_singledamage;
  static csStringID id_target;

  // For properties.
  enum propids
  {
    propid_amount = 0,
    propid_type,
    propid_sector,
    propid_position
  };
  static PropertyHolder propinfo;

#define FALLOFF_CONSTANT 0
#define FALLOFF_LINEAR 1
#define FALLOFF_NORMAL 2
  int falloff;
  csString type;
  float amount;
  csString sector;
  csWeakRef<iSector> sector_ref;
  csVector3 position;
  csWeakRef<iPcMesh> pcmesh;
  csRef<iEngine> engine;

  void GetLocation (iSector*& s, csVector3& p);
  void DoDamage (iCelEntityList* list, const csVector3& p);
  void DoDamage (iCelEntity* ent, const csVector3& p);

public:
  celPcDamage (iObjectRegistry* object_reg);
  virtual ~celPcDamage ();

  virtual void SetDamage (float amount) { celPcDamage::amount = amount; }
  virtual float GetDamage () const { return amount; }

  virtual void SetDamageType (const char* type) { celPcDamage::type = type; }
  virtual const char* GetDamageType () const { return type; }

  virtual void SetFallOff (const char* f);
  virtual const char* GetFallOff () const;

  virtual void SetDamageLocation (const char* sectorname,
      const csVector3& pos);
  virtual const char* GetDamageSector () const { return sector; }
  virtual const csVector3& GetDamagePosition () const { return position; }

  virtual void AreaDamage (float radius);
  virtual void BeamDamage (const csVector3& direction, float maxdist);
  virtual void SingleDamage (const char* target);

  virtual const char* GetName () const { return "pcdamage"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params,
      celData& ret);

  // Override SetProperty from celPcCommon in order to provide support
  // for the 'max' property.
  virtual bool SetPropertyIndexed (int, const char*);
  virtual bool GetPropertyIndexed (int, const char*&);
};

#endif // __CEL_PF_TESTFACT__

