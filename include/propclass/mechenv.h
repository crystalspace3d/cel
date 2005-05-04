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

SCF_VERSION (iPcEnvAttractor, 0, 0, 1);

/**
 * A property class representing an attractor or a repulsor.
 * <p>
 * This property class acts on nearby entities with an iPcDynamicBody.
 * <p>
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property):
 * <ul>
 * <li>strength (float, read/write): the strength of the field at its center.
 * <li>radius (float, read/write): the maximum radius at which the field affects
 * an object.
 * </ul>
 */
struct iPcEnvAttractor : public iBase
{
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


SCF_VERSION (celEnvFluid, 0, 0, 1);

/**
 * A class implementing a fluid in a sector.
 */
class celEnvFluid : public csObject
{
public:
  /**
   * The density of the fluid in this sector.
   */
  float density;
  /**
   * The velocity of fluid flow in this sector.
   */
  csVector3 flow;

  SCF_DECLARE_IBASE_EXT (csObject);
};

SCF_IMPLEMENT_IBASE_EXT (celEnvFluid)
  SCF_IMPLEMENTS_INTERFACE (celEnvFluid)
SCF_IMPLEMENT_IBASE_EXT_END


SCF_VERSION (celEnvGravity, 0, 0, 1);

/**
 * A class implementing gravity in a sector.
 */
class celEnvGravity : public csObject
{
public:
  /**
   * The acceleration due to gravity in this sector.
   */
  csVector3 gravity;

  SCF_DECLARE_IBASE_EXT (csObject);
};

SCF_IMPLEMENT_IBASE_EXT (celEnvGravity)
  SCF_IMPLEMENTS_INTERFACE (celEnvGravity)
SCF_IMPLEMENT_IBASE_EXT_END

#endif // __CEL_PF_MECHANICS_ATTRACTOR__

