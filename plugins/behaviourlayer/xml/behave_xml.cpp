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
#include "iutil/objreg.h"
#include "ivaria/reporter.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/sector.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/propclas.h"
#include "propclass/mesh.h"
#include "propclass/meshsel.h"
#include "propclass/tooltip.h"
#include "propclass/camera.h"
#include "propclass/inv.h"
#include "propclass/gravity.h"
#include "propclass/timer.h"
#include "propclass/region.h"
#include "plugins/behaviourlayer/xml/behave_xml.h"
#include "plugins/behaviourlayer/xml/xmlscript.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celBehaviourXml)
  SCF_IMPLEMENTS_INTERFACE (iCelBehaviour)
SCF_IMPLEMENT_IBASE_END

celBehaviourXml::celBehaviourXml (iCelEntity* entity,
	iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (0);
  celBehaviourXml::entity = entity;
  celBehaviourXml::object_reg = object_reg;
  name = 0;
  script = 0;
}

celBehaviourXml::~celBehaviourXml ()
{
  delete[] name;
}

bool celBehaviourXml::SendMessage (const char* msg_id, iBase* msg_info, ...)
{
  va_list arg;
  va_start (arg, msg_info);
  bool rc = SendMessageV (msg_id, msg_info, arg);
  va_end (arg);
  return rc;
}

bool celBehaviourXml::SendMessageV (const char* msg_id, iBase* msg_info,
	va_list arg)
{
  celXmlScriptEventHandler* h = script->GetEventHandler (msg_id);
  if (h)
  {
    printf ("Found handler for '%s'\n", msg_id);
    h->ResolveParameters (entity);
    h->Execute (entity);
    fflush (stdout);
  }
  return h != 0;
}

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celBehaviourBootstrap)
  SCF_IMPLEMENTS_INTERFACE (iCelBehaviour)
SCF_IMPLEMENT_IBASE_END

celBehaviourBootstrap::celBehaviourBootstrap (iCelEntity* entity,
	iObjectRegistry* object_reg)
{
  SCF_CONSTRUCT_IBASE (0);
  celBehaviourBootstrap::entity = entity;
  celBehaviourBootstrap::object_reg = object_reg;
  name = 0;
}

celBehaviourBootstrap::~celBehaviourBootstrap ()
{
  delete[] name;
}

bool celBehaviourBootstrap::SendMessage (const char* msg_id,
	iBase* msg_info, ...)
{
  va_list arg;
  va_start (arg, msg_info);
  bool rc = SendMessageV (msg_id, msg_info, arg);
  va_end (arg);
  return rc;
}

bool celBehaviourBootstrap::SendMessageV (const char* msg_id, iBase* msg_info,
	va_list arg)
{
  if (!strcmp (msg_id, "load"))
  {
    const char* path = va_arg (arg, const char*);
    printf ("path=%s\n", path); fflush (stdout);
    const char* worldname = va_arg (arg, const char*);
    printf ("worldname=%s\n", worldname); fflush (stdout);

    csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
    pl->LoadPropertyClassFactory ("cel.pcfactory.region");
    pl->LoadPropertyClassFactory ("cel.pcfactory.camera");
    iCelPropertyClass* pc;

    csRef<iPcRegion> pcregion;
    pc = pl->CreatePropertyClass (entity, "pcregion");
    if (!pc)
    {
      csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	  "crystalspace.cel.behaviour.xml",
	  "Couldn't create 'pcregion' property class!");
      return false;
    }
    pcregion = SCF_QUERY_INTERFACE (pc, iPcRegion);
    pcregion->SetWorldFile (path, worldname);
    pcregion->SetRegionName (worldname);
    if (!pcregion->Load ())
    {
      csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	  "crystalspace.cel.behaviour.xml",
	  "Couldn't load '%s' at '%s'!", worldname, path);
      return false;
    }

    csRef<iPcCamera> pccamera;
    pc = pl->CreatePropertyClass (entity, "pccamera");
    if (!pc)
    {
      csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	  "crystalspace.cel.behaviour.xml",
	  "Couldn't create 'pccamera' property class!");
      return false;
    }
    pccamera = SCF_QUERY_INTERFACE (pc, iPcCamera);
    pccamera->SetRegion (pcregion);

    return true;
  }
  return false;
}

//---------------------------------------------------------------------------

