#include "cssysdef.h"
#include "switcher.h"
#include "imesh/nskeleton.h"
#include "imesh/skelanim.h"
#include "iutil/objreg.h"
#include "iutil/virtclk.h"
#include "ivaria/reporter.h"

namespace CEL
{
namespace Animation
{

SwitcherNode::SwitcherNode ()
  : scfImplementationType (this), actid (UINT_MAX), deactid (UINT_MAX), lasttime (0),
  crosstime (125.0f), currdur (0.0f), in_crossing (false)
{
}
bool SwitcherNode::Initialise (iObjectRegistry *objreg, iCelEntity *ent, csRef<Skeleton::iSkeleton> skel)
{
  vc = csQueryRegistry<iVirtualClock> (objreg);
  if (!vc)
  {
    csReport (objreg, CS_REPORTER_SEVERITY_ERROR,
      "cel.animation.system",
      "Missing virtual clock!");
    return false;
  }
  lasttime = vc->GetCurrentTicks ();
  csRef<Skeleton::Animation::iAnimationLayer> animlay = skel->GetAnimationLayer ();
  blender = animlay->CreateBlendNode ();
  for (csRefArray<iNode>::Iterator it = children.GetIterator (); it.HasNext (); )
  {
    iNode* c = it.Next ();
    blender->AddNode (0.0f, c->GetMixingNode ());
  }
  return true;
}
void SwitcherNode::AddChild (csRef<iNode> c)
{
  children.Push (c);
}
void SwitcherNode::AttachCondition (csRef<iCondition> cond)
{
  conditions.Push (cond);
}
Skeleton::Animation::iMixingNode* SwitcherNode::GetMixingNode ()
{
  return blender;
}
bool SwitcherNode::SetParameter (const char* name, const celData &param)
{
  if (!strcmp (name, "state") && param.type == CEL_DATA_STRING)
  {
    currstate = nextstate;
    nextstate = param.value.s->GetData ();
    if (currstate && !strcmp (currstate, nextstate))
      return true;
    in_crossing = true;
    currdur = 0.0f;
    setup_for_next = false;
  }
  else if (!strcmp (name, "crosstime") && param.type == CEL_DATA_STRING)
  {
    crosstime = param.value.f;
  }
  else
    return false;
  return true;
}
void SwitcherNode::Update ()
{
  if (active_child)
    active_child->Update ();
  for (csRefArray<iCondition>::Iterator it = conditions.GetIterator (); it.HasNext (); )
  {
    iCondition* c = it.Next ();
    c->Evaluate ();
  }
  if (!vc)
    return;
  size_t timenow = vc->GetCurrentTicks (), delta = timenow - lasttime;
  lasttime = timenow;
  if (!in_crossing)
    return;

  if (!setup_for_next)
  {
    setup_for_next = true;
    for (csRefArray<iNode>::Iterator it = children.GetIterator (); it.HasNext (); )
    {
      iNode* c = it.Next ();
      if (c->GetName () && !strcmp (c->GetName (), nextstate))
      {
        if (active_child)
        {
          deact_child = active_child;
          deactid = blender->FindNode (deact_child->GetMixingNode ());
        }
        active_child = c;
        actid = blender->FindNode (active_child->GetMixingNode ());
      }
    }
  }

  float perc = currdur / crosstime;
  if (perc > 1.0f)
    perc = 1.0f;
  if (deact_child)
  {
    blender->SetWeight (deactid, 1.0f - perc);
    blender->SetWeight (actid, perc);
  }
  else
  {
    blender->SetWeight (actid, 1.0f);
  }

  currdur += delta;
  if (currdur >= crosstime)
  {
    in_crossing = false;
    currdur = 0.0f;
    deact_child = 0;
  }
}
void SwitcherNode::SetName (const char* n)
{
  name = n;
}
const char* SwitcherNode::GetName ()
{
  return name;
}

}
}
