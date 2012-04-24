/*
    Crystal Space Entity Layer
    Copyright (C) 2004-2006 by Jorrit Tyberghein
  
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

#ifndef __CEL_DECORATORS__
#define __CEL_DECORATORS__


//-------------------------------------------------------------------------
// Specific decorator implementations.
//-------------------------------------------------------------------------

/**
 * This interface is implemented by the decorator that limits the number
 * of times its child is executed
 **/
struct iExecutionLimitDecorator: public virtual iBase
{
  SCF_INTERFACE (iExecutionLimitDecorator, 0, 0, 1);

  /**
   * Set the number of times to execute child
   * \param limit is the value of the limit or a parameter (starts
   * with '$').
   */
  virtual void SetExecutionLimit (const char* limit) = 0;
};

/**
 * This interface is implemented by the decorator that executes
 * its child multiple times
 **/
struct iLoopDecorator: public virtual iBase
{
  SCF_INTERFACE (iLoopDecorator, 0, 0, 1);

  /**
   * Set the number of times to execute child
   * \param limit is the value of the limit or a parameter (starts
   * with '$').
   */
  virtual void SetLoopLimit (const char* limit) = 0;
};

#endif // __CEL_DECORATORS__