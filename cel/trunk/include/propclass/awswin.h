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

#ifndef __CEL_PF_AWSWIN__
#define __CEL_PF_AWSWIN__

#include "cstypes.h"
#include "csutil/scf.h"

#include "iaws/aws.h"

/**
 * This is the AWS Window property class.
 * <p>
 * This property class can send out the following messages
 * to the behaviour (add prefix 'cel.parameter.' to get the ID for parameters):
 * <ul>
 * <li>pcawswin_<trigger>: a specific trigger from AWS has been fired.
 * </ul>
 * <p>
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * <ul>
 * <li>LoadDefinitionFile: parameters 'filename' (string).
 * <li>SelectDefaultSkin: parameters 'name' (string).
 * <li>CreateWindow: parameters 'name' (string).
 * <li>CreateSink: parameters 'name' (string).
 * <li>RegisterTrigger: parameters 'trigger' (string).
 * <li>Show: no parameters.
 * <li>Hide: no parameters.
 * <li>Raise: no parameters.
 * <li>Lower: no parameters.
 * </ul>
 */
struct iPcAwsWindow : public virtual iBase
{
  SCF_INTERFACE (iPcAwsWindow, 0, 0, 1);

  /**
   * Get AWS.
   */
  virtual iAws* GetAWS () = 0;

  /**
   * Create the window.
   */
  virtual iAwsWindow* CreateWindow (const char* name) = 0;

  /**
   * Create the sink.
   */
  virtual iAwsSink* CreateSink (const char* name) = 0;

  /**
   * Get the window.
   */
  virtual iAwsWindow* GetWindow () = 0;
};

#endif // __CEL_PF_AWSWIN__

