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
#include "propclass/prop.h"
#include "propclass/billboard.h"
#include "plugins/behaviourlayer/xml/blxml.h"
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

iPcBillboard* celBehaviourXml::GetBillboard ()
{
  if (!billboard)
  {
    csRef<iPcBillboard> b;
    b = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (),
    	iPcBillboard);
    if (!b)
    {
      csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
      iCelPropertyClass* pc = pl->CreatePropertyClass (entity, "pcbillboard");
      if (pc)
	b = SCF_QUERY_INTERFACE (pc, iPcBillboard);
    }
    billboard = b;
  }
  return billboard;
}

iPcProperties* celBehaviourXml::GetProperties ()
{
  if (!props)
  {
    csRef<iPcProperties> p;
    p = CEL_QUERY_PROPCLASS (entity->GetPropertyClassList (),
    	iPcProperties);
    if (!p)
    {
      csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
      iCelPropertyClass* pc = pl->CreatePropertyClass (entity, "pcproperties");
      if (pc)
	p = SCF_QUERY_INTERFACE (pc, iPcProperties);
    }
    props = p;
  }
  return props;
}

celBehaviourXml::~celBehaviourXml ()
{
  delete[] name;
  SCF_DESTRUCT_IBASE ();
}

bool celBehaviourXml::SendMessage (const char* msg_id,
  	iCelPropertyClass* pc,
	celData& ret, iCelParameterBlock* params, ...)
{
  va_list arg;
  va_start (arg, params);
  bool rc = SendMessageV (msg_id, pc, ret, params, arg);
  va_end (arg);
  return rc;
}

bool celBehaviourXml::SendMessageV (const char* msg_id,
  	iCelPropertyClass* /*pc*/,
	celData& ret, iCelParameterBlock* params, va_list arg)
{
  celBlXml* cbl = (celBlXml*)bl;
  celXmlScriptEventHandler* h = script->GetEventHandler (msg_id);
  if (h)
  {
    cbl->call_stack.Push (msg_id);
    cbl->call_stack_params.Push (params);
    cbl->call_stack_entity.Push (entity);
#if DO_PROFILE
    csString msg = script->GetName ();
    msg += '/';
    msg += msg_id;
    int idx = cbl->profile_info_hash.Get ((const char*)msg);
    if (idx == 0)
    {
      idx = cbl->profile_info.Push (celBlXml::celProfileInfo ());
      celBlXml::celProfileInfo& pi = cbl->profile_info[idx];
      pi.msg = csStrNew ((const char*)msg);
      pi.time = 0;
      pi.count = 0;
      cbl->profile_info_hash.Put (pi.msg, idx+1);
    }
    else idx--;
    csTicks start = csGetTicks ();
#endif
    h->Execute (entity, this, ret, params);
#if DO_PROFILE
    csTicks stop = csGetTicks ();
    celBlXml::celProfileInfo& pi = cbl->profile_info[idx];
    pi.time += stop-start;
    pi.count++;
#endif
    cbl->call_stack_entity.Pop ();
    cbl->call_stack_params.Pop ();
    cbl->call_stack.Pop ();
  }
  else
  {
    // Check the superscript if there is one.
    celXmlScript* superscript = script->GetSuperScript ();
    while (superscript)
    {
      h = superscript->GetEventHandler (msg_id);
      if (h)
      {
        cbl->call_stack.Push (msg_id);
	cbl->call_stack_params.Push (params);
	cbl->call_stack_entity.Push (entity);
#if DO_PROFILE
    csString msg = superscript->GetName ();
    msg += '/';
    msg += msg_id;
    int idx = cbl->profile_info_hash.Get ((const char*)msg);
    if (idx == 0)
    {
      idx = cbl->profile_info.Push (celBlXml::celProfileInfo ());
      celBlXml::celProfileInfo& pi = cbl->profile_info[idx];
      pi.msg = csStrNew ((const char*)msg);
      pi.time = 0;
      pi.count = 0;
      cbl->profile_info_hash.Put (pi.msg, idx+1);
    }
    else idx--;
    csTicks start = csGetTicks ();
#endif
	h->Execute (entity, this, ret, params);
#if DO_PROFILE
        csTicks stop = csGetTicks ();
        celBlXml::celProfileInfo& pi = cbl->profile_info[idx];
        pi.time += stop-start;
        pi.count++;
#endif
        cbl->call_stack_entity.Pop ();
        cbl->call_stack_params.Pop ();
	cbl->call_stack.Pop ();
	break;
      }
      else
      {
        superscript = superscript->GetSuperScript ();
      }
    }
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
  SCF_DESTRUCT_IBASE ();
}

bool celBehaviourBootstrap::SendMessage (const char* msg_id,
	iCelPropertyClass* pc,
	celData& ret, iCelParameterBlock* params, ...)
{
  va_list arg;
  va_start (arg, params);
  bool rc = SendMessageV (msg_id, pc, ret, params, arg);
  va_end (arg);
  return rc;
}

bool celBehaviourBootstrap::SendMessageV (const char* msg_id,
	iCelPropertyClass* /*pc*/,
	celData&, iCelParameterBlock* params, va_list arg)
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
    pccamera->SetMode (iPcCamera::firstperson);
    pccamera->SetRegion (pcregion);

    return true;
  }
  return false;
}

//---------------------------------------------------------------------------

