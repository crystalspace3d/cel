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

#ifndef __CEL_ENTITYTPLLOADER__
#define __CEL_ENTITYTPLLOADER__

#include "cstypes.h"
#include "csutil/scf.h"

struct iDocumentNode;
struct iMeshWrapper;
struct iCelEntityTemplate;
struct iLoaderContext;

/**
 * This interface allows one to access the celentitytpl addon
 * with an easier interface. With this addon you can load
 * entity templates with assocated behaviour and property classes
 * from an XML description file.
 */
struct iEntityTemplateLoader : virtual public iBase
{
  SCF_INTERFACE (iEntityTemplateLoader, 0, 0, 1);

  /**
   * Load the entity template from the document node.
   * Returns entity template or 0 on failure. In case of failure the
   * error has been reported to the reporter.
   */
  virtual iCelEntityTemplate* Load (iDocumentNode* node, iLoaderContext* context) = 0;

  /**
   * Conveniance function to load the entity template from a vfs file.
   * \param path optional VFS path. Can be 0.
   */
  virtual iCelEntityTemplate* Load (const char* path, const char* file) = 0;

  /**
   * Save an entity template to a document node.
   * Returns false on failure. In case of failure the error has been
   * reported to the reporter.
   */
  virtual bool Save (iCelEntityTemplate* tpl, iDocumentNode* node) = 0;

  /**
   * Load a property class template from a document node.
   * Returns 0 on failure. In case of failure the
   * error has been reported to the reporter.
   */
  virtual iCelPropertyClassTemplate* Load (iCelEntityTemplate* tpl, iDocumentNode* node,
      iLoaderContext* context) = 0;

  /**
   * Save an property class template to the given node.
   * Returns false on failure. In case of failure the error has been
   * reported to the reporter.
   */
  virtual bool Save (iCelPropertyClassTemplate* pctpl, iDocumentNode* node) = 0;
};

#endif // __CEL_ENTITYTPLLOADER__

