/*
    Crystal Space Entity Layer
    Copyright (C) 2006 by Jorrit Tyberghein
  
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

#ifndef __CEL_CELCONSOLE__
#define __CEL_CELCONSOLE__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/flags.h"
#include "csutil/stringarray.h"

struct iConsoleInput;
struct iConsoleOutput;

/**
 * A single command for the CEL console. Implement
 * this if you want to add commands to the CEL console.
 */
struct iCelConsoleCommand : public virtual iBase
{
  SCF_INTERFACE (iCelConsoleCommand, 0, 0, 1);

  /**
   * Get the command string.
   */
  virtual const char* GetCommand () = 0;

  /**
   * Get a short description of this command.
   */
  virtual const char* GetDescription () = 0;

  /**
   * Show a long usage help on the console.
   */
  virtual void Help () = 0;

  /**
   * Execute this command with all the arguments in
   * a tokenized form. Note that the command string itself will
   * be at position 0 in the 'args' array.
   */
  virtual void Execute (const csStringArray& args) = 0;
};

/**
 * Interface for the CEL console.
 */
struct iCelConsole : public virtual iBase
{
  SCF_INTERFACE (iCelConsole, 0, 0, 1);

  /**
   * Get the input console that is being used.
   */
  virtual iConsoleInput* GetInputConsole () = 0;

  /**
   * Get the ouput console that is being used.
   */
  virtual iConsoleOutput* GetOutputConsole () = 0;

  /**
   * Register a command.
   */
  virtual void RegisterCommand (iCelConsoleCommand* command) = 0;
};

#endif // __CEL_CELCONSOLE__

