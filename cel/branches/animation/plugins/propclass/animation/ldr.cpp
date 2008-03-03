#include "cssysdef.h"
#include "plugins/propclass/animation/anim.h"
#include "csutil/xmltiny.h"
#include "iutil/databuff.h"
#include "iutil/document.h"
#include "iutil/objreg.h"
#include "iutil/vfs.h"
#include "ivaria/reporter.h"

bool celPcAnimation::Load (const char* path, const char* file)
{
  csRef<iDocumentSystem> docsys = 
    csQueryRegistry<iDocumentSystem> (object_reg);
  if (!docsys)
    docsys.AttachNew (new csTinyDocumentSystem ());
  csRef<iDocument> doc = docsys->CreateDocument ();
  csRef<iVFS> vfs = csQueryRegistry<iVFS> (object_reg);
  if (path)
  {
    vfs->PushDir ();
    vfs->ChDir (path);
  }
  csRef<iDataBuffer> buf = vfs->ReadFile (file);
  if (path)
    vfs->PopDir ();

  if (!buf)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR, "cel.pcobject.mesh.animation",
      "Error opening file '%s'!", file);
    return false;
  }
  const char* error = doc->Parse (buf, true);
  if (error != 0)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR, "cel.pcobject.mesh.animation",
      "XML parse error for file '%s': %s!", file, error);
    return false;
  }

  csRef<Skeleton::iGraveyard> skelgrave = csQueryRegistry<Skeleton::iGraveyard> (object_reg);
  if (!skelgrave)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR, "cel.pcobject.mesh.animation",
      "Missing crystalspace.nskeleton.graveyard!");
    return false;
  }

  csRef<iDocumentNode> animation = doc->GetRoot ()->GetNode ("animation");
  if (!animation)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
      "cel.pcobject.mesh.animation",
      "Badly formed Xml. Missing <animation>!");
    return false;
  }

  csRef<CEL::Animation::iAnimationSystem> animsys = csQueryRegistry<CEL::Animation::iAnimationSystem> (object_reg);
  if (!animsys)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR, "cel.pcobject.mesh.animation",
      "Missing cel.animation.system!");
    return false;
  }

  bool loaded_node = false;
  csRef<iDocumentNodeIterator> it = animation->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    csStringID id = xmltokens.Request (value);
    switch (id)
    {
      case XMLTOKEN_NODE:
      {
        if (!loaded_node)
        {
          ParseNode (child, animsys, 0);
          loaded_node = true;
        }
        break;
      }
    }
  }
  if (rootnode)
  {
    Skeleton::Animation::iMixingNode* mix = rootnode->GetMixingNode ();
    if (!mix)
    {
      csReport (object_reg, CS_REPORTER_SEVERITY_WARNING, "cel.pcobject.mesh.animation",
        "Root node '%s' did not create a node!", "NodesArentNamedYet");
    }
    skel->GetAnimationLayer ()->SetRootMixingNode (mix);
  }
  return true;
}
void celPcAnimation::ParseNode(iDocumentNode* xmlnode, const csRef<CEL::Animation::iAnimationSystem> &animsys,
  csRef<CEL::Animation::iNode> parent)
{
  // so far unused...
  const char* name = xmlnode->GetAttributeValue ("name");
  const char* type = xmlnode->GetAttributeValue ("type");
  if (!type)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_WARNING, "cel.pcobject.mesh.animation",
      "Missing node type... bailing out");
    return;
  }
  csRef<CEL::Animation::iNode> animnode = animsys->CreateNode (type);
  if (!animnode)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_WARNING, "cel.pcobject.mesh.animation",
      "404: Couldn't find node type '%s'!", type);
    return;
  }

  csRef<iDocumentNodeIterator> it = xmlnode->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    csStringID id = xmltokens.Request (value);
    switch (id)
    {
      case XMLTOKEN_NODE:
      {
        ParseNode (child, animsys, animnode);
        break;
      }
      case XMLTOKEN_PARAMETER:
      {
        const char* paramname = child->GetAttributeValue ("name");
        if (!paramname)
          continue;
        celData paramval;
        if (child->GetAttributeValue ("string"))
        {
          paramval.Set (child->GetAttributeValue ("string"));
          animnode->SetParameter (paramname, paramval);
        }
        else if (child->GetAttributeValue ("bool"))
        {
          paramval.Set (child->GetAttributeValueAsBool ("bool"));
          animnode->SetParameter (paramname, paramval);
        }
        else if (child->GetAttributeValue ("int"))
        {
          paramval.Set ((int32)child->GetAttributeValueAsInt ("int"));
          animnode->SetParameter (paramname, paramval);
        }
        else if (child->GetAttributeValue ("float"))
        {
          paramval.Set (child->GetAttributeValueAsFloat ("float"));
          animnode->SetParameter (paramname, paramval);
        }
      }
    }
  }

  if (!animnode->Initialise (object_reg, GetEntity (), skel))
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_WARNING, "cel.pcobject.mesh.animation",
      "Node didn't initialise! name: '%s' type: '%s'!", name, type);
    return;
  }
  if (parent)
    parent->AddChild (animnode);
  else
    rootnode = animnode;
}
