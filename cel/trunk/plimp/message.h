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

#ifndef __CEL_PLIMP_MESSAGE__
#define __CEL_PLIMP_MESSAGE__

#include <stdarg.h>
#include "pl/message.h"

/**
 * A message in the CEL framework.
 */
class celMessage : public iCelMessage
{
private:
  char* msg_string;
  int num_parms;
  char** parm_strings;

public:
  celMessage (const char* msg_string, va_list arg);
  virtual ~celMessage ();

  SCF_DECLARE_IBASE;

  virtual const char* GetMessageString () const { return msg_string; }
  virtual int GetMessageParameterCount () const { return num_parms; }
  virtual const char* GetMessageParameter (int idx) const;
};

#endif // __CEL_PLIMP_MESSAGE__

