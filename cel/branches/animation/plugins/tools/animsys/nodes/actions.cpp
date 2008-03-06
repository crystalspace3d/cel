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
    const csString &rampname = rampon.Pop ();
    for (size_t i = 0; i < children.GetSize (); i++)
    {
      iNode* c = children.Get (i);
      const char* nodename = c->GetName ();
      if (nodename && !strcmp (nodename, rampname))
      {
        for (size_t mi = 0; mi < mix_steps.GetSize (); mi++)
        {
          const csTuple2<size_t, float> &mixv = mix_steps.Get (mi);
          // already doing something! fuckoff!
          if (mixv.first == i)
            mix_steps.DeleteIndexFast (mi);  // we will re-add it...
        }
        // now add it to mix steps
        size_t nodeid = accum->FindNodeIndex (c->GetMixingNode ());
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
      active_children.DeleteIndex (i);
    }
    float ttilfin = c->GetMixingNode ()->TimeUntilFinish ();
    if (ttilfin < mixtime)
    {
      // TODO abstract this
      size_t nodeid = accum->FindNodeIndex (c->GetMixingNode ());
      if (nodeid != UINT_MAX)
      {
        float destweight = 0.0f;
        float step = (destweight - accum->GetWeight (nodeid)) / ttilfin;
        //mix_steps.Push (csTuple2<size_t, float> (i, step));
        active_children.DeleteIndex (i);
      }
    }
  }
  for (csRefArray<iCondition>::Iterator it = conditions.GetIterator (); it.HasNext (); )
  {
    iCondition* c = it.Next ();
    c->Evaluate ();
  }

  if (!vc)
    return;
  size_t timenow = vc->GetCurrentTicks (), delta = timenow - lasttime;
  lasttime = timenow;

  for (size_t mi = 0; mi < mix_steps.GetSize (); mi++)
  {
    const csTuple2<size_t, float> &mixv = mix_steps.Get (mi);
    float nweight = accum->GetWeight (mixv.first) + mixv.second * delta;
    if (nweight > 1.0f)
    {
      nweight = 1.0f;
      active_children.Push (children.Get (mixv.first));
      mix_steps.DeleteIndexFast (mi);
    }
    else if (nweight < EPSILON)
    {
      mix_steps.DeleteIndexFast (mi);
    }
    accum->SetWeight (mixv.first, nweight);
  }
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
      size_t ai = accum->FindNodeIndex (c->GetMixingNode ());
      if (ai != UINT_MAX)
        accum->SetWeight (ai, active ? 1.0f : 0.0f);
    }
  }
}

}
}
