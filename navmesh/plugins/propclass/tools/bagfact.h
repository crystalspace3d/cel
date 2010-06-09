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

#ifndef __CEL_PF_BAGFACT__
#define __CEL_PF_BAGFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "csutil/weakref.h"
#include "csutil/parray.h"
#include "csutil/weakref.h"
#include "csutil/hash.h"
#include "csutil/csstring.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/bag.h"

struct iCelEntity;
struct iCelPlLayer;
struct iObjectRegistry;
struct iVirtualClock;
struct iGraphics2D;
struct iGraphics3D;
struct iFont;
class csStringArray;

/**
 * Factory for tools.
 */
CEL_DECLARE_FACTORY(Bag)

/**
 * This is a bag property class.
 */
class celPcBag : public scfImplementationExt1<
		 celPcBag, celPcCommon, iPcBag>
{
private:
  csSet<csString> bag;

  static csStringID id_value;
  static csStringID id_msgid;

  enum actionids
  {
    action_addstring = 0,
    action_removestring,
    action_clear,
    action_hasstring,
    action_sendmessage
  };

  // For properties.
  enum propids
  {
    propid_size = 0
  };

  static PropertyHolder propinfo;

public:
  celPcBag (iObjectRegistry* object_reg);
  virtual ~celPcBag ();

  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformActionIndexed (int idx, iCelParameterBlock* params,
  	celData& ret);

  virtual void AddString (const char* str);
  virtual void RemoveString (const char* str);
  virtual void Clear ();
  virtual bool HasString (const char* str);
  virtual csSet<csString>::GlobalIterator GetIterator ();
  virtual bool SendMessage (const char* msgid, iCelParameterBlock* params = 0);

  virtual bool GetPropertyIndexed (int, long&);
};

#endif // __CEL_PF_BAGFACT__
