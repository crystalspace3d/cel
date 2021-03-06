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
#include <math.h>
#include "propclass/mesh.h"
#include "propclass/solid.h"
#include "propclass/zone.h"
#include "plugins/propclass/newcamera/modes/firstperson.h"
#include "propclass/newcamera.h"

namespace celCameraMode
{

FirstPerson::FirstPerson ()
	: scfImplementationType (this)
{
}

bool FirstPerson::UseSpringPos () const
{
  return false;
}

bool FirstPerson::UseSpringOrigin () const
{
  return false;
}

bool FirstPerson::UseSpringTarget () const
{
  return false;
}

bool FirstPerson::UseSpringUp () const
{
  return false;
}

bool FirstPerson::DrawAttachedMesh () const
{
  return false;
}

FirstPerson::~FirstPerson ()
{
}

}
