#ifndef __CEL_ANIMATIONSYSTEM_PROPERTY_CONDITION_H
#define __CEL_ANIMATIONSYSTEM_PROPERTY_CONDITION_H

#include "csutil/scf_implementation.h"
#include "csutil/weakref.h"
#include "propclass/prop.h"
#include "tools/animsys.h"

namespace CEL
{
namespace Animation
{

class PropertyCondition : public scfImplementation2<PropertyCondition, iCondition, iPcPropertyListener>
{
public:
  PropertyCondition ();
  bool Initialise (iObjectRegistry *objreg, iCelEntity* ent);
  bool SetParameter (const char* name, const celData &param);
  bool Evaluate ();
  // used by iPcPropertyListener to notify this class
  void PropertyChanged (iPcProperties *pcprop, size_t idx);
private:
  csWeakRef<iPcProperties> pcprop;
  iCelEntity* entity;
  float min, max;
  csString propname, matches;
  size_t propidx;
  bool property_changed;
};

class PropertyConditionFactory : public scfImplementation1<PropertyConditionFactory, iConditionFactory>
{
public:
  PropertyConditionFactory () : scfImplementationType (this) {}
  csPtr<iCondition> CreateNode () const
  {
    return csPtr<iCondition> (new PropertyCondition);
  }
  const char* GetTypeName () const
  {
    return "property";
  }
};

}
}

#endif
