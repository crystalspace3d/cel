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

#ifndef __CEL_PF_MESHDEFORM__
#define __CEL_PF_MESHDEFORM__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/weakref.h"

/**
 * This is a mesh deformation property class.
 */

struct iMeshWrapper;

struct iPcMeshDeform : public virtual iBase
{
  SCF_INTERFACE (iPcMeshDeform, 0, 0, 1);

  /**
   * Set the mesh to deform
   */
  virtual void SetMesh (iMeshWrapper* mesh) = 0;

  /**
   * Get the mesh to deform
   */
  virtual iMeshWrapper* GetMesh () = 0;

  /**
   * Deform the mesh
   */
  virtual void DeformMesh
  (const csVector3& position, const csVector3& direction, float radius,
    bool worldspace = false) = 0;

  /**
   * Change the deformation factor. Higher gives a larger deformation.
   */
  virtual void SetDeformFactor(float deformfactor) = 0;

  /**
   * Change the deformation noise factor, ie the amount of randomness of each
   * deform operation. Default is 0.2
   */
  virtual void SetNoise(float deformnoise) = 0;

  /**
   * Set the maximum number of deforms allowed per second. Defaults to 10.
   * Lower settings may improve performance.
   */
  virtual void SetMaxFrequency(float frequency) = 0;

  /**
   * Set the deformation allowed per vertice. Default is 0.5
   */
  virtual void SetMaxDeform(float maxdeform) = 0;

  /**
   * Get the deformation factor.
   */
  virtual float GetDeformFactor() = 0;

  /**
   * Get the random noise on deformation
   */
  virtual float GetNoise() = 0;

  /**
   * Get the maximum frequency of deformations
   */
  virtual float GetMaxFrequency() = 0;

  /**
   * Get the random noise on deformation
   */
  virtual float GetMaxDeform() = 0;
};

#endif // __CEL_PF_MESHDEFORM__

