/*
    Crystal Space Entity Layer
    Copyright (C) 2003 by Jorrit Tyberghein
  
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

#ifndef __CEL_BL_BLGEN__
#define __CEL_BL_BLGEN__

#include "cstypes.h"
#include "csutil/scf.h"

struct iDocumentNode;

/**
 * This is an interface that behaviour layers can optionally
 * implement. If they do implement this interface then it is possible
 * to dynamically add behaviour (scripts) to the behaviour layer by
 * using the API in this interface.
 */
struct iCelBlLayerGenerate : virtual public iBase
{
  SCF_INTERFACE (iCelBlLayerGenerate, 0, 0, 1);

  /**
   * Create a new named behaviour layer script from a document node.
   * Returns false on error (will use reporter to report the error).
   */
  virtual bool CreateBehaviourScriptFromDoc (const char* name,
  	iDocumentNode* node) = 0;

  /**
   * Create a new named behaviour layer script from a string.
   * Returns false on error (will use reporter to report the error).
   */
  virtual bool CreateBehaviourScriptFromString (const char* name,
  	const char* string) = 0;

  /**
   * Create a new named behaviour layer script from a file (VFS path).
   * Returns false on error (will use reporter to report the error).
   */
  virtual bool CreateBehaviourScriptFromFile (const char* name,
  	const char* filename) = 0;
};

#endif // __CEL_BL_BLGEN__

