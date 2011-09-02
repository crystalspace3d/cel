/*
    Crystal Space Entity Layer
    Copyright (C) 2011 by Jorrit Tyberghein
  
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

#ifndef __CEL_DYNWORLDLOAD_H__

#include "csutil/scf.h"

struct iDocumentNode;
struct iPcDynamicWorld;

/**
 * Implement this interface if you want to make an extension for the
 * dynamic world loader addon. These plugins are loaded from within
 * a dynamic world definition file using the <extension> tag.
 */
struct iDynamicWorldLoaderExtension : public virtual iBase
{
  SCF_INTERFACE (iDynamicWorldLoaderExtension, 0, 0, 1);

  /**
   * Parse this node. Returns false (but no error!) if the extension
   * does not recognize this node.
   */
  virtual bool Parse (iDocumentNode* node, iPcDynamicWorld* dynworld) = 0;
};


#endif
