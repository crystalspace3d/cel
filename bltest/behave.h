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

#ifndef __CEL_BLTEST_BEHAVE__
#define __CEL_BLTEST_BEHAVE__

#include "cstypes.h"
#include "csutil/scf.h"
#include "bl/behave.h"

/**
 * 'printer' behaviour.
 */
struct celBehaviourPrinter : public iCelBehaviour
{
private:

public:
  celBehaviourPrinter ();
  virtual ~celBehaviourPrinter ();

  SCF_DECLARE_IBASE;

  virtual bool SendMessage (const char* msg_id, iBase* msg_info, ...);
  virtual bool SendMessageV (const char* msg_id, iBase* msg_info, va_list arg);
};

/**
 * 'room' behaviour.
 */
struct celBehaviourRoom : public iCelBehaviour
{
private:

public:
  celBehaviourRoom ();
  virtual ~celBehaviourRoom ();

  SCF_DECLARE_IBASE;

  virtual bool SendMessage (const char* msg_id, iBase* msg_info, ...);
  virtual bool SendMessageV (const char* msg_id, iBase* msg_info, va_list arg);
};

#endif // __CEL_BLTEST_BEHAVE__

