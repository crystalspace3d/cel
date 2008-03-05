#include "cssysdef.h"
#include "actions.h"
#include "imesh/nskeleton.h"
#include "imesh/skelanim.h"
#include "iutil/objreg.h"
#include "iutil/virtclk.h"
#include "ivaria/reporter.h"

namespace CEL
{
namespace Animation
{

ActionsNode::ActionsNode ()
  : scfImplementationType (this), lasttime (0), mixtime (125.0f), currdur (0.0f)
{
}
bool ActionsNode::Initialise (iObjectRegistry *objreg, iCelEntity *ent, csRef<Skeleton::iSkeleton> skel)
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
  accum = animlay->CreateAccumulateNode ();
  for (size_t i = 0; i < children.GetSize (); i++)
  {
    iNode* c = children.Get (i);
    accum->AddNode (0.0f, c->GetMixingNode ());
  }
  return true;
}
void ActionsNode::AddChild (csRef<iNode> c)
{
  children.Push (c);
}
void ActionsNode::AttachCondition (csRef<iCondition> cond)
{
  conditions.Push (cond);
}
Skeleton::Animation::iMixingNode* ActionsNode::GetMixingNode ()
{
  return accum;
}
bool ActionsNode::SetParameter (const char* name, const celData &param)
{
  if (!strcmp (name, "active") && param.type == CEL_DATA_STRING)
  {
    activate.Push (param.value.s->GetData ());
  }
  else if (!strcmp (name, "inactive") && param.type == CEL_DATA_STRING)
  {
    deactivate.Push (param.value.s->GetData ());
  }
  else if (!strcmp (name, "mixtime") && param.type == CEL_DATA_FLOAT)
  {
    mixtime = param.value.f;
  }
  // blend
  else if (!strcmp (name, "state") && param.type == CEL_DATA_STRING)
  {
    rampon.Push (param.value.s->GetData ());
  }
  else
    return false;
  return true;
}
void ActionsNode::Update ()
{
  while (!activate.IsEmpty ())
    SetNodeActivation (activate.Pop (), true);
  while (!deactivate.IsEmpty ())
    SetNodeActivation (deactivate.Pop (), false);
  while (!rampon.IsEmpty ())
  {
    for (size_t i = 0; i < children.GetSize (); i++)
    {
      iNode* c = children.Get (i);
      const char* nodename = c->GetName ();
      if (nodename && !strcmp (nodename, rampon.Pop ()))
      {
        for (size_t mi = 0; mi < mix_steps.GetSize (); i++)
        {
          const csTuple2<size_t, float> &mixv = mix_steps.Get (i);
          // already doing something! fuckoff!
          if (mixv.first == i)
            mix_steps.DeleteIndexFast (i);  // we will re-add it...
        }
        // now add it to mix steps
        size_t nodeid = accum->FindNode (c->GetMixingNode ());
        if (nodeid != UINT_MAX)
        {
          float destweight = 1.0f;
          float step = (destweight - accum->GetWeight (nodeid)) / mixtime;
          mix_steps.Push (csTuple2<size_t, float> (i, step));
          active_children.Push (c);
        }
      }
    }
  }

  for (size_t i = 0; i < active_children.GetSize (); i++)
  {
    iNode* c = active_children.Get (i);
    c->Update ();
    if (!c->GetMixingNode ()->IsActive ())
    {
    }
  }
  /*for (csRefArray<iNode>::Iterator it = active_children.GetIterator (); it.HasNext (); )
  {
    iNode* c = it.Next ();
    c->Update ();
  }*/
  for (csRefArray<iCondition>::Iterator it = conditions.GetIterator (); it.HasNext (); )
  {
    iCondition* c = it.Next ();
    c->Evaluate ();
  }

  if (!vc)
    return;
  size_t timenow = vc->GetCurrentTicks (), delta = timenow - lasttime;
  lasttime = timenow;
  /*if (!in_cross)
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
      //blender->SetWeight (i, blender->GetWeight (i) + mix_steps.Get (i) * delta);
    }
  }
  else
  {
    for (size_t i = 0; i < blender->GetNodeCount (); i++)
    {*/
      /*if (i != actid)
        blender->SetWeight (i, 0.0f);
      else
        blender->SetWeight (i, 1.0f);*/
    /*}
    currdur = 0.0f;
    in_cross = false;
  }*/
}
void ActionsNode::SetName (const char* n)
{
  name = n;
}
const char* ActionsNode::GetName ()
{
  return name;
}
void ActionsNode::SetNodeActivation (const csString &nodename, bool active)
{
  if (!nodename)
    return;
  for (csRefArray<iNode>::Iterator it = children.GetIterator (); it.HasNext (); )
  {
    iNode* c = it.Next ();
    if (c->GetName () && !strcmp (c->GetName (), nodename))
    {
      if (active)
        active_children.PushSmart (c);
      else
        active_children.Delete (c);
      size_t ai = accum->FindNode (c->GetMixingNode ());
      if (ai != UINT_MAX)
        accum->SetWeight (ai, active ? 1.0f : 0.0f);
    }
  }
}

}
}
