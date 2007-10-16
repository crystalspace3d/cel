/*
    Crystal Space Entity Layer
    Copyright (C) 2007 by Dariusz Dawidowski

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
#include "csgeom/transfrm.h"
#include "cstool/csview.h"
#include "iengine/camera.h"
#include "propclass/mesh.h"
#include "propclass/solid.h"
#include "propclass/zone.h"
#include "plugins/propclass/newcamera/modes/isometric.h"
#include "propclass/newcamera.h"

namespace celCameraMode
{

Isometric::Isometric ()
	: scfImplementationType (this)
{
  up = csVector3 (0.0f, 1.0f, 0.0f);
}

Isometric::~Isometric ()
{
}

const csVector3& Isometric::GetPosition () const
{
  return GetOrigin ();
}

const csVector3& Isometric::GetOrigin () const
{
  return origin;
}

const csVector3& Isometric::GetTarget () const
{
  return target;
}

const csVector3& Isometric::GetUp () const
{
  return up;
}

bool Isometric::UseSpringPos () const
{
  return true;
}

bool Isometric::UseSpringOrigin () const
{
  return true;
}

bool Isometric::UseSpringTarget () const
{
  return true;
}

bool Isometric::UseSpringUp () const
{
  return false;
}

bool Isometric::DrawAttachedMesh () const
{
  return true;
}

bool Isometric::DecideCameraState ()
{
  if (!parent)
    return false;
/*
  iView* view = parent->GetView ();
  if (view)
  {
    iCamera* cam = view->GetCamera ();
    if (cam)
      cam->SetFOVAngle (10.0f, 800);
  }
*/
  origin = parent->GetBaseOrigin () + csVector3 (0.0f, 10.0f, 10.0f);
  target = parent->GetBaseOrigin ();
  originSpring = parent->GetOriginSpringCoefficient ();
  targetSpring = parent->GetTargetSpringCoefficient ();
  return true;
}

}
