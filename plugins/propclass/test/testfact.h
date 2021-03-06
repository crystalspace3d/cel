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

#ifndef __CEL_PF_TESTFACT__
#define __CEL_PF_TESTFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/test.h"

struct iCelEntity;
struct iObjectRegistry;

/**
 * Factory for test.
 */
CEL_DECLARE_FACTORY (Test)

/**
 * This is a test property class.
 */
class celPcTest : public scfImplementationExt1<
	celPcTest, celPcCommon, iPcTest>
{
private:
  // For SendMessage parameters.
  static csStringID id_message;
  celOneParameterBlock* params;

  // For actions.
  enum actionids
  {
    action_print = 0
  };

  // For properties.
  enum propids
  {
    propid_counter = 0,
    propid_max
  };
  static PropertyHolder propinfo;

  // Other fields.
  int counter;
  size_t max;

  csRef<iMessageDispatcher> dispatcher_print;

public:
  celPcTest (iObjectRegistry* object_reg);
  virtual ~celPcTest ();

  virtual void Print (const char* msg);

  virtual bool PerformActionIndexed (int idx,
      iCelParameterBlock* params, celData& ret);

  // Override SetProperty from celPcCommon in order to provide support
  // for the 'max' property.
  virtual bool SetPropertyIndexed (int idx, long b);
  virtual bool GetPropertyIndexed (int, long&);
};

#endif // __CEL_PF_TESTFACT__

