/*
    Crystal Space Entity Layer
    Copyright (C) 2007 by Jorrit Tyberghein

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
#include "plugins/propclass/tools/bagfact.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/datatype.h"
#include "physicallayer/persist.h"
#include "behaviourlayer/behave.h"
#include "csutil/util.h"
#include "csutil/scanstr.h"
#include "iutil/eventq.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/objreg.h"
#include "iutil/virtclk.h"
#include "ivaria/reporter.h"
#include "ivideo/graph2d.h"
#include "ivideo/graph3d.h"
#include "ivideo/fontserv.h"
#include "ivideo/txtmgr.h"

//---------------------------------------------------------------------------

CEL_IMPLEMENT_FACTORY (Bag, "pctools.bag")

//---------------------------------------------------------------------------

static bool Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (csQueryRegistry<iReporter> (object_reg));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.propclass.bag",
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

csStringID celPcBag::id_value = csInvalidStringID;

PropertyHolder celPcBag::propinfo;

celPcBag::celPcBag (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  if (id_value == csInvalidStringID)
    id_value = pl->FetchStringID ("value");

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.bag.action.");
    AddAction (action_addstring, "AddString");
    AddAction (action_removestring, "RemoveString");
    AddAction (action_clear, "Clear");
    AddAction (action_hasstring, "HasString");
  }
}

celPcBag::~celPcBag ()
{
}

#define BAG_SERIAL 1

csPtr<iCelDataBuffer> celPcBag::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (BAG_SERIAL);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcBag::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != BAG_SERIAL) return false;
  return true;
}

bool celPcBag::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_addstring:
      {
        CEL_FETCH_STRING_PAR (value,params,id_value);
        if (!p_value)
	  return Report (object_reg,
	      "Missing parameter 'value' for action AddString!");
	AddString (value);
      }
      return true;
    case action_removestring:
      {
        CEL_FETCH_STRING_PAR (value,params,id_value);
        if (!p_value)
	  return Report (object_reg,
	      "Missing parameter 'value' for action RemoveString!");
	RemoveString (value);
      }
      return true;
    case action_clear:
      Clear ();
      return true;
    case action_hasstring:
      {
        CEL_FETCH_STRING_PAR (value,params,id_value);
        if (!p_value)
	  return Report (object_reg,
	      "Missing parameter 'value' for action HasString!");
	ret.Set (HasString (value));
      }
      return true;
    default:
      return false;
  }
}

void celPcBag::AddString (const char* str)
{
  bag.Add (str);
}

void celPcBag::RemoveString (const char* str)
{
  bag.Delete (str);
}

void celPcBag::Clear ()
{
  bag.DeleteAll ();
}

bool celPcBag::HasString (const char* str)
{
  return bag.Contains (str);
}

csSet<csString>::GlobalIterator celPcBag::GetIterator ()
{
  return bag.GetIterator ();
}

//---------------------------------------------------------------------------

