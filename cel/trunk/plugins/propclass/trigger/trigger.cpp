/*
    Crystal Space Entity Layer
    Copyright (C) 2004 by Jorrit Tyberghein

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

#include <math.h>
#include "cssysdef.h"
#include "plugins/propclass/trigger/trigger.h"
#include "propclass/camera.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "physicallayer/datatype.h"
#include "physicallayer/databhlp.h"
#include "behaviourlayer/behave.h"
#include "csutil/util.h"
#include "csutil/debug.h"
#include "csutil/flags.h"
#include "iutil/objreg.h"
#include "iutil/object.h"
#include "iutil/vfs.h"
#include "iutil/virtclk.h"
#include "iutil/csinput.h"
#include "iutil/eventq.h"
#include "iutil/event.h"
#include "iutil/evdefs.h"
#include "iutil/string.h"
#include "iutil/stringarray.h"
#include "imap/parser.h"
#include "iengine/engine.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "iengine/camera.h"
#include "iengine/region.h"
#include "iengine/campos.h"
#include "iengine/sector.h"
#include "cstool/csview.h"
#include "ivaria/view.h"
#include "ivideo/graph3d.h"
#include "csqsqrt.h"
#include "ivaria/reporter.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

CEL_IMPLEMENT_FACTORY (Trigger, "pctrigger")

static void Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.persistance",
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

csStringID celPcTrigger::id_entity = csInvalidStringID;

SCF_IMPLEMENT_IBASE_EXT (celPcTrigger)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iPcTrigger)
SCF_IMPLEMENT_IBASE_EXT_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celPcTrigger::PcTrigger)
  SCF_IMPLEMENTS_INTERFACE (iPcTrigger)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celPcTrigger::celPcTrigger (iObjectRegistry* object_reg)
  : celPcCommon (object_reg)
{
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiPcTrigger);
  engine = CS_QUERY_REGISTRY (object_reg, iEngine);

  if (id_entity == csInvalidStringID)
    id_entity = pl->FetchStringID ("cel.parameter.entity");
  params = new celOneParameterBlock ();
  params->SetParameterDef (id_entity, "entity");
}

celPcTrigger::~celPcTrigger ()
{
  delete params;
}

#define TRIGGER_SERIAL 1

csPtr<iCelDataBuffer> celPcTrigger::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (TRIGGER_SERIAL);
  databuf->SetDataCount (0);

  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcTrigger::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != TRIGGER_SERIAL)
  {
    Report (object_reg, "serialnr != TRIGGER_SERIAL.  Cannot load.");
    return false;
  }
  if (databuf->GetDataCount () != 0)
  {
    Report (object_reg, "0 data elements required, not %d.  Cannot load.",
    	databuf->GetDataCount () );
    return false;
  }

  return true;
}

bool celPcTrigger::PerformAction (csStringID /*actionId*/,
	iCelParameterBlock* /*params*/)
{
  return false;
}

void celPcTrigger::SendTriggerMessage (iCelEntity* ent, const char* msgid)
{
  if (ent) params->GetParameter (0).SetIBase (ent);
  celData ret;
  entity->GetBehaviour ()->SendMessage (msgid, ret, params);
}

//---------------------------------------------------------------------------

