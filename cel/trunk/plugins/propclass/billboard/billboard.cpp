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

#include "cssysdef.h"
#include "managers/billboard.h"
#include "plugins/propclass/billboard/billboard.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/datatype.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "csutil/util.h"
#include "csutil/debug.h"
#include "iutil/eventq.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/objreg.h"
#include "iutil/virtclk.h"
#include "iutil/plugin.h"
#include "ivideo/graph2d.h"
#include "ivideo/graph3d.h"
#include "ivideo/fontserv.h"
#include "ivideo/txtmgr.h"
#include "ivaria/reporter.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (Billboard, "pcbillboard")

Property* celPcBillboard::properties = 0;
int celPcBillboard::propertycount = 0;

void celPcBillboard::UpdateProperties (iObjectRegistry* object_reg)
{
  if (propertycount == 0)
  {
    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    CS_ASSERT( pl != 0 );

    propertycount = 2;
    properties = new Property[propertycount];

    properties[propid_billboardname].id = pl->FetchStringID (
    	"cel.property.pcbillboard.name");
    properties[propid_billboardname].datatype = CEL_DATA_STRING;
    properties[propid_billboardname].readonly = false;
    properties[propid_billboardname].desc = "Name of billboard.";

    properties[propid_materialname].id = pl->FetchStringID (
    	"cel.property.pcbillboard.materialname");
    properties[propid_materialname].datatype = CEL_DATA_STRING;
    properties[propid_materialname].readonly = false;
    properties[propid_materialname].desc = "Name of material.";
  }
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE_EXT (celPcBillboard)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcBillboard)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcBillboard::PcBillboard)
  SCF_IMPLEMENTS_INTERFACE (iPcBillboard)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcBillboard::celPcBillboard (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcBillboard);
  billboard_name = csStrNew ("default");
  billboard = 0;

  UpdateProperties (object_reg);
  propdata = new void* [propertycount];

  props = properties;
  propcount = &propertycount;

  propdata[propid_billboardname] = &billboard_name;
  propdata[propid_materialname] = 0;	//@@@@@@@
}

celPcBillboard::~celPcBillboard ()
{
  if (billboard_mgr && billboard)
  {
    billboard_mgr->RemoveBillboard (billboard);
  }
  delete[] billboard_name;
}

#define BILLBOARD_SERIAL 1

csPtr<iCelDataBuffer> celPcBillboard::Save ()
{
  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (BILLBOARD_SERIAL);
  databuf->SetDataCount (3);
  databuf->GetData (0)->Set (billboard_name);
  if (billboard)
  {
    databuf->GetData (1)->Set (billboard->GetMaterialName ());
    databuf->GetData (2)->Set ((uint32)billboard->GetFlags ().Get ());
  }
  else
  {
    databuf->GetData (1)->Set ((const char*)0);
    databuf->GetData (2)->Set ((uint32)0);
  }
  return csPtr<iCelDataBuffer> (databuf);
}


bool celPcBillboard::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != BILLBOARD_SERIAL) return false;
  if (databuf->GetDataCount () != 3) return false;
  celData* cd;

  delete[] billboard_name; billboard_name = 0;
  cd = databuf->GetData (0); if (!cd) return false;
  billboard_name = csStrNew (*cd->value.s);

  cd = databuf->GetData (1); if (!cd) return false;
  if (billboard)
    billboard->SetMaterialName (*cd->value.s);

  cd = databuf->GetData (2); if (!cd) return false;
  GetBillboard ();
  if (billboard)
    billboard->GetFlags ().SetAll (cd->value.ul);

  return true;
}

void celPcBillboard::SetBillboardName (const char* n)
{
  delete[] billboard_name;
  billboard_name = csStrNew (n);
}

iBillboard* celPcBillboard::GetBillboard ()
{
  if (billboard) return billboard;
  if (!billboard_mgr)
  {
    billboard_mgr = CS_QUERY_REGISTRY (object_reg, iBillboardManager);
    if (!billboard_mgr)
    {
      csRef<iPluginManager> plugin_mgr = CS_QUERY_REGISTRY (object_reg,
      	iPluginManager);
      billboard_mgr = CS_LOAD_PLUGIN (plugin_mgr, "cel.manager.billboard",
      	iBillboardManager);
      if (!billboard_mgr)
      {
	csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	  "cel.propclass.billboard",
	  "Can't find billboard manager plugin!");
        return 0;
      }
      if (!object_reg->Register (billboard_mgr, "iBillboardManager"))
      {
	csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	  "cel.propclass.billboard",
	  "Couldn't register billboard manager plugin!");
        return 0;
      }
    }
  }
  billboard = billboard_mgr->CreateBillboard (billboard_name);
  if (!billboard)
  {
    csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	  "cel.propclass.billboard",
	  "Couldn't create billboard '%s'!", billboard_name);
    return 0;
  }
  return billboard;
}


//---------------------------------------------------------------------------

