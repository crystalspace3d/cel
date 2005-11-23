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

#ifndef __CEL_PF_AWSWINFACT__
#define __CEL_PF_AWSWINFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/awswin.h"

#include <csutil/set.h>
#include <iaws/aws.h>

struct iCelEntity;
struct iObjectRegistry;

/**
 * Factory for test.
 */
CEL_DECLARE_FACTORY (AwsWin)

/**
 * This is a test property class.
 */
class celPcAwsWin : public scfImplementationExt1<
	celPcAwsWin, celPcCommon, iPcAwsWindow>
{
private:
  csRef<iAws> aws;
  csRef<iAwsWindow> awswindow;
  iAwsSink* awssink;

  csSet<csString> triggers;

  // For PerformAction.
  static csStringID action_loaddefinitionfile;
  static csStringID action_selectdefaultskin;
  static csStringID action_createwindow;
  static csStringID action_createsink;
  static csStringID action_registertrigger;
  static csStringID action_show;
  static csStringID action_hide;
  static csStringID action_raise;
  static csStringID action_lower;
  static csStringID id_filename;
  static csStringID id_name;
  static csStringID id_trigger;

public:
  celPcAwsWin (iObjectRegistry* object_reg);
  virtual ~celPcAwsWin ();

  virtual iAws* GetAWS ();
  virtual iAwsWindow* CreateWindow (const char* name);
  virtual iAwsSink* CreateSink (const char* name);
  virtual iAwsWindow* GetWindow () { return awswindow; }

  virtual const char* GetName () const { return "pcawswindow"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params);
};

#endif // __CEL_PF_AWSWINFACT__

