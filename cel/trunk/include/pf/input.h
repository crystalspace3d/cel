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

#ifndef __CEL_PF_INPUT__
#define __CEL_PF_INPUT__

#include "cstypes.h"
#include "csutil/scf.h"

SCF_DECLARE_FAST_INTERFACE (iPcCommandInputData)
SCF_DECLARE_FAST_INTERFACE (iPcCommandInput)

SCF_VERSION (iPcCommandInput, 0, 0, 1);

/**
 * Timer propery class.
 */
struct iPcCommandInput : public iBase
{
  /**
   * Activates the input to get Commands
   */
  virtual void Activate (bool activate=true) = 0;

  /**
   * Loads a Config File binding triggers(for example keys) to commands
   */
  virtual bool LoadConfig (const char* fname) = 0;

  /**
   * Binds a trigger to a command
   */
  virtual bool Bind (const char* triggername, const char* command) = 0;

  /** returns the command bind to a key */
  virtual const char* GetBind (const char* triggername) const = 0;

  /** deletes a bind, if triggername is 0 deletes all binds to the command */
  virtual bool RemoveBind (const char* triggername, const char* command) = 0;
};

#endif // __CEL_PF_TIMER__

