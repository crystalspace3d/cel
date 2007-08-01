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
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef __CEL_PF_CAMERA__
#define __CEL_PF_CAMERA__

#include "cstypes.h"
#include "csutil/scf.h"

struct iCamera;
struct iView;
struct iPcRegion;
struct iPcZoneManager;

SCF_VERSION (iPcCamera, 0, 0, 3);

/**
 * The representation of a camera.
 */
struct iPcCamera : public iBase
{
  /**
   * Set Camera to a Region
   * \param region is a pointer to the region to add the camera to.
   * \param point if true, set the camera to a given start position.
   * \param name is the name of the start position to move the camera to.
   */
  virtual bool SetRegion (iPcRegion* region, bool point = true,
      const char* name = 0) = 0;

  /**
   * Set Camera to a zone manager
   * \param zonemgr is a pointer to the zone manager to add the camera to.
   * \param point if true, set the camera to a given start position.
   * \param regionname is the name of the region to add the camera to.
   * \param name is the name of the start position to move the camera to.
   */
  virtual bool SetZoneManager (iPcZoneManager* zonemgr, bool point,
      const char* regionname, const char* name = 0) = 0;

  /**
   * Set Camera to a zone manager
   * \param entityname is a name of the entity with the zone manager.
   * \param point if true, set the camera to a given start position.
   * \param regionname is the name of the region to add the camera to.
   * \param name is the name of the start position to move the camera to.
   */
  virtual bool SetZoneManager (const char* entityname, bool point,
      const char* regionname, const char* name = 0) = 0;

  /**
   * Set the view rectangle to use on screen.
   * \param x is the distance from the left side of the screen to the left side
   *        of the view.
   * \param y is the distance from the top of the screen to the top of the
   *        view.
   * \param w is the width of the view.
   * \param h is the height of the view.
   */
  virtual void SetRectangle (int x, int y, int w, int h) = 0;

  /**
   * Get the camera.
   */
  virtual iCamera* GetCamera () const = 0;

  /**
   * Get the view.
   */
  virtual iView* GetView () const = 0;

  /**
   * Set flag indicating if the camera should clear z-buffer every frame.
   * Off by default unless the loaded map specifies this.
   * \param flag if set to true, the camera will clear the z-buffer every
   *        frame, otherwise it will not clear the z-buffer.
   */
  virtual void SetClearZBuffer (bool flag) = 0;

  /**
   * Get the clear zbuffer flag.
   */
  virtual bool GetClearZBuffer () const = 0;

  /**
   * Set flag indicating if the camera should clear screen every frame.
   * Off by default unless the loaded map specifies this.
   * \param flag if set to true, the camera will clear the screen every frame,
   *        otherwise it will not clear the screen.
   */
  virtual void SetClearScreen (bool flag) = 0;

  /**
   * Get the clear screen flag.
   */
  virtual bool GetClearScreen () const = 0;

  /**
   * Disable distance clipping.
   */
  virtual void DisableDistanceClipping () = 0;

  /**
   * Enable fixed distance clipping.
   * In this mode there is a fixed plane at the specified distance.
   * All geometry that is fully behind that plane will not be rendered.
   * This can speed up rendering.
   * \param dist is the distance from the camera to the "far" clipping plane.
   */
  virtual void EnableFixedDistanceClipping (float dist) = 0;

  /**
   * Enable adaptive distance clipping. In this mode the clipping plane
   * will vary depending on the desired minimum and maximum fps.
   * \param min_fps is the minimum fps that we would like to have. If we go
   *        beyond that then we will move the clipping plane closer.
   * \param max_fps is the maximum fps that is ok. If we go beyond that we
   *        will move the clipping plane further away.
   * \param min_dist is the minimum clipping plane distance. We will never
   *        move the clipping closer to the camera then this distance.
   */
  virtual void EnableAdaptiveDistanceClipping (float min_fps,
	float max_fps, float min_dist) = 0;

  /**
   * Returns true if we use distance clipping (either fixed or adaptive).
   */
  virtual bool UseDistanceClipping () const = 0;

  /**
   * Returns true if we use fixed distance clipping (as opposed to adaptive).
   */
  virtual bool UseFixedDistanceClipping () const = 0;

  /**
   * Get the fixed distance (returns < 0 if adaptive is used).
   */
  virtual float GetFixedDistance () const = 0;

  /**
   * Get the minimum fps used for adaptive distance clipping.
   */
  virtual float GetAdaptiveMinFPS () const = 0;

  /**
   * Get the maximum fps used for adaptive distance clipping.
   */
  virtual float GetAdaptiveMaxFPS () const = 0;

  /**
   * Get the minimum distance used for adaptive distance clipping.
   */
  virtual float GetAdaptiveMinDistance () const = 0;

  /**
   * Set the auto-draw option. If this option is enabled, the camera will
   * automatically render.
   * \param auto_draw will cause the camera to automatically render every frame
   *        if set to true, or do nothing if set to false.
   */
  virtual void SetAutoDraw (bool auto_draw) = 0;

  /**
   * Render. This will clear the screen then draw on top of it.
   */
  virtual void Draw () = 0;

  /**
   * Update camera position. This method will update camera position without 
   * rendering (you will have to call iView::Draw manual later).
   */
  virtual void UpdateCamera () = 0;

  /**
   * Get camera drawing flags.
   */
  virtual int GetDrawFlags () = 0;
};

#endif // __CEL_PF_CAMERA__

