/*
    Crystal Space Entity Layer
    Copyright (C) 2001 by Jorrit Tyberghein
  
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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __CEL_PF_MOVEFACT__
#define __CEL_PF_MOVEFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "csutil/csvector.h"
#include "pl/propclas.h"
#include "pl/propfact.h"
#include "pf/move.h"
#include "pf/solid.h"

struct iCelEntity;
class csVector3;

/**
 * Factory for move.
 */
class celPfMove : public iCelPropertyClassFactory
{
private:
  iObjectRegistry* object_reg;

public:
  celPfMove (iBase* parent);
  virtual ~celPfMove ();
  bool Initialize (iObjectRegistry* object_reg);

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "pfmove"; }
  virtual iCelPropertyClass* CreatePropertyClass (const char* type);
  virtual int GetTypeCount () const { return 3; }
  virtual const char* GetTypeName (int idx) const;

  struct Component : public iComponent
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPfMove);
    virtual bool Initialize (iObjectRegistry* p)
    { return scfParent->Initialize (p); }
  } scfiComponent;
};

/**
 * This is a move property class.
 */
class celPcMovable : public iCelPropertyClass
{
private:
  iCelEntity* entity;
  iPcMesh* pcmesh;
  iObjectRegistry* object_reg;
  csVector constraints;

public:
  celPcMovable (iObjectRegistry* object_reg);
  virtual ~celPcMovable ();
  void SetMesh (iPcMesh* mesh);
  iPcMesh* GetMesh ();
  int Move (iSector* sector, const csVector3& pos);
  void AddConstraint (iPcMovableConstraint* constraint);
  void RemoveConstraint (iPcMovableConstraint* constraint);
  void RemoveAllConstraints ();

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "pcmovable"; }
  virtual iCelEntity* GetEntity () { return entity; }
  virtual void SetEntity (iCelEntity* entity);

  struct PcMovable : public iPcMovable
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcMovable);
    virtual void SetMesh (iPcMesh* mesh)
    {
      scfParent->SetMesh (mesh);
    }
    virtual iPcMesh* GetMesh ()
    {
      return scfParent->GetMesh ();
    }
    virtual int Move (iSector* sector, const csVector3& pos)
    {
      return scfParent->Move (sector, pos);
    }
    virtual void AddConstraint (iPcMovableConstraint* constraint)
    {
      scfParent->AddConstraint (constraint);
    }
    virtual void RemoveConstraint (iPcMovableConstraint* constraint)
    {
      scfParent->RemoveConstraint (constraint);
    }
    virtual void RemoveAllConstraints ()
    {
      scfParent->RemoveAllConstraints ();
    }
  } scfiPcMovable;
};

/**
 * This is a solid property class.
 */
class celPcSolid : public iCelPropertyClass
{
private:
  iCelEntity* entity;
  iPcMesh* pcmesh;
  iCollider* collider;
  iObjectRegistry* object_reg;

public:
  celPcSolid (iObjectRegistry* object_reg);
  virtual ~celPcSolid ();
  void SetMesh (iPcMesh* mesh);
  iPcMesh* GetMesh () const { return pcmesh; }
  virtual iCollider* GetCollider ();

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "pcsolid"; }
  virtual iCelEntity* GetEntity () { return entity; }
  virtual void SetEntity (iCelEntity* entity);

  struct PcSolid : public iPcSolid
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcSolid);
    virtual void SetMesh (iPcMesh* mesh)
    {
      scfParent->SetMesh (mesh);
    }
    virtual iPcMesh* GetMesh () const
    {
      return scfParent->GetMesh ();
    }
    virtual iCollider* GetCollider ()
    {
      return scfParent->GetCollider ();
    }
  } scfiPcSolid;
};

/**
 * This is a movable constraint based on collision detection (iPcSolid).
 */
class celPcMovableConstraintCD : public iCelPropertyClass
{
private:
  iCelEntity* entity;
  iObjectRegistry* object_reg;

public:
  celPcMovableConstraintCD (iObjectRegistry* object_reg);
  virtual ~celPcMovableConstraintCD ();
  int CheckMove (iSector* sector, const csVector3& start, const csVector3& end, csVector3& pos);

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "pcmovableconst_cd"; }
  virtual iCelEntity* GetEntity () { return entity; }
  virtual void SetEntity (iCelEntity* entity);

  struct PcMovableConstraintCD : public iPcMovableConstraint
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcMovableConstraintCD);
    virtual int CheckMove (iSector* sector, const csVector3& start,
		    const csVector3& end, csVector3& pos)
    {
      return scfParent->CheckMove (sector, start, end, pos);
    }
  } scfiPcMovableConstraint;
};

#endif // __CEL_PF_MOVEFACT__

