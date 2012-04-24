/*
    Crystal Space Entity Layer
    Copyright (C) 2001 by Jorrit Tyberghein

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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __CEL_PF_SIMPLE_CAMERA_FACTORY__
#define __CEL_PF_SIMPLE_CAMERA_FACTORY__

#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "csutil/weakref.h"
#include "csutil/weakrefarr.h"
#include "csutil/refarr.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "celtool/camera.h"
#include "propclass/simpcam.h"
#include "propclass/region.h"
#include "propclass/mesh.h"
#include "csgeom/transfrm.h"

struct iCelEntity;
struct iObjectRegistry;
struct iGraphics3D;
struct iEngine;
struct iVirtualClock;
struct iKeyboardDriver;
struct iMouseDriver;
struct iCollideSystem;
struct iView;
class csView;
class celPcSimpleCamera;
struct iPcZoneManager;

/**
 * Factory for engine stuff.
 */
CEL_DECLARE_FACTORY(SimpleCamera)

/**
 * This is a camera property class.
 */
class celPcSimpleCamera : public scfImplementationExt1<
	celPcSimpleCamera, celPcCameraCommon, iPcSimpleCamera>
{
private:
  // Actions.
  enum actionids
  {
    action_initcam = 0,
    action_setpos,
    action_setmesh
  };

  static PropertyHolder propinfo;

  // Parameters for action_initcam
  static csStringID param_campos;
  static csStringID param_lookat;
  static csStringID param_drawmesh;

  // Parameters for action_setmesh
  static csStringID param_mesh;

  //bool use_cd;
  bool drawmesh;
  csVector3 objectcampos;
  bool objectcampos_world;
  csVector3 objectlookat;

  bool meshExplicitlySet;

  csReversibleTransform actor_trans;
  iSector* actor_sector;

  csWeakRef<iPcMesh> pcmesh;
  void FindSiblingPropertyClasses ();

  iSector* lastActorSector;

  void GetActorTransform ();

public:
  celPcSimpleCamera (iObjectRegistry* object_reg);
  virtual ~celPcSimpleCamera ();

  /**
   * Set a flag to determine whether or not the attached mesh and its children
   * will be drawn in this camera view.
   * @param draw if true, the mesh will be drawn in this view.
   */
  virtual void SetDrawMesh (bool draw);

  /**
   * Sets the offset from the center of the mesh's iMovable to the position of
   * the camera.
   * \param offset the offset from the center of the mesh to the camera
   *        position.
   */
  virtual void SetCameraOffset (const csVector3& offset, bool world = false);

  /**
   * Sets the offset from the center of the mesh's iMovable to the point the
   * camera should look at.
   * \param lookat the offset from the center of the mesh to the lookat
   *        position.
   */
  virtual void SetLookAtOffset (const csVector3& lookat);

  /**
   * Set the mesh to follow.
   * \param mesh the mesh property class for the camera to follow.
   */
  virtual void SetMesh (iPcMesh* mesh);

  /**
   * Render. This will clear the screen then draw on top of it.
   */
  virtual void Draw();

  virtual void UpdateCamera ();

  virtual int GetDrawFlags ();

  virtual bool PerformActionIndexed (int idx, iCelParameterBlock* params,
      celData& ret);

  virtual bool SetRegion (iPcRegion* region, bool point = true,
        const char* name = 0)
  {
    return celPcCameraCommon::SetRegion (region, point, name);
  }
  virtual bool SetZoneManager (iPcZoneManager* zonemgr, bool point,
        const char* regionname, const char* name = 0)
  {
    return celPcCameraCommon::SetZoneManager (zonemgr, point, regionname, name);
  }
  virtual bool SetZoneManager (const char* entityname, bool point,
        const char* regionname, const char* name = 0)
  {
    return celPcCameraCommon::SetZoneManager (entityname,
	point, regionname, name);
  }
  virtual void SetRectangle (int x, int y, int w, int h)
  {
    celPcCameraCommon::SetRectangle (x, y, w, h);
  }
  virtual void SetPerspectiveCenter (float x, float y)
  {
    celPcCameraCommon::SetPerspectiveCenter (x, y);
  }
  virtual iCamera* GetCamera () const
  {
    return celPcCameraCommon::GetCamera ();
  }
  virtual iView* GetView () const
  {
    return celPcCameraCommon::GetView ();
  }
  virtual void SetClearZBuffer (bool flag)
  {
  }
  virtual bool GetClearZBuffer () const
  {
    return false;
  }
  virtual void SetClearScreen (bool flag)
  {
  }
  virtual bool GetClearScreen () const
  {
    return false;
  }
  virtual void DisableDistanceClipping ()
  {
    celPcCameraCommon::DisableDistanceClipping ();
  }
  virtual void EnableFixedDistanceClipping (float dist)
  {
    celPcCameraCommon::EnableFixedDistanceClipping (dist);
  }
  virtual void EnableAdaptiveDistanceClipping (float min_fps,
        float max_fps, float min_dist)
  {
    celPcCameraCommon::EnableAdaptiveDistanceClipping (min_fps,
	max_fps, min_dist);
  }
  virtual bool UseDistanceClipping () const
  {
    return celPcCameraCommon::UseDistanceClipping ();
  }
  virtual bool UseFixedDistanceClipping () const
  {
    return celPcCameraCommon::UseFixedDistanceClipping ();
  }
  virtual float GetFixedDistance () const
  {
    return celPcCameraCommon::GetFixedDistance ();
  }
  virtual float GetAdaptiveMinFPS () const
  {
    return celPcCameraCommon::GetAdaptiveMinFPS ();
  }
  virtual float GetAdaptiveMaxFPS () const
  {
    return celPcCameraCommon::GetAdaptiveMaxFPS ();
  }
  virtual float GetAdaptiveMinDistance () const
  {
    return celPcCameraCommon::GetAdaptiveMinDistance ();
  }
  virtual void SetAutoDraw (bool auto_draw)
  {
    celPcCameraCommon::SetAutoDraw (auto_draw);
  }
};

#endif // __CEL_PF_SIMPLE_CAMERA_FACTORY__

