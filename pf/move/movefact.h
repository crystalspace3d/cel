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
#include "pl/propclas.h"
#include "pl/propfact.h"
#include "pf/move.h"

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
  virtual int GetTypeCount () const { return 1; }
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

public:
  celPcMovable (iObjectRegistry* object_reg);
  virtual ~celPcMovable ();
  void SetMesh (iPcMesh* mesh);
  iPcMesh* GetMesh () const { return pcmesh; }
  int Move (iSector* sector, const csVector3& pos);

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
    virtual iPcMesh* GetMesh () const
    {
      return scfParent->GetMesh ();
    }
    virtual int Move (iSector* sector, const csVector3& pos)
    {
      return scfParent->Move (sector, pos);
    }
  } scfiPcMovable;
};

#endif // __CEL_PF_MOVEFACT__

