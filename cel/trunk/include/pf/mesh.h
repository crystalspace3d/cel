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

#ifndef __CEL_PF_MESH__
#define __CEL_PF_MESH__

#include "cstypes.h"
#include "csutil/scf.h"

struct iMeshWrapper;
struct iSector;
class csVector3;

SCF_VERSION (iPcMesh, 0, 0, 1);

/**
 * This is a property class holding the representation of a mesh.
 */
struct iPcMesh : public iBase
{
  /**
   * Set the mesh filename to use.
   */
  virtual void SetMesh (const char* factname, const char* filename) = 0;

  /**
   * Get the mesh.
   */
  virtual iMeshWrapper* GetMesh () const = 0;

  /**
   * Conveniance function to move the mesh to some sector and position
   * and also update lighting.
   */
  virtual void MoveMesh (iSector* sector, const csVector3& pos) = 0;
};

#endif // __CEL_PF_MESH__

