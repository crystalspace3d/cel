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
#include "plugins/behaviourlayer/xml/xmlscript.h"

//---------------------------------------------------------------------------

celXmlScriptEventHandler::celXmlScriptEventHandler ()
{
  name = 0;
}

celXmlScriptEventHandler::~celXmlScriptEventHandler ()
{
  delete[] name;
}

//---------------------------------------------------------------------------

celXmlScript::celXmlScript ()
{
  name = 0;
}

celXmlScript::~celXmlScript ()
{
  delete[] name;
}

celXmlScriptEventHandler* celXmlScript::CreateEventHandler (const char* name)
{
  celXmlScriptEventHandler* h = new celXmlScriptEventHandler ();
  h->SetName (name);
  event_handlers.Push (h);
  event_handlers_hash.Put (name, h);
  return h;
}

celXmlScriptEventHandler* celXmlScript::GetEventHandler (const char* name)
{
  celXmlScriptEventHandler* h = event_handlers_hash.Get (name);
  return h;
}

//---------------------------------------------------------------------------

