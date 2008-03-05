#ifndef __CEL_ANIMATIONSYSTEM_PROPERTY_CONDITION_H
#define __CEL_ANIMATIONSYSTEM_PROPERTY_CONDITION_H

#include "csutil/scf_implementation.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "physicallayer/pl.h"
#include "propclass/prop.h"
#include "tools/animsys.h"

namespace CEL
{
namespace Animation
{

class PropertyCondition : public scfImplementation1<PropertyCondition, iCondition>
{
public:
  PropertyCondition ();
  bool Initialise (iObjectRegistry *objreg, iCelEntity* ent, iNode* owner);
  void AddChild (csRef<iCondition> c);
  void AttachResult (csRef<iResult> res);
  bool SetParameter (const char* name, const celData &param);
  bool Evaluate ();
private:
  csWeakRef<iCelPropertyClass> pc;
  csWeakRef<iPcProperties> pcprop;
  iCelEntity* entity;
  float min, max;
  csString propname, propclassname, matches;
  size_t propid;
  csRef<iCelPlLayer> pl;

  csRefArray<iResult> results;
  csRefArray<iCondition> children;
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
