/*
    Copyright (C) 2004 by Jorrit Tyberghein

    This file is part of Ecksdee.

    Ecksdee is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Ecksdee is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Ecksdee; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __XD_SHIPUPTHRUSTER__
#define __XD_SHIPUPTHRUSTER__

// CEL Includes
#include "tools/stabiliser_dist.h"

/**
 *
 */
class xdShipUpthrusterFunction : public celStabiliserFunction
{
public:
  float Force(celHoverObjectInfo obj_info)
  {
    float force = 0.0;

    if (obj_info.height < 0.1)
      force = 30.0;
    else if (obj_info.height < 2)
      force = 19.8997 / std::pow(obj_info.height, float(0.1447533));
    else if (obj_info.height < 6)
      force = 16.0;

    //printf("Fr: %f\tVy: %f\n",force,obj_info.yvel);

    if (obj_info.yvel > 3)
      force *= 0.0;
    else if(obj_info.yvel > 0.5)
      force *= 0.25;
    else if (obj_info.yvel < -0.1)
      force *= 1.2;
    else if (obj_info.yvel < -16.0)
      force *= -obj_info.yvel - 15.0;

    return force;
  }
};

#endif
