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

#ifndef __CEL_PF_SIMPLE_CAMERA__
#define __CEL_PF_SIMPLE_CAMERA__

#include "propclass/camera.h"

struct iPcMesh;

/**
 * A class representing a simple camera, with capabilities for both
 * first-person and third-person modes.
 *
 * This property class supports the following actions (add prefix 'cel.camera.simple.action.'
 * if you want to access this action through a message):
 * - InitCamera: parameters 'campos' (vector3), 'lookat' (vector3),
 *   'drawmesh' (bool).
 * - SetPosition: parameters 'campos' (vector3), 'lookat' (vector3).
 * - SetMesh: parameters 'meshpctag' (string).
 */
struct iPcSimpleCamera : public virtual iPcCamera
{
  SCF_INTERFACE (iPcSimpleCamera, 0, 0, 3);

  /**
   * Set a flag to determine whether or not the attached mesh and its children
   * will be drawn in this camera view.
   * \param draw if true, the mesh will be drawn in this view.
   */
  virtual void SetDrawMesh (bool draw) = 0;

  /**
   * Sets the offset from the center of the mesh's iMovable to the position of
   * the camera.
   * \param offset the offset from the center of the mesh to the camera
   *        position.
   * \param world if true then the offset is in world space so the camera
   *        will always look from the same side.
   */
  virtual void SetCameraOffset (const csVector3& offset, bool world = false) = 0;

  /**
   * Sets the offset from the center of the mesh's iMovable to the point the
   * camera should look at.
   * \param lookat the offset from the center of the mesh to the lookat
   *        position.
   */
  virtual void SetLookAtOffset (const csVector3& lookat) = 0;

  /**
   * Set the mesh to follow.
   * \param mesh the mesh property class for the camera to follow.
   */
  virtual void SetMesh (iPcMesh* mesh) = 0;

  /**
   * Render. This will clear the screen then draw on top of it.
   */
  virtual void Draw() = 0;
};

#endif // __CEL_PF_SIMPLE_CAMERA__

