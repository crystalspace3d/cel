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

#ifndef __CEL_PL_MESSAGE__
#define __CEL_PL_MESSAGE__

#include <stdarg.h>
#include "cstypes.h"
#include "csutil/scf.h"

SCF_VERSION (iCelMessage, 0, 0, 1);

/**
 * A message in the CEL framework.
 */
struct iCelMessage : public iBase
{
  /**
   * Get message string.
   */
  virtual const char* GetMessageString () const = 0;

  /**
   * Get number of message parameters.
   */
  virtual int GetMessageParameterCount () const = 0;

  /**
   * Get message parameter.
   */
  virtual const char* GetMessageParameter (int idx) const = 0;
};

#endif // __CEL_PL_MESSAGE__

