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
  : scfImplementationType (this), actid (UINT_MAX), cross_setup (true),
  in_cross (false), lasttime (0), crosstime (125.0f), currdur (0.0f)
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
    cross_setup = false;
    in_cross = true;
  }
  else if (!strcmp (name, "crosstime") && param.type == CEL_DATA_FLOAT)
  {
    crosstime = param.value.f;
  }
  else
    return false;
  return true;
}
void SwitcherNode::Update ()
{
  if (active_anim)
    active_anim->Update ();
  for (csRefArray<iCondition>::Iterator it = conditions.GetIterator (); it.HasNext (); )
  {
    iCondition* c = it.Next ();
    c->Evaluate ();
  }

  if (!vc)
    return;
  size_t timenow = vc->GetCurrentTicks (), delta = timenow - lasttime;
  lasttime = timenow;
  if (!in_cross)
    return;
  currdur += delta;

  if (!cross_setup)
  {
    if (!(currstate && !strcmp (currstate, nextstate)))
    {
      mix_steps.SetSize (children.GetSize ());
      for (size_t i = 0; i < mix_steps.GetSize (); i++)
      {
        iNode* c = children.Get (i);
        if (c->GetName () && !strcmp (c->GetName (), nextstate))
        {
          active_anim = c;
          if (currstate)
          {
            mix_steps.Get (i) = 1.0f / crosstime;
            currdur = 0.0f;
          }
          else
          {
            // the very first animation!!
            mix_steps.Get (i) = 1.0f;
            currdur = crosstime + 2.0f;  // cheat to finish ticking ;)
          }
          actid = i;
        }
        else
          mix_steps.Get (i) = -blender->GetWeight (i) / crosstime;
      }
      in_cross = true;
    }
    cross_setup = true;
  }

  if (currdur < crosstime)
  {
    for (size_t i = 0; i < mix_steps.GetSize (); i++)
    {
      blender->SetWeight (i, blender->GetWeight (i) + mix_steps.Get (i) * delta);
    }
  }
  else
  {
    for (size_t i = 0; i < blender->GetNodeCount (); i++)
    {
      if (i != actid)
        blender->SetWeight (i, 0.0f);
      else
        blender->SetWeight (i, 1.0f);
    }
    currdur = 0.0f;
    in_cross = false;
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
