/*
    Crystal Space Entity Layer
    Copyright (C) 2003 by Jorrit Tyberghein

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

#ifndef __CEL_PF_DYNMOVEFACT__
#define __CEL_PF_DYNMOVEFACT__

#include "cstypes.h"
#include "csgeom/transfrm.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "csutil/refarr.h"
#include "csutil/parray.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "plugins/propclass/common/stdpcimp.h"
#include "propclass/dynmove.h"
#include "ivaria/dynamics.h"

struct iVirtualClock;
struct iCelPlLayer;
struct iCelEntity;
struct iCelEntityList;
class csVector3;
class csReversibleTransform;
class csColliderWrapper;

/**
 * Factory for dynmove.
 */
CEL_DECLARE_FACTORY(DynamicSystem)
CEL_DECLARE_FACTORY(DynamicBody)

/**
 * This is a dynamic system.
 */
class celPcDynamicSystem : public celPcCommon
{
private:
  csRef<iDynamics> dynamics;
  csRef<iDynamicSystem> dynsystem;
  csRef<iVirtualClock> vc;
  float delta;

public:
  celPcDynamicSystem (iObjectRegistry* object_reg);
  virtual ~celPcDynamicSystem ();

  iDynamicSystem* GetDynamicSystem ();
  void SetStepTime (float delta)
  {
    celPcDynamicSystem::delta = delta;
  }
  float GetStepTime () const { return delta; }
  bool HandleEvent (iEvent& ev);

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcdynsys"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  struct PcDynamicSystem : public iPcDynamicSystem
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcDynamicSystem);
    virtual iDynamicSystem* GetDynamicSystem ()
    {
      return scfParent->GetDynamicSystem ();
    }
    virtual void SetStepTime (float delta)
    {
      scfParent->SetStepTime (delta);
    }
    virtual float GetStepTime () const
    {
      return scfParent->GetStepTime ();
    }
  } scfiPcDynamicSystem;

  // Not an embedded event handler to avoid circular references!!!
  class EventHandler : public iEventHandler
  {
  private:
    celPcDynamicSystem* parent;

  public:
    EventHandler (celPcDynamicSystem* parent)
    {
      SCF_CONSTRUCT_IBASE (0);
      EventHandler::parent = parent;
    }
    virtual ~EventHandler () { }

    SCF_DECLARE_IBASE;
    virtual bool HandleEvent (iEvent& ev)
    {
      return parent->HandleEvent (ev);
    }
  } *scfiEventHandler;
};

#define CEL_BODY_INVALID 0
#define CEL_BODY_SPHERE 1
#define CEL_BODY_BOX 2
#define CEL_BODY_CYLINDER 3
#define CEL_BODY_PLANE 4
#define CEL_BODY_MESH 5

struct body_data
{
  virtual ~body_data () { }
};

struct sphere_data : public body_data
{
  sphere_data (float radius, const csVector3& offset)
  {
    sphere_data::radius = radius;
    sphere_data::offset = offset;
  }
  float radius;
  csVector3 offset;
};

struct box_data : public body_data
{
  box_data (const csVector3& size, const csOrthoTransform& transform)
  {
    box_data::size = size;
    box_data::transform = transform;
  }
  csVector3 size;
  csOrthoTransform transform;
};

struct cylinder_data : public body_data
{
  cylinder_data (float length, float radius, const csOrthoTransform& transform)
  {
    cylinder_data::length = length;
    cylinder_data::radius = radius;
    cylinder_data::transform = transform;
  }
  float length;
  float radius;
  csOrthoTransform transform;
};

struct plane_data : public body_data
{
  plane_data (const csPlane3& plane)
  {
    plane_data::plane = plane;
  }
  csPlane3 plane;
};

/**
 * This is a dynamic body property class.
 */
class celPcDynamicBody : public celPcCommon
{
private:
  csRef<iPcDynamicSystem> dynsystem;
  csRef<iPcMesh> pcmesh;
  csRef<iRigidBody> body;
  int btype;
  body_data* bdata;

  bool is_static;

  float friction;
  float elasticity;
  float softness;
  float density;
  float mass;

public:
  celPcDynamicBody (iObjectRegistry* object_reg);
  virtual ~celPcDynamicBody ();

  void SetMesh (iPcMesh* mesh)
  {
    pcmesh = mesh;
  }
  iPcMesh* GetMesh ();

  void SetDynamicSystem (iPcDynamicSystem* dynsys)
  {
    dynsystem = dynsys;
  }

  iPcDynamicSystem* GetDynamicSystem ()
  {
    return dynsystem;
  }

  iRigidBody* GetBody ();

  void SetParameters (float friction, float density,
  	float elasticity, float softness, float mass)
  {
    celPcDynamicBody::friction = friction;
    celPcDynamicBody::density = density;
    celPcDynamicBody::elasticity = elasticity;
    celPcDynamicBody::softness = softness;
    celPcDynamicBody::mass = mass;
  }

  void MakeStatic (bool stat);
  bool IsStatic () const { return is_static; }

  void AttachColliderSphere (float radius, const csVector3& offset);
  void AttachColliderCylinder (float length, float radius,
  	const csOrthoTransform& trans);
  void AttachColliderBox (const csVector3& size,
  	const csOrthoTransform& trans);
  void AttachColliderPlane (const csPlane3& plane);
  void AttachColliderMesh ();

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcdynbody"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  struct PcDynamicBody : public iPcDynamicBody
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcDynamicBody);
    virtual void SetMesh (iPcMesh* mesh)
    {
      scfParent->SetMesh (mesh);
    }
    virtual iPcMesh* GetMesh ()
    {
      return scfParent->GetMesh ();
    }
    virtual void SetDynamicSystem (iPcDynamicSystem* dynsys)
    {
      scfParent->SetDynamicSystem (dynsys);
    }
    virtual iPcDynamicSystem* GetDynamicSystem ()
    {
      return scfParent->GetDynamicSystem ();
    }
    virtual iRigidBody* GetBody ()
    {
      return scfParent->GetBody ();
    }
    virtual void SetParameters (float friction, float density,
  	float elasticity, float softness, float mass)
    {
      scfParent->SetParameters (friction, density, elasticity, softness,
        mass);
    }
    virtual void MakeStatic (bool stat)
    {
      scfParent->MakeStatic (stat);
    }
    virtual bool IsStatic () const
    {
      return scfParent->IsStatic ();
    }
    virtual void AttachColliderSphere (float radius, const csVector3& offset)
    {
      scfParent->AttachColliderSphere (radius, offset);
    }
    virtual void AttachColliderCylinder (float length, float radius,
    	const csOrthoTransform& trans)
    {
      scfParent->AttachColliderCylinder (length, radius, trans);
    }
    virtual void AttachColliderBox (const csVector3& size,
  	const csOrthoTransform& trans)
    {
      scfParent->AttachColliderBox (size, trans);
    }
    virtual void AttachColliderPlane (const csPlane3& plane)
    {
      scfParent->AttachColliderPlane (plane);
    }
    virtual void AttachColliderMesh ()
    {
      scfParent->AttachColliderMesh ();
    }
  } scfiPcDynamicBody;
};

#endif // __CEL_PF_DYNMOVEFACT__

