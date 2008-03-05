#include "cssysdef.h"
#include "setprop.h"
#include "ivaria/reporter.h"
#include "physicallayer/propclas.h"

namespace CEL
{
namespace Animation
{

SetPropertyResult::SetPropertyResult () : scfImplementationType (this), propidx (UINT_MAX)
{
}
bool SetPropertyResult::Initialise (iObjectRegistry *objreg, iCelEntity *ent, iPcAnimation *pcanim)
{
  if (!pcprop)
  {
    pcprop = celQueryPropertyClassEntity<iPcProperties> (ent);
    if (!pcprop)
    {
      csReport (objreg, CS_REPORTER_SEVERITY_ERROR,
        "cel.animation.system",
        "Missing virtual clock!");
      return false;
    }
  }
  return true;
}
bool SetPropertyResult::SetParameter (const char* name, const celData &param)
{
  if (!strcmp (name, "name") && param.type == CEL_DATA_STRING)
  {
    propname = param.value.s->GetData ();
  }
  else if (!strcmp (name, "value"))
  {
    propvalue = param;
  }
  else
    return false;
  return true;
}
bool SetPropertyResult::Execute ()
{
  if (!pcprop)
    return false;
  if (propidx == UINT_MAX)
    propidx = pcprop->GetPropertyIndex (propname);
  switch (propvalue.type)
  {
    case CEL_DATA_STRING:
      pcprop->SetPropertyIndex (propidx, propvalue.value.s->GetData ());
      break;
    default:
      break;
  }
  return true;
}

}
}
