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

#ifndef STABILISER_DISTRIBUTIONS_H
#define STABILISER_DISTRIBUTIONS_H

#include <csutil/array.h>
#include <csutil/ref.h>
#include <csutil/refcount.h>
#include <ivaria/script.h>

struct iPcMechanicsObject;

/**
 * Contains info about a hovering object,
 * and is used to feed information to the
 * upthrust calculation functor.
 */
struct celHoverObjectInfo
{
  /**
   * The height of the object above the
   * ground in along the world's y axis.
   */
  float height;
  /**
   * The vertical velocity of the object
   * along the objects local y axis.
   */
  float yvel;
};

/**
 * A function object where the returned
 * force will be applied to an object upwards.
 * along world axis.
 */
class celStabiliserFunction : public csRefCount
{
public:
  virtual ~celStabiliserFunction () {};
  /**
   * The force upwards to apply.
   */
  virtual float Force (celHoverObjectInfo obj_info) = 0;
};

/**
 * This function will be used when
 * UseDefaultStabiliserFunction is called.
 *
 * See libs/celtool/stabiliser_dist.cpp for
 * the implementation.
 */
class celDefaultHoverUpthruster : public celStabiliserFunction
{
public:
  float Force (celHoverObjectInfo obj_info);
};

#endif
