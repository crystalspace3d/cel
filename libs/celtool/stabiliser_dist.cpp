/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein

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

#include <cssysdef.h>

#include "tools/stabiliser_dist.h"

float celDefaultHoverUpthruster::Force (celHoverObjectInfo obj_info)
{
  float force = 0.0f;

  /* at smaller heights we provide constant
      upthrust, because dividing by small numbers
      gives extremely huge force up */
  if (obj_info.height < 0.1f)
    force = 30.0f;
  else if (obj_info.height < 2.0f)
    force = 40.0f / std::pow (obj_info.height, 0.1447533f);
  else if (obj_info.height < 6.0f)
    force = 16.0f;

  /* above certain upward speeds
      we don't want to push object up
      anymore */
  if (obj_info.yvel > 3.0f)
    force *= 0.0f;
  /* dampen upward speed when already
      travelling up */
  else if(obj_info.yvel > 0.5f)
    force *= 0.25f;
  /* encourage upward speed when travelling
      downwards */
  else if (obj_info.yvel < -0.1f)
    force *= 1.2f;
  /* linear equation so object doesn't
      bounce harshly when falling */
  else if (obj_info.yvel < -16.0f)
    force *= -obj_info.yvel - 15.0f;

  //printf("Fr: %f\th: %f\tVy: %f\n",force,obj_info.height,obj_info.yvel);
  return force / 1.4;  // adjust this divisor to adjust hover height and dampening
}
