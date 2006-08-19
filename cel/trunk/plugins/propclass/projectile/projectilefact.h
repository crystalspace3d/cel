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

#ifndef __CEL_PF_PROJECTILEFACT__
#define __CEL_PF_PROJECTILEFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/virtclk.h"
#include "csutil/scf.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/projectile.h"
#include "propclass/mesh.h"
#include "propclass/linmove.h"

struct iCelEntity;
struct iObjectRegistry;

/**
 * Factory for projectile.
 */
CEL_DECLARE_FACTORY (Projectile)

/**
 * This is a projectile property class.
 */
class celPcProjectile : public scfImplementationExt1<
	celPcProjectile, celPcCommon, iPcProjectile>
{
private:
  // For SendMessage parameters.
  static csStringID id_entity;
  static csStringID id_intersection;
  csRef<celVariableParameterBlock> params;

  // For PerformAction.
  // id_message is shared.
  static csStringID action_start;
  static csStringID id_direction;
  static csStringID id_speed;
  static csStringID id_maxdist;
  static csStringID id_maxhits;
  static csStringID action_interrupt;

  // For properties.
  enum propids
  {
    propid_moving = 0
  };
  static PropertyHolder propinfo;

  void FindSiblingPropertyClasses ();
  void SendMessage (const char* msg);
  void SendMessage (const char* msg, iCelEntity* hitent,
  	const csVector3& isect);

  csRef<iVirtualClock> vc;
  csWeakRef<iPcLinearMovement> pclinmove;
  csWeakRef<iPcMesh> pcmesh;

  bool is_moving;

  int maxhits;
  int curhits;
  csTicks start_time;

  // Used for straight movement from a point in some direction.
  csVector3 start;
  csVector3 direction;
  float speed;
  float maxdist;

public:
  celPcProjectile (iObjectRegistry* object_reg);
  virtual ~celPcProjectile ();

  virtual bool Start (const csVector3& direction,
  	float speed, float maxdist, int maxhits);
  virtual void Interrupt ();
  virtual bool IsMoving () const { return is_moving; }

  virtual const char* GetName () const { return "pcprojectile"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params,
      celData& ret);
  virtual void TickEveryFrame ();

  virtual bool GetPropertyBool (csStringID);
};

#endif // __CEL_PF_PROJECTILEFACT__

