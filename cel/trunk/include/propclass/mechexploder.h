#ifndef __CEL_PF_MECHANICS_EXPLODER__
#define __CEL_PF_MECHANICS_EXPLODER__

#include "cstypes.h"
#include "csutil/scf.h"

SCF_VERSION (iPcMechanicsExploder, 0, 0, 1);

/**
 * A property class for handling the physics of an explosion.
 * <p>
 * This property class depends on iPcMechanicsObject and iPcAttractor, and
 * extends their functionality.
 * <p>
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * <ul>
 * <li>Explode: no parameters.
 * </ul>
 * <p>
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property):
 * <ul>
 * <li>force (float, read/write): the force of the explosion.
 * </ul>
 */
struct iPcMechanicsExploder : public iBase
{
  /**
   * Set the magnitude of the force of the explosion this object will create.
   */
  virtual void SetExplosiveForce (float force) = 0;

  /**
   * Blow up the object!
   */
  virtual void Explode () = 0;
};

#endif // __CEL_PF_MECHANICS_EXPLODER__

