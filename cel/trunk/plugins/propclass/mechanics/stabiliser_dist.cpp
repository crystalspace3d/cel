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

#include <cmath>
#include <math.h>
#include <stdio.h>
#include <propclass/mechsys.h>
#include <csgeom/vector3.h>

#ifndef M_E
    #define M_E 2.71828182845904523536
#endif

void celIntervalMetaDistribution::Add(csRef<celStabiliserFunction> func , float low_int , float high_int)
{
  Interval i;
  i.low = low_int;
  i.high = high_int;
  i.func = func;

  funcs.Push(i);
}

float celIntervalMetaDistribution::Force(float h)
{
  for(csArray<Interval>::Iterator it = funcs.GetIterator() ; it.HasNext() ; ) {
    Interval i = it.Next();

    if( ((i.low < h) && (i.high > h)) || (i.low == h) || (i.high == h) )
      return i.func->Force(h);
  }

  return 0;
}


celIfFallingDistribution::celIfFallingDistribution(csRef<iPcMechanicsObject> ship,
  csRef<celStabiliserFunction> ifdist , csRef<celStabiliserFunction> elsedist , float adelta)
{
  ship_mech = ship;
  if_dist = ifdist;
  else_dist = elsedist;
  delta = adelta;
}

float celIfFallingDistribution::Force(float h)
{
  //printf("fall: %f\t",ship_mech->WorldToLocal(ship_mech->GetLinearVelocity()).y);
  if(ship_mech->WorldToLocal(ship_mech->GetLinearVelocity()).y < delta)
    return if_dist->Force(h);
  return else_dist->Force(h);
}

celFallingMultiplierDistribution::celFallingMultiplierDistribution(csRef<iPcMechanicsObject> ship , csRef<celStabiliserFunction> adist)
{
  dist = adist;
  ship_mech = ship;
}

float celFallingMultiplierDistribution::Force(float h)
{
  return ship_mech->WorldToLocal(ship_mech->GetLinearVelocity()).y * dist->Force(h);
}

celReturnConstantValue::celReturnConstantValue(float avalue)
{
  value = avalue;
}

float celReturnConstantValue::Force(float h)
{
  return value;
}


celInversePowerDistribution::celInversePowerDistribution(float h0 , float f0 , float h1 , float f1)
{
  k = ComputeK(h0, f0, h1, f1);
  x = ComputeX(h0, f0, k);  // h0 == 1 will fail
}
float celInversePowerDistribution::Force(float h)
{
  return Formula(h);
}

float celInversePowerDistribution::ComputeK(float h0 , float f0 , float h1 , float f1)
{
  return std::pow(float(M_E), (( std::log(h1)*std::log(f0) - std::log(h0)*std::log(f1) ) /
          ( std::log(h1) - std::log(h0) )) );
}
float celInversePowerDistribution::ComputeX(float h , float f , float k)
{
  return std::log(k / f) / std::log(h);
}

float celInversePowerDistribution::Formula(float h)
{
  return k / std::pow(h , x);
}


celSquareDistribution::celSquareDistribution(float h0 , float f0 , float h1 , float f1)
{
  k = (f1 - f0) / (h1*h1 - h0*h0);
  a = f1 - k*h1*h1;
}
float celSquareDistribution::Force(float h)
{
  return a + k*h*h;
}
