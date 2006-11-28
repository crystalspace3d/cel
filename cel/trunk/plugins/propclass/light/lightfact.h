/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_LIGHTFACT__
#define __CEL_PF_LIGHTFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "csutil/weakref.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/light.h"

struct iCelEntity;
struct iLight;
struct iEngine;
class csVector3;

/**
 * Factory for light.
 */
CEL_DECLARE_FACTORY (Light)

/**
 * This is a light property class.
 */
class celPcLight : public scfImplementationExt1<
	celPcLight, celPcCommon, iPcLight>
{
private:
  csRef<iLight> light;
  bool created;	// If true we created this light ourselves.
  csWeakRef<iEngine> engine;

  static csStringID id_name;
  static csStringID id_pos;
  static csStringID id_sector;
  static csStringID id_radius;
  static csStringID id_color;
  static csStringID id_entity;
  static csStringID id_tag;

  enum actionids
  {
    action_setlight = 0,
    action_createlight,
    action_changecolor,
    action_movelight,
    action_parentmesh,
    action_clearparent
  };
  static PropertyHolder propinfo;

public:
  celPcLight (iObjectRegistry* object_reg);
  virtual ~celPcLight ();
  virtual bool SetLight (const char* lightname);
  virtual void SetLight (iLight* mesh);
  virtual iLight* CreateLight (const char* lightname,
      iSector* sector, const csVector3& pos,
      float radius, const csColor& color);
  virtual iLight* GetLight () const { return light; }

  virtual const char* GetName () const { return "pclight"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformActionIndexed (int idx, iCelParameterBlock* params,
      celData& ret);
};

#endif // __CEL_PF_LIGHTFACT__

