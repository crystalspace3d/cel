/*
    Crystal Space Entity Layer
    Copyright (C) 2006 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_DAMAGE__
#define __CEL_PF_DAMAGE__

#include "cstypes.h"
#include "csutil/scf.h"

/**
 * This is a property class for damage emitting. Attach this property
 * class to entities that can cause damage to other entities. Causing
 * damage is basically a matter of sending a message to the affected
 * entities. The affecting entity will have to handle the damage in whatever
 * way is appropriate.
 *
 * This property class can send out the following messages
 * to the behaviour of affecting entities (add prefix 'cel.parameter.' to
 * get the ID for parameters):
 * - pcdamage_hurt: damage has been done. Parameters: amount (float: amount
 *   of damage), source (string: name of source entity), sector (string:
 *   name of source sector), position (vector3: origin of damage),
 *   type (string: type of damage, application dependend).
 *
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * - AreaDamage: 'radius' (float: default very large). In case of
 *   'normal' falloff this is actually ignored.
 * - BeamDamage: 'direction' (vector3), 'maxdist' (float: default very large).
 *   In case of 'normal' falloff the maxdist is ignored.
 * - SingleDamage: 'target' (string: name of target entity).
 *
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property:
 * - amount (float, read/write): amount of damage to do.
 * - type (string, read/write): application dependend type of damage to do.
 * - sector (string, read/write): optional sector where the damage
 *   originates. If not given this will be obtained from the optionally
 *   attached pcmesh.
 * - position (vector3, read/write): optional vector where the damage
 *   originates. If not given this will be obtained from the optionally
 *   attached pcmesh.
 * - falloff (string, read/write): one of 'constant', 'normal', 'linear'.
 */
struct iPcDamage : public virtual iBase
{
  SCF_INTERFACE (iPcDamage, 0, 0, 1);

  /// Set the amount of damage.
  virtual void SetDamage (float amount) = 0;
  /// Get the amount of damage.
  virtual float GetDamage () const = 0;

  /// Set the type of damage.
  virtual void SetDamageType (const char* type) = 0;
  /// Get the type of damage.
  virtual const char* GetDamageType () const = 0;

  /**
   * Set the damage falloff ('constant', 'normal', or 'linear').
   * 'normal' is default.
   */
  virtual void SetFallOff (const char* f) = 0;
  /// Get the damage falloff.
  virtual const char* GetFallOff () const = 0;

  /**
   * Set the source location for the damage. If this is not given
   * then this property class will try to find the attached
   * pcmesh.
   */
  virtual void SetDamageLocation (const char* sectorname,
      const csVector3& pos) = 0;
  /// Get the source sector for the damage (only if explicitly set).
  virtual const char* GetDamageSector () const = 0;
  /// Get the source position for the damage (only if explicitly set).
  virtual const csVector3& GetDamagePosition () const = 0;

  /// Do area damage.
  virtual void AreaDamage (float radius) = 0;

  /// Do beam damage.
  virtual void BeamDamage (const csVector3& direction, float maxdist) = 0;

  /// Do single damage.
  virtual void SingleDamage (const char* target) = 0;
};

#endif // __CEL_PF_DAMAGE__

