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

#ifndef __CEL_PF_STEERFACT__
#define __CEL_PF_STEERFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/steer.h"
#include "propclass/linmove.h"
#include "propclass/actormove.h"
#include "propclass/mesh.h"
#include "iengine/engine.h"

struct iCelEntity;
struct iObjectRegistry;

/**
 * Factory for steer.
 */
CEL_DECLARE_FACTORY (Steer)


/**
 * This is the Steer property class.
 */
class celPcSteer : public scfImplementationExt1<
	celPcSteer, celPcCommon, iPcSteer>
{
private:
  csWeakRef<iEngine> engine;
  csWeakRef<iPcLinearMovement> pclinmove;
  csWeakRef<iPcActorMove> pcactormove;
  csWeakRef<iPcMesh> pcmesh;

  // For actions.
  static csStringID id_sectorname;
  static csStringID id_cur_sector;
  static csStringID id_position;
  static csStringID id_cur_position;
  static csStringID id_cur_direction;
  static csStringID id_up;
  static csStringID id_cur_yrot;
  static csStringID id_arrival_radius;
  static csStringID id_ca_lookahead;
  static csStringID id_ca_weight;
  
  enum actionids
  {
    action_seek=0,
    action_flee,
    action_interrupt
  };

  // For properties.
  enum propids
  {
    propid_position = 0,
    propid_cur_position,
    propid_cur_direction,
    propid_up,
    propid_cur_yrot,
    propid_moving,
    propid_check_arrival,
    propid_arrival_radius,
    propid_collision_avoidance,
    propid_ca_lookahead,
    propid_ca_weight,
  };
  static PropertyHolder propinfo;

  // Normal fields.
  iSector* sector;
  iSector* cur_sector;
  csVector3 position, cur_position, cur_direction, up;
  float cur_yrot, arrival_radius;
  float ca_lookahead, ca_weight;
  bool is_moving, check_arrival, collision_avoidance;
  int current_action;

  void FindSiblingPropertyClasses ();
  void SendMessage (const char* msg, const char* meshname = 0);
  void StopMovement ();

  static csStringID id_meshname;
  celOneParameterBlock* params;

public:
  celPcSteer (iObjectRegistry* object_reg);
  
  virtual ~celPcSteer ();

  virtual bool Seek (iSector* sector, const csVector3& position);

  virtual bool Flee(iSector* sector, const csVector3& position);

  virtual bool Move ();
      
  virtual void Interrupt ();

  virtual void CheckArrivalOn(float radius);

  virtual void CheckArrivalOff();

  virtual bool CheckArrival();
  
  virtual void CollisionAvoidanceOn(float lookahead, float weight);

  virtual void CollisionAvoidanceOff();

  virtual bool CollisionAvoidance();
  
  virtual iSector* GetSector () const { return sector; }
  
  virtual const csVector3& GetPosition () const { return position; }
  
  virtual const csVector3& GetUp () const { return up; }
  
  //virtual float GetSqRadius () const { return sqradius; }
  
  virtual bool IsMoving () const { return is_moving; }

  virtual csPtr<iCelDataBuffer> Save ();
  
  virtual bool Load (iCelDataBuffer* databuf);
  
  virtual bool PerformActionIndexed (int idx, iCelParameterBlock* params,
      celData& ret);
      
  virtual void TickOnce ();

};

#endif // __CEL_PF_STEERFACT__

