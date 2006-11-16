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

#ifndef __CEL_PF_MOVERFACT__
#define __CEL_PF_MOVERFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/mover.h"
#include "propclass/linmove.h"
#include "propclass/actormove.h"
#include "propclass/mesh.h"
#include "iengine/engine.h"

struct iCelEntity;
struct iObjectRegistry;

/**
 * Factory for mover.
 */
CEL_DECLARE_FACTORY (Mover)

/**
 * This is the Mover property class.
 */
class celPcMover : public scfImplementationExt1<
	celPcMover, celPcCommon, iPcMover>
{
private:
  csWeakRef<iEngine> engine;
  csWeakRef<iPcLinearMovement> pclinmove;
  csWeakRef<iPcActorMove> pcactormove;
  csWeakRef<iPcMesh> pcmesh;

  // For actions.
  static csStringID id_sectorname;
  static csStringID id_position;
  static csStringID id_up;
  static csStringID id_sqradius;

  enum actionids
  {
    action_start = 0,
    action_interrupt
  };

  // For properties.
  enum propids
  {
    propid_position = 0,
    propid_up,
    propid_sqradius,
    propid_moving
  };
  static PropertyHolder propinfo;

  // Normal fields.
  iSector* sector;
  csVector3 position;
  csVector3 up;
  float sqradius;
  bool is_moving;

  void FindSiblingPropertyClasses ();
  void SendMessage (const char* msg, const char* meshname = 0);
  void StopMovement ();

  static csStringID id_meshname;
  celOneParameterBlock* params;

public:
  celPcMover (iObjectRegistry* object_reg);
  virtual ~celPcMover ();

  virtual bool Start (iSector* sector, const csVector3& position,
      const csVector3& up, float sqradius);
  virtual void Interrupt ();
  virtual iSector* GetSector () const { return sector; }
  virtual const csVector3& GetPosition () const { return position; }
  virtual const csVector3& GetUp () const { return up; }
  virtual float GetSqRadius () const { return sqradius; }
  virtual bool IsMoving () const { return is_moving; }

  virtual const char* GetName () const { return "pcmover"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformActionIndexed (int idx, iCelParameterBlock* params,
      celData& ret);
  virtual void TickOnce ();
};

#endif // __CEL_PF_MOVERFACT__

