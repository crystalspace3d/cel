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

#include "cssysdef.h"
#include "csutil/util.h"
#include "plugins/stdphyslayer/message.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celMessage)
  SCF_IMPLEMENTS_INTERFACE (iCelMessage)
SCF_IMPLEMENT_IBASE_END

celMessage::celMessage (const char* msg_string, va_list /*arg*/)
{
  SCF_CONSTRUCT_IBASE (0);
  celMessage::msg_string = csStrNew (msg_string);
  num_parms = 0;
  parm_strings = 0;
}

celMessage::~celMessage ()
{
  delete[] msg_string;
  int i;
  for (i = 0 ; i < num_parms ; i++)
    delete[] parm_strings[i];
  delete[] parm_strings;
}

const char* celMessage::GetMessageParameter (int idx) const
{
  CS_ASSERT (idx >= 0 && idx < num_parms);
  return parm_strings[idx];
}

