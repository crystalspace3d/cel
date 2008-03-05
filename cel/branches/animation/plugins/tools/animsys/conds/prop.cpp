#include "cssysdef.h"
#include "prop.h"
#include "ivaria/reporter.h"
#include "physicallayer/propclas.h"

namespace CEL
{
namespace Animation
{

PropertyCondition::PropertyCondition () : scfImplementationType (this), min (-FLT_MAX), max (FLT_MAX), propid (csInvalidStringID)
{
}
bool PropertyCondition::Initialise (iObjectRegistry *objreg, iCelEntity* ent, iNode* owner)
{
  pl = csQueryRegistry<iCelPlLayer> (objreg);
  if (!pl)
  {
    csReport (objreg, CS_REPORTER_SEVERITY_ERROR,
      "cel.animation.system",
      "Missing Physical Layer!");
    return false;
  }
  if (!ent)
  {
    csReport (objreg, CS_REPORTER_SEVERITY_WARNING, "cel.animation.system",
      "Impossible! Null entity passed!!");
    return false;
  }
  entity = ent;
  return true;
}
void PropertyCondition::AddChild (csRef<iCondition> c)
{
  children.Push (c);
}
void PropertyCondition::AttachResult (csRef<iResult> res)
{
  results.Push (res);
}
bool PropertyCondition::SetParameter (const char* name, const celData &param)
{
  if (!strcmp (name, "name") && param.type == CEL_DATA_STRING)
  {
    propname = param.value.s->GetData ();
  }
  else if (!strcmp (name, "propclass") && param.type == CEL_DATA_STRING)
  {
    propclassname = param.value.s->GetData ();
  }
  else if (!strcmp (name, "min") && param.type == CEL_DATA_FLOAT)
  {
    min = param.value.f;
  }
  else if (!strcmp (name, "max") && param.type == CEL_DATA_FLOAT)
  {
    max = param.value.f;
  }
  else if (!strcmp (name, "matches") && param.type == CEL_DATA_STRING)
  {
    matches = param.value.s->GetData ();
  }
  else
    return false;
  return true;
}
bool PropertyCondition::Evaluate ()
{
  if (!propname)
    return false;
  if (!pc && !pcprop)
  {
    if (!propclassname)
    {
      pcprop = celQueryPropertyClassEntity<iPcProperties> (entity);
      if (!pcprop)
        return false;
      propid = pcprop->GetPropertyIndex (propname);
    }
    else
    {
      pc = entity->GetPropertyClassList ()->FindByName (propclassname);
      if (!pc)
        return false;
      if (!pl)
        return false;
      propid = pl->FetchStringID (propname);
    }
  }
  if (propid == csArrayItemNotFound)
    return false;
  celDataType type;
  if (pcprop)
    type = pcprop->GetPropertyType (propid);
  else
    type = pc->GetPropertyOrActionType (propid);
  switch (type)
  {
    case CEL_DATA_STRING:
      if (matches)
      {
        if (pcprop)
        {
          if (strcmp (matches, pcprop->GetPropertyString (propid)) != 0)
            return false;
        }
        else
        {
          if (strcmp (matches, pc->GetPropertyStringByID (propid)) != 0)
            return false;
        }
      }
      break;
    case CEL_DATA_FLOAT:
    {
      float v;
      if (pcprop)
        v = pcprop->GetPropertyFloat (propid);
      else
        v = pc->GetPropertyFloatByID (propid);
      if (v < min)
        return false;
      if (v > max)
        return false;
      break;
    }
    default:
      break;
  }
  for (csRefArray<iCondition>::Iterator it = children.GetIterator (); it.HasNext (); )
  {
    iCondition* c = it.Next ();
    c->Evaluate ();
  }
  for (csRefArray<iResult>::Iterator it = results.GetIterator (); it.HasNext (); )
  {
    iResult* r = it.Next ();
    r->Execute ();
  }
  return true;
}

}
}
