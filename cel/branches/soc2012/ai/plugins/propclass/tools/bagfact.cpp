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

csStringID celPcBag::id_value = csInvalidStringID;
csStringID celPcBag::id_msgid = csInvalidStringID;

PropertyHolder celPcBag::propinfo;

celPcBag::celPcBag (iObjectRegistry* object_reg)
	: scfImplementationType (this, object_reg)
{
  if (id_value == csInvalidStringID)
  {
    id_value = pl->FetchStringID ("value");
    id_msgid = pl->FetchStringID ("msgid");
  }

  propholder = &propinfo;
  if (!propinfo.actions_done)
  {
    SetActionMask ("cel.bag.action.");
    AddAction (action_addstring, "AddString");
    AddAction (action_removestring, "RemoveString");
    AddAction (action_clear, "Clear");
    AddAction (action_hasstring, "HasString");
    AddAction (action_sendmessage, "SendMessage");
  }

  // For properties.
  propinfo.SetCount (1);
  AddProperty (propid_size, "size",
	CEL_DATA_LONG, true, "Size of the bag.", 0);
}

celPcBag::~celPcBag ()
{
}

bool celPcBag::PerformActionIndexed (int idx,
	iCelParameterBlock* params,
	celData& ret)
{
  switch (idx)
  {
    case action_addstring:
      {
	csString value;
	if (!Fetch (value, params, id_value)) return false;
	AddString (value);
      }
      return true;
    case action_removestring:
      {
	csString value;
	if (!Fetch (value, params, id_value)) return false;
	RemoveString (value);
      }
      return true;
    case action_clear:
      Clear ();
      return true;
    case action_hasstring:
      {
	csString value;
	if (!Fetch (value, params, id_value)) return false;
	ret.Set (HasString (value));
      }
      return true;
    case action_sendmessage:
      {
	csString msgid;
	if (!Fetch (msgid, params, id_msgid)) return false;
	SendMessage (msgid, params);
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

bool celPcBag::SendMessage (const char* msgid, iCelParameterBlock* params)
{
  csSet<csString>::GlobalIterator it = bag.GetIterator ();
  bool total_rc = false;
  while (it.HasNext ())
  {
    csString str = it.Next ();
    iCelEntity* ent = pl->FindEntity (str);
    if (ent)
    {
      bool rc = ent->QueryMessageChannel ()->SendMessage (
		      pl->FetchStringID (msgid), this, params);
      if (rc) total_rc = true;
    }
  }
  return total_rc;
}

bool celPcBag::GetPropertyIndexed (int idx, long& l)
{
  if (idx == propid_size)
  {
    l = bag.GetSize ();
    return true;
  }
  return false;
}

//---------------------------------------------------------------------------

