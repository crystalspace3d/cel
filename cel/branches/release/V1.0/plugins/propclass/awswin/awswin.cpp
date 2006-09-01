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

#include <cssysdef.h>
#include <cstool/initapp.h>
#include <csutil/debug.h>
#include <csutil/event.h>
#include <iutil/objreg.h>
#include <iutil/plugin.h>
#include <iutil/eventh.h>
#include <iutil/eventq.h>
#include <ivideo/graph3d.h>
#include <ivideo/graph2d.h>
#include <ivaria/reporter.h>

#include "plugins/propclass/awswin/awswin.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (AwsWin, "pcawswindow")

//---------------------------------------------------------------------------

csStringID celPcAwsWin::id_filename = csInvalidStringID;
csStringID celPcAwsWin::id_name = csInvalidStringID;
csStringID celPcAwsWin::id_trigger = csInvalidStringID;

PropertyHolder celPcAwsWin::propinfo;

celPcAwsWin::celPcAwsWin (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  // For actions.
  if (id_filename == csInvalidStringID)
  {
    id_filename = pl->FetchStringID ("cel.parameter.filename");
    id_name = pl->FetchStringID ("cel.parameter.name");
    id_trigger = pl->FetchStringID ("cel.parameter.trigger");
  }

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    AddAction (action_loaddefinitionfile, "cel.action.LoadDefinitionFile");
    AddAction (action_selectdefaultskin, "cel.action.SelectDefaultSkin");
    AddAction (action_createwindow, "cel.action.CreateWindow");
    AddAction (action_createsink, "cel.action.CreateSink");
    AddAction (action_registertrigger, "cel.action.RegisterTrigger");
    AddAction (action_show, "cel.action.Show");
    AddAction (action_hide, "cel.action.Hide");
    AddAction (action_raise, "cel.action.Raise");
    AddAction (action_lower, "cel.action.Lower");
  }

  awssink = 0;
  strset = CS_QUERY_REGISTRY_TAG_INTERFACE (object_reg,
      "crystalspace.shared.stringset", iStringSet);
}

celPcAwsWin::~celPcAwsWin ()
{
  if (awssink && aws)
    aws->GetSinkMgr ()->RemoveSink (awssink);
}

#define AWSWIN_SERIAL 2

csPtr<iCelDataBuffer> celPcAwsWin::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (AWSWIN_SERIAL);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcAwsWin::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != AWSWIN_SERIAL) return false;

  return true;
}

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.propclass.awswin",
    	msg, arg);
  else
  {
    csPrintfV (msg, arg);
    csPrintf ("\n");
    fflush (stdout);
  }

  va_end (arg);
  return false;
}

void celPcAwsWin::Trigger (unsigned long id, iAwsSource* source)
{
  const char* trigname = strset->Request (id);
  if (!trigname) return;
  csString msg = "pcawswin_";
  msg += trigname;
  celData ret;
  iCelBehaviour* bh = entity->GetBehaviour();
  if (bh) bh->SendMessage (msg, this, ret, 0);
}

static void TriggerFunc (unsigned long id, intptr_t sink, iAwsSource* source)
{
  celPcAwsWin* pc = (celPcAwsWin*)sink;
  pc->Trigger (id, source);
}

bool celPcAwsWin::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_loaddefinitionfile:
      {
        CEL_FETCH_STRING_PAR (filename,params,id_filename);
        if (!p_filename)
          return Report (object_reg,
      	    "Missing parameter 'filename' for action LoadDefinitionFile!");
        if (!GetAWS ()) return false;
        if (!aws->GetPrefMgr ()->Load (filename))
          return Report (object_reg, "Couldn't load '%s'!", filename);
        return true;
      }
    case action_selectdefaultskin:
      {
        CEL_FETCH_STRING_PAR (name,params,id_name);
        if (!p_name)
          return Report (object_reg,
      	    "Missing parameter 'name' for action SelectDefaultSkin!");
        if (!GetAWS ()) return false;
        aws->GetPrefMgr ()->SelectDefaultSkin (name);
        return true;
      }
    case action_createwindow:
      {
        CEL_FETCH_STRING_PAR (name,params,id_name);
        if (!p_name)
          return Report (object_reg,
      	    "Missing parameter 'name' for action CreateWindow!");
        if (!CreateWindow (name))
          return false;
        return true;
      }
    case action_createsink:
      {
        CEL_FETCH_STRING_PAR (name,params,id_name);
        if (!p_name)
          return Report (object_reg,
      	    "Missing parameter 'name' for action CreateSink!");
        if (!CreateSink (name))
          return false;
        return true;
      }
    case action_registertrigger:
      {
        CEL_FETCH_STRING_PAR (trigger,params,id_trigger);
        if (!p_trigger)
          return Report (object_reg,
      	    "Missing parameter 'trigger' for action RegisterTrigger!");
        if (!awssink)
          return Report (object_reg,
      	    "Missing sink! Please use CreateSink first!");
        triggers.Add (trigger);
        awssink->RegisterTrigger (trigger, &TriggerFunc);
        return true;
      }
    case action_show:
      if (awswindow) awswindow->Show ();
      return true;
    case action_hide:
      if (awswindow) awswindow->Hide ();
      return true;
    case action_raise:
      if (awswindow) awswindow->Raise ();
      return true;
    case action_lower:
      if (awswindow) awswindow->Lower ();
      return true;
    default:
      return false;
  }
}

class awswinEventHandler : public scfImplementation1<awswinEventHandler,
			    iEventHandler>
{
private:
  iObjectRegistry* object_reg;
  iGraphics3D* g3d;
  iAws* aws;

public:
  awswinEventHandler (iObjectRegistry* object_reg,
  	iGraphics3D* g3d, iAws* aws) :
    scfImplementationType (this), object_reg (object_reg),
    g3d (g3d), aws (aws) { }
  virtual bool HandleEvent (iEvent& ev)
  {
    if (ev.Name == csevPostProcess (object_reg))
    {
      aws->Redraw ();
      aws->Print (g3d, 64);
      return true;
    }
    else
    {
      return aws->HandleEvent (ev);
    }
  }
  CS_EVENTHANDLER_NAMES("cel.propclass.pcawswin")
  CS_EVENTHANDLER_NIL_CONSTRAINTS
};

iAws* celPcAwsWin::GetAWS ()
{
  if (!aws)
  {
    aws = csQueryRegistryOrLoad<iAws> (object_reg,
    	"crystalspace.window.alternatemanager");
    if (!aws)
    {
      Report (object_reg, "Couldn't load aws plugin!");
      return 0;
    }
    else
    {
      csRef<iGraphics3D> g3d = CS_QUERY_REGISTRY (object_reg, iGraphics3D);
      iGraphics2D* g2d = g3d->GetDriver2D ();
      aws->SetupCanvas (0, g2d, g3d);
      csRef<iEventHandler> handler = CS_QUERY_REGISTRY_TAG_INTERFACE (
	  object_reg, "cel.awswindow.eventhandler",
	  iEventHandler);
      if (!handler)
      {
        csRef<awswinEventHandler> lst;
        lst.AttachNew (new awswinEventHandler (object_reg, g3d, aws));
	object_reg->Register ((iEventHandler*)lst,
		"cel.awswindow.eventhandler");
	csRef<iEventQueue> q = CS_QUERY_REGISTRY (object_reg, iEventQueue);
	csEventID esub[] = { 
	  csevKeyboardEvent (object_reg),
	  csevMouseEvent (object_reg),
	  csevPreProcess (object_reg),
	  csevPostProcess (object_reg),
	  csevProcess (object_reg),
	  csevFrame (object_reg),
	  CS_EVENTLIST_END 
 	};
	q->RegisterListener (lst, esub);
      }
    }
  }
  return aws;
}

iAwsWindow* celPcAwsWin::CreateWindow (const char* name)
{
  if (!GetAWS ()) return 0;
  awswindow = aws->CreateWindowFrom (name);
  return awswindow;
}

iAwsSink* celPcAwsWin::CreateSink (const char* name)
{
  if (!GetAWS ()) return 0;
  if (awssink) aws->GetSinkMgr ()->RemoveSink (awssink);
  awssink = aws->GetSinkMgr ()->CreateSink ((intptr_t)this);
  aws->GetSinkMgr ()->RegisterSink (name, awssink);
  return awssink;
}

//---------------------------------------------------------------------------

