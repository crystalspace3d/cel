/*
    Crystal Space Entity Layer
    Copyright (C) 2011 by Jorrit Tyberghein
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
  
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
  
    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include "cssysdef.h"

#include "csgeom/box.h"
#include "csutil/scanstr.h"
#include "iutil/plugin.h"
#include "iutil/document.h"
#include "imap/services.h"
#include "iengine/mesh.h"
#include "imesh/genmesh.h"

#include "dynworldload.h"

#include "physicallayer/pl.h"
#include "physicallayer/propclas.h"


SCF_IMPLEMENT_FACTORY (celAddOnDynamicWorldLoader)

enum
{
  XMLTOKEN_DYNWORLD,
  XMLTOKEN_FACTORY,
  XMLTOKEN_ATTR,
  XMLTOKEN_BOX,
  XMLTOKEN_CYLINDER,
  XMLTOKEN_SPHERE,
  XMLTOKEN_MESH,
  XMLTOKEN_CONVEXMESH,
  XMLTOKEN_MATERIAL,
  XMLTOKEN_POINT,
  XMLTOKEN_EXTENSION,
  XMLTOKEN_JOINT,
  XMLTOKEN_PIVOT,
  XMLTOKEN_INVISIBLE
};

//---------------------------------------------------------------------------------------

celAddOnDynamicWorldLoader::celAddOnDynamicWorldLoader (iBase *parent) :
  scfImplementationType (this, parent)
{
}

celAddOnDynamicWorldLoader::~celAddOnDynamicWorldLoader ()
{
}

bool celAddOnDynamicWorldLoader::Initialize (iObjectRegistry *object_reg)
{
  celAddOnDynamicWorldLoader::object_reg = object_reg;
  pl = csQueryRegistry<iCelPlLayer> (object_reg);
  if (!pl)
  {
    printf ("Can't find CEL physical layer!\n");
    return false;
  }
  synldr = csQueryRegistry<iSyntaxService> (object_reg);
  if (!synldr)
  {
    printf ("No syntax service!\n");
    return false;
  }

  xmltokens.Register ("dynworld", XMLTOKEN_DYNWORLD);
  xmltokens.Register ("factory", XMLTOKEN_FACTORY);
  xmltokens.Register ("attr", XMLTOKEN_ATTR);
  xmltokens.Register ("box", XMLTOKEN_BOX);
  xmltokens.Register ("cylinder", XMLTOKEN_CYLINDER);
  xmltokens.Register ("sphere", XMLTOKEN_SPHERE);
  xmltokens.Register ("mesh", XMLTOKEN_MESH);
  xmltokens.Register ("convexmesh", XMLTOKEN_CONVEXMESH);
  xmltokens.Register ("material", XMLTOKEN_MATERIAL);
  xmltokens.Register ("point", XMLTOKEN_POINT);
  xmltokens.Register ("extension", XMLTOKEN_EXTENSION);
  xmltokens.Register ("joint", XMLTOKEN_JOINT);
  xmltokens.Register ("pivot", XMLTOKEN_PIVOT);
  xmltokens.Register ("invisible", XMLTOKEN_INVISIBLE);

  return true;
}

bool celAddOnDynamicWorldLoader::ParseFactory (iDocumentNode* node)
{
  csString name = node->GetAttributeValue ("name");
  float maxradius = 1.0f;
  if (node->GetAttribute ("maxradius"))
    maxradius = node->GetAttributeValueAsFloat ("maxradius");
  float imposterradius = -1.0f;
  if (node->GetAttribute ("imposterradius"))
    imposterradius = node->GetAttributeValueAsFloat ("imposterradius");
  float mass = 1.0f;
  if (node->GetAttribute ("mass"))
    mass = node->GetAttributeValueAsFloat ("mass");
  iDynamicFactory* fact = dynworld->AddFactory (name, maxradius, imposterradius);
  if (!fact)
  {
    synldr->ReportError ("dynworld.loader", node,
	"Could not add factory '%s'!", name.GetData ());
    return false;
  }
  bool invis = false;
  if (node->GetAttribute ("invisible"))
    invis = node->GetAttributeValueAsBool ("invisible");
  if (invis)
    fact->SetInvisible (invis);
  if (node->GetAttribute ("template"))
  {
    csString tmpName = node->GetAttributeValue ("template");
    fact->SetDefaultEntityTemplate (tmpName);
  }

  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    csStringID id = xmltokens.Request (child->GetValue ());
    switch (id)
    {
      case XMLTOKEN_ATTR:
	fact->SetAttribute (child->GetAttributeValue ("name"),
	    child->GetAttributeValue ("value"));
	break;
      case XMLTOKEN_PIVOT:
	{
	  csRef<iDocumentNode> originNode = child->GetNode ("origin");
	  if (originNode)
	  {
	    csVector3 origin;
	    if (!synldr->ParseVector (originNode, origin))
	    {
	      synldr->ReportError ("dynworld.loader", child,
		  "Error parsing 'origin' for factory '%s'!", name.GetData ());
	      return false;
	    }
	    fact->CreatePivotJoint (origin);
	  }
	}
	break;
      case XMLTOKEN_JOINT:
	{
	  DynFactJointDefinition& def = fact->CreateJoint ();
	  csRef<iDocumentNode> originNode = child->GetNode ("origin");
	  if (originNode)
	  {
	    csVector3 origin;
	    if (!synldr->ParseVector (originNode, origin))
	    {
	      synldr->ReportError ("dynworld.loader", child,
		  "Error parsing 'origin' for factory '%s'!", name.GetData ());
	      return false;
	    }
	    def.trans.SetOrigin (origin);
	  }
	  csRef<iDocumentNode> transNode = child->GetNode ("locktrans");
	  if (transNode)
	  {
	    def.transX = transNode->GetAttributeValueAsBool ("x");
	    def.transY = transNode->GetAttributeValueAsBool ("y");
	    def.transZ = transNode->GetAttributeValueAsBool ("z");
	  }
	  csRef<iDocumentNode> rotNode = child->GetNode ("lockrot");
	  if (rotNode)
	  {
	    def.rotX = rotNode->GetAttributeValueAsBool ("x");
	    def.rotY = rotNode->GetAttributeValueAsBool ("y");
	    def.rotZ = rotNode->GetAttributeValueAsBool ("z");
	  }
	  csRef<iDocumentNode> mindistNode = child->GetNode ("mindist");
	  if (mindistNode)
	  {
	    if (!synldr->ParseVector (mindistNode, def.mindist))
	    {
	      synldr->ReportError ("dynworld.loader", child,
		  "Error parsing 'mindist' for factory '%s'!", name.GetData ());
	      return false;
	    }
	  }
	  csRef<iDocumentNode> maxdistNode = child->GetNode ("maxdist");
	  if (maxdistNode)
	  {
	    if (!synldr->ParseVector (maxdistNode, def.maxdist))
	    {
	      synldr->ReportError ("dynworld.loader", child,
		  "Error parsing 'maxdist' for factory '%s'!", name.GetData ());
	      return false;
	    }
	  }
	  csRef<iDocumentNode> minrotNode = child->GetNode ("minrot");
	  if (minrotNode)
	  {
	    if (!synldr->ParseVector (minrotNode, def.minrot))
	    {
	      synldr->ReportError ("dynworld.loader", child,
		  "Error parsing 'minrot' for factory '%s'!", name.GetData ());
	      return false;
	    }
	  }
	  csRef<iDocumentNode> maxrotNode = child->GetNode ("maxrot");
	  if (maxrotNode)
	  {
	    if (!synldr->ParseVector (maxrotNode, def.maxrot))
	    {
	      synldr->ReportError ("dynworld.loader", child,
		  "Error parsing 'maxrot' for factory '%s'!", name.GetData ());
	      return false;
	    }
	  }
	  csRef<iDocumentNode> bounceNode = child->GetNode ("bounce");
	  if (bounceNode)
	  {
	    if (!synldr->ParseVector (bounceNode, def.bounce))
	    {
	      synldr->ReportError ("dynworld.loader", child,
		  "Error parsing 'bounce' for factory '%s'!", name.GetData ());
	      return false;
	    }
	  }
	  csRef<iDocumentNode> maxforceNode = child->GetNode ("maxforce");
	  if (maxforceNode)
	  {
	    if (!synldr->ParseVector (maxforceNode, def.maxforce))
	    {
	      synldr->ReportError ("dynworld.loader", child,
		  "Error parsing 'maxforce' for factory '%s'!", name.GetData ());
	      return false;
	    }
	  }
	}
	break;
      case XMLTOKEN_BOX:
	{
	  csRef<iDocumentNode> offsetNode = child->GetNode ("offset");
	  csRef<iDocumentNode> sizeNode = child->GetNode ("size");
	  if (!offsetNode && !sizeNode)
	  {
	    const csBox3& bbox = fact->GetBBox ();
	    fact->AddRigidBox (bbox.GetCenter (), bbox.GetSize (), mass);
	  }
	  else
	  {
	    csVector3 offset (0), size;
	    if (offsetNode && !synldr->ParseVector (offsetNode, offset))
	    {
	      synldr->ReportError ("dynworld.loader", child,
		  "Error loading 'offset' for factory '%s'!", name.GetData ());
	      return false;
	    }

	    if (!sizeNode || !synldr->ParseVector (sizeNode, size))
	    {
	      synldr->ReportError ("dynworld.loader", child,
		"Error loading 'size' for factory '%s'!", name.GetData ());
	      return false;
	    }
	    fact->AddRigidBox (offset, size, mass);
	  }
	}
	break;
      case XMLTOKEN_CYLINDER:
	{
	  float radius = -1, length = -1;
	  csRef<iDocumentNode> offsetNode = child->GetNode ("offset");
	  if (!child->GetAttribute ("radius") && !child->GetAttribute ("length") &&
	      !offsetNode)
	  {
	    const csBox3& bbox = fact->GetBBox ();
	    csVector3 size = bbox.GetSize ();
	    radius = size.x;
	    if (size.z > radius) radius = size.z;
	    radius /= 2.0;
	    length = size.y;
	    fact->AddRigidCylinder (radius, length, bbox.GetCenter (), mass);
	  }
	  else
	  {
	    radius = child->GetAttributeValueAsFloat ("radius");
	    length = child->GetAttributeValueAsFloat ("length");
	    csVector3 offset (0);
	    if (offsetNode && !synldr->ParseVector (offsetNode, offset))
	    {
	      synldr->ReportError ("dynworld.loader", child,
		  "Error loading 'offset' for factory '%s'!", name.GetData ());
	      return false;
	    }

	    if (radius < 0)
	    {
	      synldr->ReportError ("dynworld.loader", child,
		"Error loading 'radius' for factory '%s'!", name.GetData ());
	      return false;
	    }
	    if (length < 0)
	    {
	      synldr->ReportError ("dynworld.loader", child,
		"Error loading 'length' for factory '%s'!", name.GetData ());
	      return false;
	    }
	    fact->AddRigidCylinder (radius, length, offset, mass);
	  }
	}
	break;
      case XMLTOKEN_SPHERE:
	{
	  float radius = -1;
	  csRef<iDocumentNode> offsetNode = child->GetNode ("offset");
	  if (!child->GetAttribute ("radius") && !offsetNode)
	  {
	    const csBox3& bbox = fact->GetBBox ();
	    csVector3 size = bbox.GetSize ();
	    radius = size.x;
	    if (size.y > radius) radius = size.y;
	    if (size.z > radius) radius = size.z;
	    radius /= 2.0;
	    fact->AddRigidSphere (radius, bbox.GetCenter (), mass);
	  }
	  else
	  {
	    radius = child->GetAttributeValueAsFloat ("radius");
	    csVector3 offset (0);
	    if (offsetNode && !synldr->ParseVector (offsetNode, offset))
	    {
	      synldr->ReportError ("dynworld.loader", child,
		  "Error loading 'offset' for factory '%s'!", name.GetData ());
	      return false;
	    }

	    if (radius < 0)
	    {
	      synldr->ReportError ("dynworld.loader", child,
		"Error loading 'radius' for factory '%s'!", name.GetData ());
	      return false;
	    }
	    fact->AddRigidSphere (radius, offset, mass);
	  }
	}
	break;
      case XMLTOKEN_MESH:
	{
	  csVector3 offset (0);
	  csRef<iDocumentNode> offsetNode = child->GetNode ("offset");
	  if (offsetNode && !synldr->ParseVector (offsetNode, offset))
	  {
	    synldr->ReportError ("dynworld.loader", child,
		  "Error loading 'offset' for factory '%s'!", name.GetData ());
	    return false;
	  }
	  fact->AddRigidMesh (offset, mass);
	}
	break;
      case XMLTOKEN_CONVEXMESH:
	{
	  csVector3 offset (0);
	  csRef<iDocumentNode> offsetNode = child->GetNode ("offset");
	  if (offsetNode && !synldr->ParseVector (offsetNode, offset))
	  {
	    synldr->ReportError ("dynworld.loader", child,
		  "Error loading 'offset' for factory '%s'!", name.GetData ());
	    return false;
	  }
	  fact->AddRigidConvexMesh (offset, mass);
	}
	break;
      default:
        synldr->ReportBadToken (child);
	return false;
    }
  }
  return true;
}

csPtr<iBase> celAddOnDynamicWorldLoader::Parse (iDocumentNode* node,
  	iStreamSource* ssource, iLoaderContext* ldr_context,
  	iBase* context)
{
  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    csStringID id = xmltokens.Request (child->GetValue ());
    switch (id)
    {
      case XMLTOKEN_EXTENSION:
	{
	  csString plugin = child->GetAttributeValue ("plugin");
	  csRef<iDynamicWorldLoaderExtension> extension = csLoadPlugin<iDynamicWorldLoaderExtension> (object_reg, plugin);
	  if (!extension)
	  {
	    synldr->ReportError ("dynworld.loader", child,
		"Could not load extension plugin '%s'!", plugin.GetData ());
	    return 0;
	  }
	  extensions.Push (extension);
	}
	break;
      case XMLTOKEN_DYNWORLD:
	{
	  csString name = child->GetAttributeValue ("name");
	  csRef<iCelEntity> dynworldEntity = pl->FindEntity (name);
	  if (!dynworldEntity)
	  {
	    synldr->ReportError ("dynworld.loader", child,
		"Could not find entity '%s'!", name.GetData ());
	    return 0;
	  }
	  dynworld = celQueryPropertyClassEntity<iPcDynamicWorld> (dynworldEntity);
	  if (!dynworld)
	  {
	    synldr->ReportError ("dynworld.loader", child,
		"Entity '%s' does not have a dynamic world property class!",
		name.GetData ());
	    return 0;
	  }
	}
	break;
      case XMLTOKEN_FACTORY:
	if (!ParseFactory (child)) return 0;
	break;
      default:
	{
	  bool handled = false;
	  for (size_t i = 0 ; i < extensions.GetSize () ; i++)
	  {
	    if (extensions[i]->Parse (child, dynworld))
	    {
	      handled = true;
	      break;
	    }
	  }
	  if (!handled)
	  {
            synldr->ReportBadToken (child);
	    return 0;
	  }
	}
	break;
    }
  }

  IncRef ();
  return this;
}


