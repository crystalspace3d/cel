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

#ifndef __CEL_PF_PORTAL__
#define __CEL_PF_PORTAL__

#include "cstypes.h"
#include "csutil/scf.h"

struct iPortal;

SCF_VERSION (iPcPortal, 0, 0, 1);

/**
 * This is a property class holding the representation of a portal.
 * This is mainly useful in cases where you want to close a portal for
 * performance reasons. Closing a portal can be useful in case you have
 * a door that closes off the portal and it doesn't make sense to use
 * the portal anyway.
 *
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property:
 * - mesh (string, read/write): the name of the portal mesh.
 * - portal (string, read/write): the name of the portal.
 * - closed (bool, read/write): if the portal is closed or not.
 */
struct iPcPortal : public iBase
{
  /**
   * Set the portal to use.
   * \param mesh is the name of the portal mesh.
   * \param portal is the optional name of the portal.
   */
  virtual bool SetPortal (const char* mesh, const char* portal = 0) = 0;
  
  /**
   * Get the portal.
   */
  virtual iPortal* GetPortal () = 0;

  /**
   * Close portal.
   */
  virtual void ClosePortal () = 0;

  /**
   * Open portal.
   */
  virtual void OpenPortal () = 0;

  /**
   * Is the portal closed?
   */
  virtual bool IsPortalClosed () const = 0;
};

#endif // __CEL_PF_PORTAL__

