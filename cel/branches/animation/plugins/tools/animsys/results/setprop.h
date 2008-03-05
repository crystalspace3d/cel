#ifndef __CEL_ANIMATION_SETPROPERTY_RESULT_H
#define __CEL_ANIMATION_SETPROPERTY_RESULT_H

#include "csutil/scf_implementation.h"
#include "csutil/weakref.h"
#include "propclass/prop.h"
#include "tools/animsys.h"

namespace CEL
{
namespace Animation
{

class SetPropertyResult : public scfImplementation1<SetPropertyResult, iResult>
{
public:
  SetPropertyResult ();
  bool Initialise (iObjectRegistry *objreg, iCelEntity *ent, iPcAnimation *pcanim);
  bool SetParameter (const char* name, const celData &param);
  bool Execute ();
private:
  csString propname;
  celData propvalue;
  size_t propidx;
  csWeakRef<iPcProperties> pcprop;
};

class SetPropertyResultFactory : public scfImplementation1<SetPropertyResultFactory, iResultFactory>
{
public:
  SetPropertyResultFactory () : scfImplementationType (this) {}
  csPtr<iResult> CreateNode () const
  {
    return csPtr<iResult> (new SetPropertyResult);
  }
  const char* GetTypeName () const
  {
    return "setproperty";
  }
};

}
}

#endif
