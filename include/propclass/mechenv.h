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

#ifndef __CEL_PF_MECHANICS_ATTRACTOR__
#define __CEL_PF_MECHANICS_ATTRACTOR__

//////
// Environmental objects.
//////

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/csobject.h"

struct iPcSolid;
class csVector3;

/**
 * A property class representing an attractor or a repulsor.
 *
 * This property class acts on nearby entities with an iPcDynamicBody.
 *
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property):
 * - strength (float, read/write): the strength of the field at its center.
 * - radius (float, read/write): the maximum radius at which the field affects
 *   an object.
 */
struct iPcEnvAttractor : public virtual iBase
{
  SCF_INTERFACE (iPcEnvAttractor, 0, 0, 1);

  /**
   * Set the magnitude of the force at the center of the attractor. (negative
   * creates a repulsor)
   */
  virtual void SetStrengthAtCenter (float strength) = 0;
  /**
   * Get magnitude of the force at the center of the attractor.
   */
  virtual float GetStrengthAtCenter () = 0;

  /**
   * Set the maximum radius of the attractor's field.
   */
  virtual void SetMaxRadius (float radius) = 0;
  /**
   * Get the maximum radius of the attractor's field.
   */
  virtual float GetMaxRadius () = 0;
};


/**
 * A class implementing a fluid in a sector.
 */
class celEnvFluid : public scfImplementationExt1<
        celEnvFluid, csObject, scfFakeInterface<celEnvFluid> >
{
public:
  SCF_INTERFACE (celEnvFluid, 0, 0, 1);

  celEnvFluid () : scfImplementationType (this) { }
  virtual ~celEnvFluid () { }

  /**
   * The density of the fluid in this sector.
   */
  float density;
  /**
   * The velocity of fluid flow in this sector.
   */
  csVector3 flow;
};

/**
 * A class implementing gravity in a sector.
 */
class celEnvGravity : public scfImplementationExt1<
        celEnvGravity, csObject, scfFakeInterface<celEnvGravity> >
{
public:
  SCF_INTERFACE (celEnvGravity, 0, 0, 1);

  celEnvGravity () : scfImplementationType (this) { }
  virtual ~celEnvGravity () { }

  /**
   * The acceleration due to gravity in this sector.
   */
  csVector3 gravity;
};

#endif // __CEL_PF_MECHANICS_ATTRACTOR__

