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
#include "iutil/objreg.h"
#include "csutil/debug.h"
#include "plugins/propclass/awswin/awswin.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"

#include <ivaria/reporter.h>
#include <iutil/plugin.h>

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (AwsWin, "pcawswindow")

//---------------------------------------------------------------------------

csStringID celPcAwsWin::action_loaddefinitionfile = csInvalidStringID;
csStringID celPcAwsWin::action_selectdefaultskin = csInvalidStringID;
csStringID celPcAwsWin::action_createwindow = csInvalidStringID;
csStringID celPcAwsWin::action_createsink = csInvalidStringID;
csStringID celPcAwsWin::action_registertrigger = csInvalidStringID;
csStringID celPcAwsWin::action_show = csInvalidStringID;
csStringID celPcAwsWin::action_hide = csInvalidStringID;
csStringID celPcAwsWin::action_raise = csInvalidStringID;
csStringID celPcAwsWin::action_lower = csInvalidStringID;
csStringID celPcAwsWin::id_filename = csInvalidStringID;
csStringID celPcAwsWin::id_name = csInvalidStringID;
csStringID celPcAwsWin::id_trigger = csInvalidStringID;

celPcAwsWin::celPcAwsWin (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  // For actions.
  if (action_loaddefinitionfile == csInvalidStringID)
  {
    action_loaddefinitionfile = pl->FetchStringID ("cel.action.LoadDefinitionFile");
    action_selectdefaultskin = pl->FetchStringID ("cel.action.SelectDefaultSkin");
    action_createwindow = pl->FetchStringID ("cel.action.CreateWindow");
    action_createsink = pl->FetchStringID ("cel.action.CreateSink");
    action_registertrigger = pl->FetchStringID ("cel.action.RegisterTrigger");
    action_show = pl->FetchStringID ("cel.action.Show");
    action_hide = pl->FetchStringID ("cel.action.Hide");
    action_raise = pl->FetchStringID ("cel.action.Raise");
    action_lower = pl->FetchStringID ("cel.action.Lower");
    id_filename = pl->FetchStringID ("cel.parameter.filename");
    id_name = pl->FetchStringID ("cel.parameter.name");
    id_trigger = pl->FetchStringID ("cel.parameter.trigger");
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

bool celPcAwsWin::PerformAction (csStringID actionId,
	iCelParameterBlock* params)
{
  if (actionId == action_loaddefinitionfile)
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
  else if (actionId == action_selectdefaultskin)
  {
    CEL_FETCH_STRING_PAR (name,params,id_name);
    if (!p_name)
      return Report (object_reg,
      	"Missing parameter 'name' for action SelectDefaultSkin!");
    if (!GetAWS ()) return false;
    aws->GetPrefMgr ()->SelectDefaultSkin (name);
    return true;
  }
  else if (actionId == action_createwindow)
  {
    CEL_FETCH_STRING_PAR (name,params,id_name);
    if (!p_name)
      return Report (object_reg,
      	"Missing parameter 'name' for action CreateWindow!");
    if (!CreateWindow (name))
      return false;
    return true;
  }
  else if (actionId == action_createsink)
  {
    CEL_FETCH_STRING_PAR (name,params,id_name);
    if (!p_name)
      return Report (object_reg,
      	"Missing parameter 'name' for action CreateSink!");
    if (!CreateSink (name))
      return false;
    return true;
  }
  else if (actionId == action_registertrigger)
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
  else if (actionId == action_show)
  {
    if (awswindow) awswindow->Show ();
    return true;
  }
  else if (actionId == action_hide)
  {
    if (awswindow) awswindow->Hide ();
    return true;
  }
  else if (actionId == action_raise)
  {
    if (awswindow) awswindow->Raise ();
    return true;
  }
  else if (actionId == action_lower)
  {
    if (awswindow) awswindow->Lower ();
    return true;
  }
  return false;
}

iAws* celPcAwsWin::GetAWS ()
{
  if (!aws)
  {
    aws = CS_QUERY_REGISTRY (object_reg, iAws);
    if (!aws)
    {
      csRef<iPluginManager> plugmgr = CS_QUERY_REGISTRY (object_reg,
      	iPluginManager);
      aws = CS_LOAD_PLUGIN (plugmgr, "crystalspace.window.alternatemanager",
      	iAws);
      if (!aws)
      {
        Report (object_reg, "Couldn't load aws plugin!");
	return 0;
      }
      object_reg->Register (aws, "iAws");
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
  awssink = aws->GetSinkMgr ()->CreateSink ((intptr_t)0);
  aws->GetSinkMgr ()->RegisterSink (name, awssink);
  return awssink;
}

//---------------------------------------------------------------------------

