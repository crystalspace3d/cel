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

struct iCelEntity;
struct iObjectRegistry;

/**
 * Factory for mover.
 */
CEL_DECLARE_FACTORY (Mover)

/**
 * This is the Mover property class.
 */
class celPcMover : public celPcCommon
{
private:
  // For PerformAction.
  static csStringID id_position;
  static csStringID id_up;
  static csStringID id_movespeed;
  static csStringID id_rotatespeed;
  static csStringID id_sqradius;
  static csStringID action_start;
  static csStringID action_interrupt;

  // For properties.
  enum propids
  {
    propid_position = 0,
    propid_up,
    propid_movespeed,
    propid_rotatespeed,
    propid_sqradius,
    propid_moving
  };
  static Property* properties;
  static int propertycount;
  static void UpdateProperties (iObjectRegistry* object_reg);

  // Normal fields.
  csVector3 position;
  csVector3 up;
  float movespeed;
  float rotatespeed;
  float sqradius;
  bool is_moving;

public:
  celPcMover (iObjectRegistry* object_reg);
  virtual ~celPcMover ();

  void SetLinMove (iPcLinearMovement* pclinmove) { }
  bool Start (const csVector3& position, const csVector3& up,
  	float movespeed, float rotatespeed, float sqradius);
  void Interrupt ();
  const csVector3& GetPosition () const { return position; }
  const csVector3& GetUp () const { return up; }
  float GetMoveSpeed () const { return movespeed; }
  float GetRotateSpeed () const { return rotatespeed; }
  float GetSqRadius () const { return sqradius; }
  bool IsMoving () const { return is_moving; }

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcmover"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params);

  struct PcMover : public iPcMover
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcMover);
    virtual void SetLinMove (iPcLinearMovement* pclinmove)
    {
      scfParent->SetLinMove (pclinmove);
    }
    virtual bool Start (const csVector3& position, const csVector3& up,
  	float movespeed, float rotatespeed, float sqradius)
    {
      return scfParent->Start (position, up, movespeed, rotatespeed, sqradius);
    }
    virtual void Interrupt ()
    {
      scfParent->Interrupt ();
    }
    virtual const csVector3& GetPosition () const
    {
      return scfParent->GetPosition ();
    }
    virtual const csVector3& GetUp () const
    {
      return scfParent->GetUp ();
    }
    virtual float GetMoveSpeed () const
    {
      return scfParent->GetMoveSpeed ();
    }
    virtual float GetRotateSpeed () const
    {
      return scfParent->GetRotateSpeed ();
    }
    virtual float GetSqRadius () const
    {
      return scfParent->GetSqRadius ();
    }
    virtual bool IsMoving () const
    {
      return scfParent->IsMoving ();
    }
  } scfiPcMover;
};

#endif // __CEL_PF_MOVERFACT__

