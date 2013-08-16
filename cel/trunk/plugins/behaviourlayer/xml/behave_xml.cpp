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
#include "csutil/eventnames.h"
#include "iutil/objreg.h"
#include "iutil/eventq.h"
#include "iutil/vfs.h"
#include "csutil/scfstringarray.h"
#include "ivaria/reporter.h"
#include "iengine/engine.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/sector.h"
#include "ivideo/graph3d.h"
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
#include "propclass/rules.h"
#include "propclass/billboard.h"
#include "plugins/behaviourlayer/xml/blxml.h"
#include "plugins/behaviourlayer/xml/behave_xml.h"
#include "plugins/behaviourlayer/xml/xmlscript.h"

//---------------------------------------------------------------------------

csRandomGen celBehaviourXml::rng (1234567);

celBehaviourXml::celBehaviourXml (iCelEntity* entity,
	iObjectRegistry* object_reg) : scfImplementationType (this)
{
  celBehaviourXml::entity = entity;
  celBehaviourXml::object_reg = object_reg;
  pl = csQueryRegistry<iCelPlLayer> (object_reg);
  name = 0;
  script = 0;
  entity->QueryMessageChannel ()->Subscribe (this, "cel.");
}

iPcBillboard* celBehaviourXml::GetBillboard ()
{
  if (!billboard)
  {
    csRef<iPcBillboard> b = celQueryPropertyClassEntity<iPcBillboard> (entity);
    if (!b)
    {
      iCelPropertyClass* pc = pl->CreatePropertyClass (entity, "pcbillboard");
      if (pc)
	b = scfQueryInterface<iPcBillboard> (pc);
    }
    billboard = b;
  }
  return billboard;
}

iPcRules* celBehaviourXml::GetRules ()
{
  if (!rules)
  {
    csRef<iPcRules> p = celQueryPropertyClassEntity<iPcRules> (entity);
    if (!p)
    {
      iCelPropertyClass* pc = pl->CreatePropertyClass (entity, "pcrules");
      if (pc)
	p = scfQueryInterface<iPcRules> (pc);
    }
    rules = p;
  }
  return rules;
}

iPcProperties* celBehaviourXml::GetProperties ()
{
  if (!props)
  {
    csRef<iPcProperties> p;
    p = celQueryPropertyClassEntity<iPcProperties> (entity);
    if (!p)
    {
      iCelPropertyClass* pc = pl->CreatePropertyClass (entity,
	  "pctools.properties");
      if (pc)
	p = scfQueryInterface<iPcProperties> (pc);
    }
    props = p;
  }
  return props;
}

celBehaviourXml::~celBehaviourXml ()
{
  delete[] name;
}

bool celBehaviourXml::ReceiveMessage (csStringID msg_id,
    iMessageSender* /*sender*/, celData& ret, iCelParameterBlock* params)
{
  bool rc = SendMessageV (pl->FetchString (msg_id), 0, ret, params, 0);
  return rc;
}

bool celBehaviourXml::SendMessageV (const char* msg_id,
  	iCelPropertyClass* /*pc*/,
        celData& ret, iCelParameterBlock* params, va_list /*arg*/)
{
  ret.Set ((int32)-1);
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
    h->Execute (entity, cbl, this, ret, params);
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
	h->Execute (entity, cbl, this, ret, params);
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

void celBehaviourXml::Quit ()
{
  csRef<iEventQueue> q = csQueryRegistry<iEventQueue> (object_reg);
  q->GetEventOutlet ()->Broadcast (csevQuit (object_reg));
}

//---------------------------------------------------------------------------

celBehaviourBootstrap::celBehaviourBootstrap (iCelEntity* entity,
	iObjectRegistry* object_reg) : scfImplementationType (this)
{
  celBehaviourBootstrap::entity = entity;
  celBehaviourBootstrap::object_reg = object_reg;
  name = 0;
}

celBehaviourBootstrap::~celBehaviourBootstrap ()
{
  delete[] name;
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
    csRef<iCelPlLayer> pl = csQueryRegistry<iCelPlLayer> (object_reg);
    if (!strcmp (msg_id, "load"))
    {
     csString path,worldname;
     if (params)
     {
       const celData* cd;
       cd = params->GetParameter (pl->FetchStringID ("parameter1"));
       if (cd)
         path = csString(((iString const *)(cd->value.s))->GetData());
       cd = params->GetParameter (pl->FetchStringID ("parameter2"));
       if (cd)
         worldname = csString(((iString const *)(cd->value.s))->GetData());
    }
    printf ("path=%s\n", path.GetData()); fflush (stdout);
    printf ("worldname=%s\n", worldname.GetData()); fflush (stdout);

    pl->LoadPropertyClassFactory ("cel.pcfactory.region");
    pl->LoadPropertyClassFactory ("cel.pcfactory.defaultcamera");
    iCelPropertyClass* pc;

    csRef<iPcCamera> pccamera;
    pc = pl->CreatePropertyClass (entity, "pcdefaultcamera");
    if (!pc)
    {
      csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	  "crystalspace.cel.behaviour.xml",
	  "Couldn't create 'pccamera' property class!");
      return false;
    }
    pccamera = scfQueryInterface<iPcCamera> (pc);
    //pccamera->SetMode (iPcCamera::firstperson);

    csRef<iPcRegion> pcregion;
    pc = pl->CreatePropertyClass (entity, "pcregion");
    if (!pc)
    {
      csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	  "crystalspace.cel.behaviour.xml",
	  "Couldn't create 'pcregion' property class!");
      return false;
    }
    // find path
    csStringArray paths;
    paths.Push ("/lev/");
    csRef<iVFS> VFS = csQueryRegistry<iVFS> (object_reg);
    VFS->PushDir ();
    if (!VFS->ChDirAuto (path.GetData(),&paths,0,worldname.GetData()))
    {
      csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	  "crystalspace.cel.behaviour.xml",
	  "Error setting directory '%s'!",path.GetData());
      VFS->PopDir ();
      return false;
    }
    // get back the path we mounted and get out
    path=VFS->GetCwd();
    VFS->PopDir ();
		
    // load with pcregion
    pcregion = scfQueryInterface<iPcRegion> (pc);
    pcregion->SetWorldFile (path, worldname);
    pcregion->SetRegionName (worldname);
    if (!pcregion->Load ())
    {
      csReport (object_reg, CS_REPORTER_SEVERITY_ERROR,
	  "crystalspace.cel.behaviour.xml",
	  "Couldn't load '%s' at '%s'!", worldname.GetData(), path.GetData());
      return false;
    }
    pccamera->SetRegion (pcregion);

    return true;
  }
  return false;
}

//---------------------------------------------------------------------------

