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

#ifndef __CEL_PF_HOVER_FACTORY__
#define __CEL_PF_HOVER_FACTORY__

#include "cstypes.h"

#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"

#include "propclass/hover.h"
#include "propclass/mesh.h"

#include "propclass/stabiliser_dist.h"
#include "../mechanics/ticktimer.h"

struct iPcMesh;
struct iPcMechanicsObject;
struct iVirtualClock;

/**
 * Factory for hover.
 */
CS_IMPLEMENT_PLUGIN

CEL_DECLARE_FACTORY (Hover)

/**
 * Hover stabiliser property class.
 */
class celPcHover : public celPcCommon , public celPeriodicTimer
{
public:
  celPcHover (iObjectRegistry* object_reg);
  virtual ~celPcHover ();

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName() const { return "pchover"; };
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params,
      celData& ret);
  virtual void Tick ();

  /**
   * Should Tick every delta and update the objects
   * height
   */
  virtual void PerformStabilising();

  virtual void SetWorldMesh (csRef<iPcMesh> wmesh) { world_mesh = wmesh; }
  virtual void SetWorld (const char *name);
  virtual void SetHeightBeamCutoff (float chm) { height_beam_cutoff = chm; }
  virtual void SetAngularBeamOffset (float abo) { ang_beam_offset = abo; }
  virtual void SetAngularCutoffHeight (float ach) { ang_cutoff_height = ach; }
  virtual void SetAngularCorrectionStrength (float mul) { ang_mult = mul; }
  virtual void SetStabiliserFunction (celStabiliserFunction *sfunc)
  { func.AttachNew (sfunc); }

  virtual void HoverOn () { hover_on = true; }
  virtual void HoverOff () { hover_on = false; }
  virtual void UseDefaultFunction (float dampening);
  virtual float GetHeight () { return object_height; }

  struct PcHover : public iPcHover
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcHover);

    virtual void SetWorldMesh (csRef<iPcMesh> wmesh)
    {
      scfParent->SetWorldMesh (wmesh);
    }
    virtual void SetWorld (const char *name)
    {
      scfParent->SetWorld (name);
    }
    virtual void SetHeightBeamCutoff (float chm)
    {
      scfParent->SetHeightBeamCutoff (chm);
    }
    virtual void SetAngularBeamOffset (float abo)
    {
      scfParent->SetAngularBeamOffset (abo);
    }
    virtual void SetAngularCutoffHeight (float ach)
    {
      scfParent->SetAngularCutoffHeight (ach);
    }
    virtual void SetAngularCorrectionStrength (float mul)
    {
      scfParent->SetAngularCorrectionStrength (mul);
    }
    virtual void SetStabiliserFunction (celStabiliserFunction *sfunc)
    {
      scfParent->SetStabiliserFunction (sfunc);
    }

    virtual void HoverOn ()
    {
      scfParent->HoverOn ();
    }
    virtual void HoverOff ()
    {
      scfParent->HoverOff ();
    }

    virtual void UseDefaultFunction (float dampening)
    {
      scfParent->UseDefaultFunction (dampening);
    }

    virtual float GetHeight ()
    {
      return scfParent->GetHeight ();
    }
  } scfiPcHover;

private:
  // Actions
  static csStringID action_setworld;
  static csStringID action_sethbeamcutoff;
  static csStringID action_setangoff;
  static csStringID action_setangheight;
  static csStringID action_setangstr;
  static csStringID action_usedeffunc;
  static csStringID action_hoveron;

  // Parameters.
  static csStringID param_world;
  static csStringID param_hbeamcutoff;
  static csStringID param_angoff;
  static csStringID param_angheight;
  static csStringID param_angstr;
  static csStringID param_hover;

  /**
   * Calculate height of object to ground (world mesh)
   */
  float Height(csVector3 offset = csVector3(0,0,0));

  /**
   * In case of object not being in range in Height(),
   * extend another beam upwards to calculate reverse distance,
   * else return infinity
   */
  float ReverseHeight(csVector3 &start);

  /**
   * This function computes the angle the ship has to roll through to
   * to be aligned with the terrain
   *
   * offset is which offset to use for hit beams to test difference in
   * heights. i.e csVector3(0,0,-1)
   *
   * height is the height at center of ship
   */
  float AngularAlignment(csVector3 offset , float height);

  /**
   * Looks up the world_mesh and gets the csWeakRef.
   */
  void LookUpWorldMesh ();

  /// so we can turn the hovering off and on.
  bool hover_on;

  /// because the ref isn't gotten until first tick
  csString world_mesh_name;
  /// mesh used for height calculation
  csWeakRef<iPcMesh> world_mesh;

  /// offset used for calculating alignment wrt to ground
  float ang_beam_offset;
  /// how fast the object aligns its orientation to the ground
  float ang_mult;
  /// cutoff height at which not to align with ground
  float ang_cutoff_height;
  /// cutoff height at which height test returns inf
  float height_beam_cutoff;
  /// function which computes upthrust depending on height
  csRef<celStabiliserFunction> func;

  /// last calculated height of object
  float object_height;

  /// stored ship mech interface
  csWeakRef<iPcMechanicsObject> ship_mech;
};

#endif
