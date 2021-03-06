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

#ifndef __CEL_ENTITYLOADER__
#define __CEL_ENTITYLOADER__

#include "cstypes.h"
#include "csutil/scf.h"

struct iDocumentNode;
struct iMeshWrapper;
struct iCelEntity;

/**
 * This interface allows one to access the celentity addon
 * with an easier interface. With this addon you can load
 * entities with assocated behaviour and property classes
 * from an XML description file.
 */
struct iEntityLoader : virtual public iBase
{
  SCF_INTERFACE (iEntityLoader, 0, 0, 1);

  /**
   * Load the entity from the document node.
   * If the optional mesh is given then a pcmesh is automatically
   * created.
   * Returns entity or 0 on failure. In case of failure the
   * error has been reported to the reporter.
   */
  virtual iCelEntity* Load (iDocumentNode* node,
		  iMeshWrapper* mesh = 0) = 0;

  /**
   * Conveniance function to load the entity from a vfs file.
   * \param path optional VFS path. Can be 0.
   */
  virtual iCelEntity* Load (const char* path, const char* file,
      iMeshWrapper* mesh = 0) = 0;
};

#endif // __CEL_ENTITYLOADER__

