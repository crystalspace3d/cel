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

#ifndef __CEL_PF_MECHANICS_SYSTEM_FACTORY__
#define __CEL_PF_MECHANICS_SYSTEM_FACTORY__

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
#include "celtool/stdparams.h"
#include "propclass/mechsys.h"
#include "propclass/mechthruster.h"
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
 * Factory for mechthruster.
 */
CEL_DECLARE_FACTORY(MechanicsThrusterReactionary)

/**
 * This is a thruster.
 */
class celPcMechanicsThrusterReactionary : public celPcCommon
{
private:
  // Actions
  static csStringID action_initthruster;

  // Parameters for action_initthruster
  static csStringID param_object;
  static csStringID param_position;
  static csStringID param_orientation;
  static csStringID param_maxthrust;

  // Data members
  csWeakRef<iPcMechanicsObject> mechanicsobject;
  celThrusterRequestData request;
  csVector3 position;
  csVector3 orientation;
  float maxthrust;

public:
  celPcMechanicsThrusterReactionary (iObjectRegistry* object_reg);
  virtual ~celPcMechanicsThrusterReactionary ();

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcmechthrustreactionary"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params);
  virtual void TickEveryFrame ();

  virtual void SetMechanicsObject (iPcMechanicsObject* mechobj)
  {
    mechanicsobject = mechobj;
  }

  virtual iPcMechanicsObject* GetMechanicsObject ()
  {
    return mechanicsobject;
  }

  virtual void SetPosition (const csVector3& pos);

  virtual const csVector3& GetPosition ();

  virtual void SetOrientation (const csVector3& orientation);

  virtual const csVector3& GetOrientation ();

  virtual void SetMaxThrust (float maxthrust);

  virtual float GetMaxThrust ();

  virtual float GetEffectiveMaxThrust ();

  virtual void ThrustRequest (iPcMechanicsThrusterGroup* group, float thrust);

  virtual void CancelThrustRequest (iPcMechanicsThrusterGroup* group);

  virtual bool IsAvailable ()
  {
    return (fabs (request.thrust) < 0.000001f);
  }

  struct PcMechanicsThruster : public iPcMechanicsThruster
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcMechanicsThrusterReactionary);

    virtual void SetMechanicsObject (iPcMechanicsObject* mechsys)
    {
      scfParent->SetMechanicsObject (mechsys);
    }

    virtual iPcMechanicsObject* GetMechanicsObject ()
    {
      return scfParent->GetMechanicsObject ();
    }

    virtual void SetPosition (const csVector3& pos)
    {
      scfParent->SetPosition (pos);
    }

    virtual const csVector3& GetPosition ()
    {
      return scfParent->GetPosition ();
    }

    virtual void SetOrientation (const csVector3& orientation)
    {
      scfParent->SetOrientation (orientation);
    }

    virtual const csVector3& GetOrientation ()
    {
      return scfParent->GetOrientation ();
    }

    virtual void SetMaxThrust (float maxthrust)
    {
      scfParent->SetMaxThrust (maxthrust);
    }

    virtual float GetMaxThrust ()
    {
      return scfParent->GetMaxThrust ();
    }

    virtual float GetEffectiveMaxThrust ()
    {
      return scfParent->GetEffectiveMaxThrust ();
    }

    virtual void ThrustRequest (iPcMechanicsThrusterGroup* group, float thrust)
    {
      scfParent->ThrustRequest (group, thrust);
    }

    virtual void CancelThrustRequest (iPcMechanicsThrusterGroup* group)
    {
      scfParent->CancelThrustRequest (group);
    }

    virtual bool IsAvailable ()
    {
      return scfParent->IsAvailable ();
    }
  } scfiPcMechanicsThruster;

  struct CelTimerListener : public iCelTimerListener
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcMechanicsThrusterReactionary);
    virtual void TickEveryFrame ()
    {
      scfParent->TickEveryFrame ();
    }
    virtual void TickOnce ()
    {
      return;
    }
  } scfiCelTimerListener;
};

#endif // __CEL_PF_MECHANICS_SYSTEM_FACTORY__

