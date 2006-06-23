/*
    Crystal Space Entity Layer
    Copyright (C) 2001 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_WHEELEDFACT__
#define __CEL_PF_WHEELEDFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/wheeled.h"

struct iCelEntity;
struct iObjectRegistry;

/**
 * Factory for test.
 */
CEL_DECLARE_FACTORY (Wheeled)

/**
 * This is a property class for making and controlling wheeled vehicles, eg bikes, cars, tanks.
 */
class celPcWheeled : public scfImplementationExt1<
	celPcWheeled, celPcCommon, iPcWheeled>
{
private:
  // For SendMessage parameters.
  static csStringID id_message;
  celOneParameterBlock* params;

  // For PerformAction.
  // id_message is shared.
  static csStringID action_print;

  // For properties.
  enum propids
  {
    propid_counter = 0,
    propid_max
  };
  static Property* properties;
  static size_t propertycount;
  static void UpdateProperties (iObjectRegistry* object_reg);

  // Other fields.
  int counter;
  size_t max;

public:
  celPcWheeled (iObjectRegistry* object_reg);
  virtual ~celPcWheeled ();

  virtual void Print (const char* msg);

  virtual const char* GetName () const { return "pcwheeled"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params);

  // Override SetProperty from celPcCommon in order to provide support
  // for the 'max' property.
  virtual bool SetProperty (csStringID, long);
  virtual long GetPropertyLong (csStringID);
};

#endif // __CEL_PF_VEHICLEFACT__

