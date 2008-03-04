#include "cssysdef.h"
#include "transition.h"
#include "ivaria/reporter.h"

namespace CEL
{
namespace Animation
{

TransitionResult::TransitionResult () : scfImplementationType (this)
{
}
bool TransitionResult::Initialise (iObjectRegistry *objreg, iCelEntity *ent, iPcAnimation *pcanim)
{
  this->pcanim = pcanim;
  return true;
}
bool TransitionResult::SetParameter (const char* name, const celData &param)
{
  if (!strcmp (name, "node") && param.type == CEL_DATA_STRING)
    nodename = param.value.s->GetData ();
  else if (!strcmp (name, "state") && param.type == CEL_DATA_STRING)
    state = param.value.s->GetData ();
  else
    return false;
  return true;
}
bool TransitionResult::Execute ()
{
  if (!node)
  {
    if (!nodename)
      return false;
    node = pcanim->FindNodeByName (nodename);
    if (!node)
      return false;
  }
  celData statename;
  statename.Set (state);
  node->SetParameter ("state", statename);
  return true;
}

}
}
