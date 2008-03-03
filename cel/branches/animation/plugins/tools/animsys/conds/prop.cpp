#include "cssysdef.h"
#include "prop.h"
#include "ivaria/reporter.h"
#include "physicallayer/propclas.h"

namespace CEL
{
namespace Animation
{

PropertyCondition::PropertyCondition () : scfImplementationType (this), min (-FLT_MAX), max (FLT_MAX), propidx (csArrayItemNotFound),
  property_changed (false)
{
}
bool PropertyCondition::Initialise (iObjectRegistry *objreg, iCelEntity* ent, iNode* owner)
{
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
  return false;
}
bool PropertyCondition::Evaluate ()
{
  if (!propname)
    return false;
  if (!pcprop)
  {
    pcprop = celQueryPropertyClassEntity<iPcProperties> (entity);
    if (!pcprop)
      return false;
    pcprop->AddPropertyListener (this);
  }
  if (property_changed || propidx == csArrayItemNotFound)
  {
    propidx = pcprop->GetPropertyIndex (propname);
    if (propidx == csArrayItemNotFound)
      return false;
  }
  celDataType type = pcprop->GetPropertyType (propidx);
  switch (type)
  {
    case CEL_DATA_STRING:
      if (matches)
      {
        if (strcmp (matches, pcprop->GetPropertyString (propidx)) != 0)
        {
          return false;
        }
      }
      break;
    case CEL_DATA_FLOAT:
    {
      float v = pcprop->GetPropertyFloat (propidx);
      if (v < min)
        return false;
      if (v > max)
        return false;
      break;
    }
    default:
      break;
  }
  puts ("do your stuff!");
  return true;
}
void PropertyCondition::PropertyChanged (iPcProperties *pcprop, size_t idx)
{
  property_changed = true;
}

}
}
