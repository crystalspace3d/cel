#include "cssysdef.h"
#include "playanim.h"
#include "ivaria/reporter.h"

namespace CEL
{
namespace Animation
{

PlayAnimationResult::PlayAnimationResult () : scfImplementationType (this), playcount (1), playspeed (1.0f), reset (false)
{
}
bool PlayAnimationResult::Initialise (iObjectRegistry *objreg, iCelEntity *ent, iPcAnimation *pcanim)
{
  this->pcanim = pcanim;
  return true;
}
bool PlayAnimationResult::SetParameter (const char* name, const celData &param)
{
  if (!strcmp (name, "name") && param.type == CEL_DATA_STRING)
    nodename = param.value.s->GetData ();
  else if (!strcmp (name, "loop") && param.type == CEL_DATA_BOOL)
  {
    if (param.value.bo == true)
      playcount = -1;
  }
  else if (!strcmp (name, "playcount") && param.type == CEL_DATA_LONG)
    playcount = param.value.l;
  else if (!strcmp (name, "playspeed") && param.type == CEL_DATA_FLOAT)
    playspeed = param.value.f;
  else if (!strcmp (name, "reset") && param.type == CEL_DATA_BOOL)
  {
    reset = param.value.bo;
  }
  else
    return false;
  return true;
}
bool PlayAnimationResult::Execute ()
{
  if (!node)
  {
    if (!nodename)
      return false;
    node = pcanim->FindNodeByName (nodename);
    if (!node)
      return false;
  }
  /*csRef<Skeleton::Animation::iAnimation> anim = scfQueryInterface<Skeleton::Animation::iAnimation> (node->GetMixingNode ());
  if (!anim)
    return false;*/
  // ignored
  celData param;
  param.type = CEL_DATA_LONG;
  param.value.l = playcount;
  node->SetParameter ("playcount", param);
  param.type = CEL_DATA_FLOAT;
  param.value.f = playspeed;
  node->SetParameter ("playspeed", param);
  // only bother to send if it's true
  param.type = CEL_DATA_BOOL;
  param.value.bo = true;
  if (reset)
    node->SetParameter ("reset", param);
  return true;
}

}
}
