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
#include "cssysdef.h"
#include "csgeom/vector3.h"
#include "csgeom/math3d.h"
#include "plugins/propclass/portal/portalfact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "physicallayer/datatype.h"
#include "behaviourlayer/behave.h"
#include "celtool/stdparams.h"
#include "csutil/util.h"
#include "iengine/sector.h"
#include "iengine/portal.h"
#include "iengine/portalcontainer.h"
#include "iengine/engine.h"
#include "iengine/mesh.h"
#include "imesh/object.h"
#include "ivaria/reporter.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (Portal, "pcportal")

static void Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.propclass.portal",
    	msg, arg);
  else
  {
    csPrintfV (msg, arg);
    csPrintf ("\n");
    fflush (stdout);
  }

  va_end (arg);
}

//---------------------------------------------------------------------------

class celPcPortalCallback : public iPortalCallback
{
public:
  celPcPortalCallback ()
  {
    SCF_CONSTRUCT_IBASE (0);
  }
  virtual ~celPcPortalCallback ()
  {
    SCF_DESTRUCT_IBASE ();
  }
  SCF_DECLARE_IBASE;
  virtual bool Traverse (iPortal*, iBase*)
  {
    return false;
  }
};

SCF_IMPLEMENT_IBASE (celPcPortalCallback)
  SCF_IMPLEMENTS_INTERFACE (iPortalCallback)
SCF_IMPLEMENT_IBASE_END

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE_EXT (celPcPortal)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcPortal)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcPortal::PcPortal)
  SCF_IMPLEMENTS_INTERFACE (iPcPortal)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

PropertyHolder celPcPortal::propinfo;

celPcPortal::celPcPortal (iObjectRegistry* object_reg)
	: celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcPortal);
  engine = CS_QUERY_REGISTRY (object_reg, iEngine);

  closed = false;

  // For properties.
  propholder = &propinfo;
  propinfo.SetCount (3);
  AddProperty (propid_mesh, "cel.property.mesh",
	CEL_DATA_STRING, false, "Portal mesh name.", 0);
  AddProperty (propid_portal, "cel.property.portal",
	CEL_DATA_STRING, false, "Portal name.", 0);
  AddProperty (propid_closed, "cel.property.closed",
	CEL_DATA_BOOL, false, "Closed status.", 0);
}

celPcPortal::~celPcPortal ()
{
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiPcPortal);
}

bool celPcPortal::PerformAction (csStringID, iCelParameterBlock*,
    celData& ret)
{
  return false;
}

bool celPcPortal::SetPropertyIndexed (int idx, const char* b)
{
  switch (idx)
  {
    case propid_mesh:
      meshname = b;
      portal = 0;
      return true;
    case propid_portal:
      portalname = b;
      portal = 0;
      return true;
    default:
      return false;
  }
}

bool celPcPortal::GetPropertyIndexed (int idx, const char*& b)
{
  switch (idx)
  {
    case propid_mesh:
      b = (const char*)meshname;
      return true;
    case propid_portal:
      b = (const char*)portalname;
      return true;
    default:
      return false;
  }
}

bool celPcPortal::SetPropertyIndexed (int idx, bool b)
{
  if (idx == propid_closed)
  {
    if (b)
      ClosePortal ();
    else
      OpenPortal ();
    return true;
  }
  return false;
}

bool celPcPortal::GetPropertyIndexed (int idx, bool& b)
{
  if (idx == propid_closed)
  {
    b = closed;
    return true;
  }
  return false;
}

#define PORTAL_SERIAL 2

csPtr<iCelDataBuffer> celPcPortal::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (PORTAL_SERIAL);
  databuf->Add ((const char*)meshname);
  databuf->Add ((const char*)portalname);
  databuf->Add (closed);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcPortal::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != PORTAL_SERIAL)
  {
    Report (object_reg, "Serialnr != PORTAL_SERIAL.  Cannot load.");
    return false;
  }

  portal = 0;
  meshname = databuf->GetString ()->GetData ();
  portalname = databuf->GetString ()->GetData ();
  closed = databuf->GetBool ();

  ResolvePortal ();

  return true;
}

void celPcPortal::ResolvePortal ()
{
  if (!portal)
  {
    iMeshWrapper* m = engine->FindMeshObject (meshname);
    if (!m) return;
    csRef<iPortalContainer> pc = SCF_QUERY_INTERFACE (m->GetMeshObject (),
    	iPortalContainer);
    if (!pc) return;	// @@@ ERROR?
    if (portalname.IsEmpty ())
    {
      portal = pc->GetPortal (0);
    }
    else
    {
      int i;
      for (i = 0 ; i < pc->GetPortalCount () ; i++)
      {
        portal = pc->GetPortal (i);
	if (portalname == portal->GetName ()) break;
      }
    }
    if (portal)
    {
      if (cb)
        portal->RemovePortalCallback (cb);
      if (!cb) cb.AttachNew (new celPcPortalCallback ());
      if (closed)
        portal->SetPortalCallback (cb);
    }
  }
}

bool celPcPortal::SetPortal (const char* mesh, const char* portal)
{
  celPcPortal::portal = 0;
  portalname = portal;
  meshname = mesh;
  return true;
}

void celPcPortal::ClosePortal ()
{
  closed = true;
  ResolvePortal ();
  if (portal)
  {
    portal->RemovePortalCallback (cb);
    portal->SetPortalCallback (cb);
  }
}

void celPcPortal::OpenPortal ()
{
  closed = false;
  ResolvePortal ();
  if (portal)
  {
    portal->RemovePortalCallback (cb);
  }
}

//---------------------------------------------------------------------------

