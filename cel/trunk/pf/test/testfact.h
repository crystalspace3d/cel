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

#ifndef __CEL_PF_TESTFACT__
#define __CEL_PF_TESTFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "pl/propclas.h"
#include "pl/propfact.h"
#include "pf/test.h"

struct iCelEntity;
struct iObjectRegistry;

/**
 * Factory for test.
 */
class celPfTest : public iCelPropertyClassFactory
{
private:
  iObjectRegistry* object_reg;

public:
  celPfTest (iBase* parent);
  virtual ~celPfTest ();
  bool Initialize (iObjectRegistry* object_reg);

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "pftest"; }
  virtual iCelPropertyClass* CreatePropertyClass (const char* type);
  virtual int GetTypeCount () const { return 1; }
  virtual const char* GetTypeName (int idx) const { return "pctest"; }

  struct Component : public iComponent
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPfTest);
    virtual bool Initialize (iObjectRegistry* p)
    { return scfParent->Initialize (p); }
  } scfiComponent;
};

/**
 * This is a test property class.
 */
class celPcTest : public iCelPropertyClass
{
private:
  iCelEntity* entity;
  iObjectRegistry* object_reg;

public:
  celPcTest (iObjectRegistry* object_reg);
  virtual ~celPcTest ();

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "pctest"; }
  virtual iCelEntity* GetEntity () { return entity; }
  virtual void SetEntity (iCelEntity* entity);
  virtual iCelDataBuffer* Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  struct PcTest : public iPcTest
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcTest);
    virtual void Print (const char* msg);
  } scfiPcTest;
};

#endif // __CEL_PF_TESTFACT__

