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
#include "cstool/objectcomment.h"
#include "iutil/plugin.h"
#include "iutil/document.h"
#include "imap/services.h"
#include "imap/ldrctxt.h"
#include "iengine/collection.h"
#include "iengine/mesh.h"
#include "imesh/genmesh.h"

#include "dynworldload.h"

#include "physicallayer/pl.h"
#include "physicallayer/propclas.h"


SCF_IMPLEMENT_FACTORY (celAddOnDynamicWorldLoader)

enum
{
  XMLTOKEN_DYNWORLD,
  XMLTOKEN_DECAL,
  XMLTOKEN_FACTORY,
  XMLTOKEN_ATTR,
  XMLTOKEN_BOX,
  XMLTOKEN_CYLINDER,
  XMLTOKEN_CAPSULE,
  XMLTOKEN_SPHERE,
  XMLTOKEN_MESH,
  XMLTOKEN_CONVEXMESH,
  XMLTOKEN_MATERIAL,
  XMLTOKEN_POINT,
  XMLTOKEN_EXTENSION,
  XMLTOKEN_JOINT,
  XMLTOKEN_PIVOT,
  XMLTOKEN_LOGICBOX
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
  decalMgr = csQueryRegistry<iDecalManager> (object_reg);
  if (!decalMgr)
  {
    printf ("No decal manager!\n");
    return false;
  }
  engine = csQueryRegistry<iEngine> (object_reg);
  if (!engine)
  {
    printf ("No engine!\n");
    return false;
  }

  xmltokens.Register ("dynworld", XMLTOKEN_DYNWORLD);
  xmltokens.Register ("factory", XMLTOKEN_FACTORY);
  xmltokens.Register ("decal", XMLTOKEN_DECAL);
  xmltokens.Register ("attr", XMLTOKEN_ATTR);
  xmltokens.Register ("box", XMLTOKEN_BOX);
  xmltokens.Register ("cylinder", XMLTOKEN_CYLINDER);
  xmltokens.Register ("capsule", XMLTOKEN_CAPSULE);
  xmltokens.Register ("sphere", XMLTOKEN_SPHERE);
  xmltokens.Register ("mesh", XMLTOKEN_MESH);
  xmltokens.Register ("convexmesh", XMLTOKEN_CONVEXMESH);
  xmltokens.Register ("material", XMLTOKEN_MATERIAL);
  xmltokens.Register ("point", XMLTOKEN_POINT);
  xmltokens.Register ("extension", XMLTOKEN_EXTENSION);
  xmltokens.Register ("joint", XMLTOKEN_JOINT);
  xmltokens.Register ("pivot", XMLTOKEN_PIVOT);
  xmltokens.Register ("logicbox", XMLTOKEN_LOGICBOX);

  return true;
}

bool celAddOnDynamicWorldLoader::ParseFactory (iDocumentNode* node, iLoaderContext* context)
{
  csString name = node->GetAttributeValue ("name");
  iDynamicFactory* fact = dynworld->FindFactory (name);
  if (fact)
  {
    synldr->Report ("dynworld.loader", CS_REPORTER_SEVERITY_WARNING, node,
	  "Factory '%s' is already in memory!", name.GetData ());
    return true;
  }

  float maxradius = 1.0f;
  if (node->GetAttribute ("maxradius"))
    maxradius = node->GetAttributeValueAsFloat ("maxradius");
  float imposterradius = -1.0f;
  if (node->GetAttribute ("imposterradius"))
    imposterradius = node->GetAttributeValueAsFloat ("imposterradius");
  float mass = 1.0f;
  if (node->GetAttribute ("mass"))
    mass = node->GetAttributeValueAsFloat ("mass");
  bool logic = false;
  if (node->GetAttribute ("logic"))
    logic = node->GetAttributeValueAsBool ("logic");
  bool light = false;
  if (node->GetAttribute ("light"))
    light = node->GetAttributeValueAsBool ("light");
  bool hasCollider = false;
  if (node->GetAttribute ("collider"))
    hasCollider = node->GetAttributeValueAsBool ("collider");

  csVector3 min, max;
  if (logic)
  {
    csRef<iDocumentNode> boxNode = node->GetNode ("logicbox");
    if (!boxNode)
    {
      synldr->ReportError ("dynworld.loader", node,
	  "Can't find logicbox for logic factory '%s'!", name.GetData ());
      return false;
    }
    csRef<iDocumentNode> minNode = boxNode->GetNode ("min");
    if (!minNode || !synldr->ParseVector (minNode, min))
    {
      synldr->ReportError ("dynworld.loader", node,
	  "Error parsing 'min' for logic box for factory '%s'!", name.GetData ());
      return false;
    }
    csRef<iDocumentNode> maxNode = boxNode->GetNode ("max");
    if (!maxNode || !synldr->ParseVector (maxNode, max))
    {
      synldr->ReportError ("dynworld.loader", node,
	  "Error parsing 'max' for logic box for factory '%s'!", name.GetData ());
      return false;
    }
  }

  if (logic)
    fact = dynworld->AddLogicFactory (name, maxradius, imposterradius, csBox3 (min, max));
  else if (light)
    fact = dynworld->AddLightFactory (name, maxradius);
  else
    fact = dynworld->AddFactory (name, maxradius, imposterradius);

  if (!fact)
  {
    synldr->ReportError ("dynworld.loader", node,
	"Could not add factory '%s'!", name.GetData ());
    return false;
  }
  if (context)
    context->AddToCollection (fact->QueryObject ());

  if (node->GetAttribute ("template"))
  {
    csString tmpName = node->GetAttributeValue ("template");
    fact->SetDefaultEntityTemplate (tmpName);
  }

  fact->SetColliderEnabled (hasCollider);

  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();

    if (context && child->GetType () == CS_NODE_COMMENT)
      context->LoadComment (fact->QueryObject (), child);

    if (child->GetType () != CS_NODE_ELEMENT) continue;
    csStringID id = xmltokens.Request (child->GetValue ());
    switch (id)
    {
      case XMLTOKEN_LOGICBOX:
	// Ignored here, already handled above.
	break;
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
      case XMLTOKEN_CAPSULE:
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
	    if (id == XMLTOKEN_CYLINDER)
	      fact->AddRigidCylinder (radius, length, offset, mass);
	    else
	      fact->AddRigidCapsule (radius, length, offset, mass);
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
      case XMLTOKEN_DECAL:
	{
	  csString name = child->GetAttributeValue ("name");
	  if (dynworld->FindDecalTemplate (name))
	  {
	    synldr->ReportError ("dynworld.loader", child,
		"Decal template '%s' is already defined!",
		name.GetData ());
	    return 0;
	  }
	  csString material = child->GetAttributeValue ("material");
	  iMaterialWrapper* mat = engine->GetMaterialList ()->FindByName (material);
	  if (!mat)
	  {
	    synldr->ReportError ("dynworld.loader", child,
		"Cannot find material '%s'!",
		material.GetData ());
	    return 0;
	  }
	  csString uvminString = child->GetAttributeValue ("uvmin", "0,0");
	  csVector2 uvmin;
	  csScanStr (uvminString.GetData (), "%f,%f", &uvmin.x, &uvmin.y);
	  csString uvmaxString = child->GetAttributeValue ("uvmax", "1,1");
	  csVector2 uvmax;
	  csScanStr (uvmaxString.GetData (), "%f,%f", &uvmax.x, &uvmax.y);

	  csRef<iDecalTemplate> decalTpl = decalMgr->CreateDecalTemplate (mat);
	  decalTpl->SetTopClipping (false);
	  decalTpl->SetBottomClipping (false, 0.0f);
	  decalTpl->SetTexCoords (uvmin, uvmax);
	  dynworld->RegisterDecalTemplate (name, decalTpl);
	  if (ldr_context)
	    ldr_context->AddToCollection (decalTpl->QueryObject ());
	}
	break;
      case XMLTOKEN_FACTORY:
	if (!ParseFactory (child, ldr_context)) return 0;
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

bool celAddOnDynamicWorldLoader::WriteBodies (iDocumentNode* factNode, iDynamicFactory* fact)
{
  float mass = 0.0f;
  for (size_t j = 0 ; j < fact->GetBodyCount () ; j++)
  {
    celBodyInfo info = fact->GetBody (j);
    if (info.mass > mass) mass = info.mass;
    switch (info.type)
    {
      case NO_GEOMETRY:
      case PLANE_COLLIDER_GEOMETRY:
	break;
      case BOX_COLLIDER_GEOMETRY:
	{
	  csRef<iDocumentNode> bodyNode = factNode->CreateNodeBefore (CS_NODE_ELEMENT, 0);
	  bodyNode->SetValue ("box");
	  csRef<iDocumentNode> offsetNode = bodyNode->CreateNodeBefore (CS_NODE_ELEMENT, 0);
	  offsetNode->SetValue ("offset");
	  offsetNode->SetAttributeAsFloat ("x", info.offset.x);
	  offsetNode->SetAttributeAsFloat ("y", info.offset.y);
	  offsetNode->SetAttributeAsFloat ("z", info.offset.z);
	  csRef<iDocumentNode> sizeNode = bodyNode->CreateNodeBefore (CS_NODE_ELEMENT, 0);
	  sizeNode->SetValue ("size");
	  sizeNode->SetAttributeAsFloat ("x", info.size.x);
	  sizeNode->SetAttributeAsFloat ("y", info.size.y);
	  sizeNode->SetAttributeAsFloat ("z", info.size.z);
	}
	break;
      case CYLINDER_COLLIDER_GEOMETRY:
	{
	  csRef<iDocumentNode> bodyNode = factNode->CreateNodeBefore (CS_NODE_ELEMENT, 0);
	  bodyNode->SetValue ("cylinder");
	  csRef<iDocumentNode> offsetNode = bodyNode->CreateNodeBefore (CS_NODE_ELEMENT, 0);
	  offsetNode->SetValue ("offset");
	  offsetNode->SetAttributeAsFloat ("x", info.offset.x);
	  offsetNode->SetAttributeAsFloat ("y", info.offset.y);
	  offsetNode->SetAttributeAsFloat ("z", info.offset.z);
	  bodyNode->SetAttributeAsFloat ("radius", info.radius);
	  bodyNode->SetAttributeAsFloat ("length", info.length);
	}
	break;
      case CAPSULE_COLLIDER_GEOMETRY:
	{
	  csRef<iDocumentNode> bodyNode = factNode->CreateNodeBefore (CS_NODE_ELEMENT, 0);
	  bodyNode->SetValue ("capsule");
	  csRef<iDocumentNode> offsetNode = bodyNode->CreateNodeBefore (CS_NODE_ELEMENT, 0);
	  offsetNode->SetValue ("offset");
	  offsetNode->SetAttributeAsFloat ("x", info.offset.x);
	  offsetNode->SetAttributeAsFloat ("y", info.offset.y);
	  offsetNode->SetAttributeAsFloat ("z", info.offset.z);
	  bodyNode->SetAttributeAsFloat ("radius", info.radius);
	  bodyNode->SetAttributeAsFloat ("length", info.length);
	}
	break;
      case SPHERE_COLLIDER_GEOMETRY:
	{
	  csRef<iDocumentNode> bodyNode = factNode->CreateNodeBefore (CS_NODE_ELEMENT, 0);
	  bodyNode->SetValue ("sphere");
	  csRef<iDocumentNode> offsetNode = bodyNode->CreateNodeBefore (CS_NODE_ELEMENT, 0);
	  offsetNode->SetValue ("offset");
	  offsetNode->SetAttributeAsFloat ("x", info.offset.x);
	  offsetNode->SetAttributeAsFloat ("y", info.offset.y);
	  offsetNode->SetAttributeAsFloat ("z", info.offset.z);
	  bodyNode->SetAttributeAsFloat ("radius", info.radius);
	}
	break;
      case CONVEXMESH_COLLIDER_GEOMETRY:
	{
	  csRef<iDocumentNode> bodyNode = factNode->CreateNodeBefore (CS_NODE_ELEMENT, 0);
	  bodyNode->SetValue ("convexmesh");
	  csRef<iDocumentNode> offsetNode = bodyNode->CreateNodeBefore (CS_NODE_ELEMENT, 0);
	  offsetNode->SetValue ("offset");
	  offsetNode->SetAttributeAsFloat ("x", info.offset.x);
	  offsetNode->SetAttributeAsFloat ("y", info.offset.y);
	  offsetNode->SetAttributeAsFloat ("z", info.offset.z);
	}
	break;
      case TRIMESH_COLLIDER_GEOMETRY:
	{
	  csRef<iDocumentNode> bodyNode = factNode->CreateNodeBefore (CS_NODE_ELEMENT, 0);
	  bodyNode->SetValue ("mesh");
	  csRef<iDocumentNode> offsetNode = bodyNode->CreateNodeBefore (CS_NODE_ELEMENT, 0);
	  offsetNode->SetValue ("offset");
	  offsetNode->SetAttributeAsFloat ("x", info.offset.x);
	  offsetNode->SetAttributeAsFloat ("y", info.offset.y);
	  offsetNode->SetAttributeAsFloat ("z", info.offset.z);
	}
	break;
    }
  }
  factNode->SetAttributeAsFloat ("mass", mass);
  return true;
}

bool celAddOnDynamicWorldLoader::WritePivots (iDocumentNode* factNode, iDynamicFactory* fact)
{
  for (size_t i = 0 ; i < fact->GetPivotJointCount () ; i++)
  {
    csVector3 pos = fact->GetPivotJointPosition (i);
    csRef<iDocumentNode> pivotNode = factNode->CreateNodeBefore (CS_NODE_ELEMENT, 0);
    pivotNode->SetValue ("pivot");
    csRef<iDocumentNode> offsetNode = pivotNode->CreateNodeBefore (CS_NODE_ELEMENT, 0);
    offsetNode->SetValue ("origin");
    offsetNode->SetAttributeAsFloat ("x", pos.x);
    offsetNode->SetAttributeAsFloat ("y", pos.y);
    offsetNode->SetAttributeAsFloat ("z", pos.z);
  }
  return true;
}

static void WriteVector (iDocumentNode* parent, const char* name, const csVector3& v)
{
  csRef<iDocumentNode> node = parent->CreateNodeBefore (CS_NODE_ELEMENT, 0);
  node->SetValue (name);
  node->SetAttributeAsFloat ("x", v.x);
  node->SetAttributeAsFloat ("y", v.y);
  node->SetAttributeAsFloat ("z", v.z);
}

bool celAddOnDynamicWorldLoader::WriteJoints (iDocumentNode* factNode, iDynamicFactory* fact)
{
  for (size_t i = 0 ; i < fact->GetJointCount () ; i++)
  {
    DynFactJointDefinition def = fact->GetJoint (i);
    csRef<iDocumentNode> jointNode = factNode->CreateNodeBefore (CS_NODE_ELEMENT, 0);
    jointNode->SetValue ("joint");
    WriteVector (jointNode, "origin", def.trans.GetOrigin ());
    csRef<iDocumentNode> locktransNode = jointNode->CreateNodeBefore (CS_NODE_ELEMENT, 0);
    locktransNode->SetValue ("locktrans");
    locktransNode->SetAttribute ("x", def.transX ? "true" : "false");
    locktransNode->SetAttribute ("y", def.transY ? "true" : "false");
    locktransNode->SetAttribute ("z", def.transZ ? "true" : "false");
    WriteVector (jointNode, "mindist", def.mindist);
    WriteVector (jointNode, "maxdist", def.maxdist);
    csRef<iDocumentNode> lockrotNode = jointNode->CreateNodeBefore (CS_NODE_ELEMENT, 0);
    lockrotNode->SetValue ("lockrot");
    lockrotNode->SetAttribute ("x", def.rotX ? "true" : "false");
    lockrotNode->SetAttribute ("y", def.rotY ? "true" : "false");
    lockrotNode->SetAttribute ("z", def.rotZ ? "true" : "false");
    WriteVector (jointNode, "minrot", def.minrot);
    WriteVector (jointNode, "maxrot", def.maxrot);
    WriteVector (jointNode, "bounce", def.bounce);
    WriteVector (jointNode, "maxforce", def.maxforce);
  }
  return true;
}

bool celAddOnDynamicWorldLoader::WriteFactories (iPcDynamicWorld* dynworld, iDocumentNode* parent,
    iCollection* collection, iStreamSource* ssource)
{
  csRef<iCelPropertyClass> pcdynworld = scfQueryInterface<iCelPropertyClass> (dynworld);

  csRef<iDocumentNode> dynworldNode = parent->CreateNodeBefore (CS_NODE_ELEMENT, 0);
  dynworldNode->SetValue ("dynworld");
  iCelEntity* entity = pcdynworld->GetEntity ();
  dynworldNode->SetAttribute ("name", entity->GetName ());

  for (size_t i = 0 ; i < dynworld->GetDecalTemplateCount () ; i++)
  {
    iDecalTemplate* tpl = dynworld->GetDecalTemplate (i);
    if (!collection || collection->IsParentOf (tpl->QueryObject ()))
    {
      csRef<iDocumentNode> tplNode = parent->CreateNodeBefore (CS_NODE_ELEMENT, 0);
      CS::Persistence::SaveComment (object_reg, tpl->QueryObject (), tplNode);
      tplNode->SetAttribute ("name", tpl->QueryObject ()->GetName ());
      tplNode->SetAttribute ("material",
	  tpl->GetMaterialWrapper ()->QueryObject ()->GetName ());
      const csVector2& uvmin = tpl->GetMinTexCoord ();
      const csVector2& uvmax = tpl->GetMaxTexCoord ();
      csString uvminString;
      uvminString.Format ("%g,%g", uvmin.x, uvmin.y);
      csString uvmaxString;
      uvmaxString.Format ("%g,%g", uvmax.x, uvmax.y);
    }
  }

  for (size_t i = 0 ; i < dynworld->GetFactoryCount () ; i++)
  {
    iDynamicFactory* fact = dynworld->GetFactory (i);
    if (!collection || collection->IsParentOf (fact->QueryObject ()))
    {
      csRef<iDocumentNode> factNode = parent->CreateNodeBefore (CS_NODE_ELEMENT, 0);
      CS::Persistence::SaveComment (object_reg, fact->QueryObject (), factNode);
      factNode->SetValue ("factory");
      factNode->SetAttribute ("name", fact->GetName ());
      factNode->SetAttributeAsFloat ("maxradius", fact->GetMaximumRadiusRelative ());
      factNode->SetAttributeAsFloat ("imposterradius", fact->GetImposterRadius ());
      if (fact->IsLogicFactory ())
	factNode->SetAttribute ("logic", "true");
      if (fact->IsLightFactory ())
	factNode->SetAttribute ("light", "true");
      if (fact->GetDefaultEntityTemplate ())
	factNode->SetAttribute ("template", fact->GetDefaultEntityTemplate ());
      if (fact->IsColliderEnabled ())
	factNode->SetAttribute ("collider", "true");
      if (fact->IsLogicFactory ())
      {
	const csBox3& bbox = fact->GetBBox ();
	csRef<iDocumentNode> boxNode = factNode->CreateNodeBefore (CS_NODE_ELEMENT, 0);
	boxNode->SetValue ("logicbox");
	csRef<iDocumentNode> minNode = boxNode->CreateNodeBefore (CS_NODE_ELEMENT, 0);
	minNode->SetValue ("min");
	minNode->SetAttributeAsFloat ("x", bbox.MinX ());
	minNode->SetAttributeAsFloat ("y", bbox.MinY ());
	minNode->SetAttributeAsFloat ("z", bbox.MinZ ());
	csRef<iDocumentNode> maxNode = boxNode->CreateNodeBefore (CS_NODE_ELEMENT, 0);
	maxNode->SetValue ("max");
	maxNode->SetAttributeAsFloat ("x", bbox.MaxX ());
	maxNode->SetAttributeAsFloat ("y", bbox.MaxY ());
	maxNode->SetAttributeAsFloat ("z", bbox.MaxZ ());
      }

      csRef<iAttributeIterator> attrIt = fact->GetAttributes ();
      while (attrIt->HasNext ())
      {
	csStringID nameID = attrIt->Next ();
	csString name = pl->FetchString (nameID);
	csString value = fact->GetAttribute (nameID);
	csRef<iDocumentNode> attrNode = factNode->CreateNodeBefore (CS_NODE_ELEMENT, 0);
	attrNode->SetValue ("attr");
	attrNode->SetAttribute ("name", name);
	attrNode->SetAttribute ("value", value);
      }
      if (!WriteBodies (factNode, fact)) return false;
      if (!WritePivots (factNode, fact)) return false;
      if (!WriteJoints (factNode, fact)) return false;
    }
  }

  return true;
}

bool celAddOnDynamicWorldLoader::WriteDown (iBase* obj, iDocumentNode* parent,
    iStreamSource* ssource)
{
  csRef<iPcDynamicWorld> dynworld = scfQueryInterface<iPcDynamicWorld> (obj);
  if (!dynworld)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	"cel.addons.celentitytpl",
	"The given object is not a dynamic world!\n");
    return false;
  }
  return WriteFactories (dynworld, parent, 0, ssource);
}

