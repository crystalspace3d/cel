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

#include "cssysdef.h"
#include "bltest/entity.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celBehaviour)
  SCF_IMPLEMENTS_INTERFACE (iCelBehaviour)
SCF_IMPLEMENT_IBASE_END

celBehaviour::celBehaviour ()
{
  SCF_CONSTRUCT_IBASE (NULL);
}

celBehaviour::~celBehaviour ()
{
}

bool celBehaviour::SendMessage (const char* msg_id, ...)
{
  va_list arg;
  va_start (arg, msg_id);
  bool rc = SendMessageV (msg_id, arg);
  va_end (arg);
  return rc;
}

bool celBehaviour::SendMessageV (const char* msg_id, va_list arg)
{
  (void)arg;
  printf ("Got message '%s'\n", msg_id);
  return false;
}

