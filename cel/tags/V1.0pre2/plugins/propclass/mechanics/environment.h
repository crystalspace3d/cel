/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_MECHANICS_ENVIRONMENT_FACTORY__
#define __CEL_PF_MECHANICS_ENVIRONMENT_FACTORY__

#include "cstypes.h"
#include "csgeom/transfrm.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "csutil/weakref.h"
#include "csutil/refarr.h"
#include "csutil/parray.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "propclass/mechenv.h"
#include "propclass/mesh.h"
#include "ivaria/dynamics.h"

struct iVirtualClock;
struct iCelPlLayer;
struct iCelEntity;
struct iCelEntityList;
class csVector3;
class csReversibleTransform;
class csColliderWrapper;

/**
 * Movement property class for use in sectors containing a fluid.
 */
class celPcFluidMove : public celPcCommon
{
private:
  csRef<iDynamics> dynamics;
  csRef<iDynamicSystem> dynsystem;
  csRef<iVirtualClock> vc;
  csArray<celForce> forces;
  float delta;

  void ProcessForces (float dt);

public:
  celPcFluidMove (iObjectRegistry* object_reg);
  virtual ~celPcFluidMove ();

  void SetDrag (float dragcoefficient);
  float GetDrag ();
  void SetLift (float density);
  float GetLift ();
  const csVector3& CalculateFluidForces ();

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcfluidmove"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual void TickEveryFrame ();

  struct PcFluidMove : public iPcFluidMove
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcFluidMove);
    virtual void SetDrag (float dragcoefficient)
    {
      scfParent->SetDrag (dragcoefficient);
    }
    virtual float GetDrag ()
    {
      return scfParent->GetDrag ();
    }
    virtual void SetLift (float density)
    {
      scfParent->SetLift (density);
    }
    virtual float GetLift ()
    {
      return scfParent->GetLift ();
    }
    virtual const csVector3& CalculateFluidForces ();
    {
      return scfParent->CalculateFluidForces ();
    }
  } scfiPcFluidMove;
};

#endif // __CEL_PF_MECHANICS_ENVIRONMENT_FACTORY__

