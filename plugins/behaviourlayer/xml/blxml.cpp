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
#include "iutil/document.h"

#include "plugins/behaviourlayer/xml/blxml.h"
#include "plugins/behaviourlayer/xml/behave_xml.h"
#include "plugins/behaviourlayer/xml/xmlscript.h"

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celBlXml)

SCF_IMPLEMENT_IBASE (celBlXml)
  SCF_IMPLEMENTS_INTERFACE (iCelBlLayer)
  SCF_IMPLEMENTS_INTERFACE (iCelBlLayerGenerate)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celBlXml::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celBlXml::celBlXml (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
}

celBlXml::~celBlXml ()
{
}

bool celBlXml::Initialize (iObjectRegistry* object_reg)
{
  celBlXml::object_reg = object_reg;
  return true;
}

void celBlXml::SetError (const char* error, ...)
{
  va_list arg;
  va_start (arg, error);
  last_error.FormatV (error, arg);
  va_end (arg);
}

const char* celBlXml::ParseEventHandler (celXmlScriptEventHandler* h,
	iDocumentNode* node)
{
  return 0;
}

const char* celBlXml::CreateBehaviourScriptFromDoc (const char* name,
  	iDocumentNode* node)
{
  celXmlScript* script = new celXmlScript ();
  script->SetName (name);

  csRef<iDocumentNodeIterator> it = node->GetNodes ();
  while (it->HasNext ())
  {
    csRef<iDocumentNode> child = it->Next ();
    if (child->GetType () != CS_NODE_ELEMENT) continue;
    const char* value = child->GetValue ();
    if (!strcmp (value, "event"))
    {
      const char* eventname = child->GetAttributeValue ("name");
      if (!eventname)
      {
        delete script;
	SetError ("'name' attribute is missing for the event in script '%s'!",
		name);
        return last_error;
      }
      celXmlScriptEventHandler* h = script->CreateEventHandler (eventname);
      const char* error = ParseEventHandler (h, child);
      if (error)
      {
        delete script;
	return error;
      }
    }
    else
    {
      delete script;
      SetError ("Illegal token '%s' while parsing script '%s'!",
      	value, name);
      return last_error;
    }
  }

  scripts.Push (script);
  scripts_hash.Put (name, script);
  return 0;
}

const char* celBlXml::CreateBehaviourScriptFromString (const char* name,
  	const char* string)
{
  return CEL_BLERROR_UNSUPPORTED;
}

const char* celBlXml::CreateBehaviourScriptFromFile (const char* name,
  	const char* filename)
{
  return CEL_BLERROR_UNSUPPORTED;
}

iCelBehaviour* celBlXml::CreateBehaviour (iCelEntity* entity, const char* name)
{
  celXmlScript* script = scripts_hash.Get (name);
  if (!script)
  {
    // First check if we have the predefined 'bootstrap' script.
    if (!strcmp (name, "bootstrap"))
    {
      celBehaviourBootstrap* bootstrap = new celBehaviourBootstrap (entity,
    	  object_reg);
      bootstrap->SetName (name);
      bootstrap->SetBehaviourLayer (this);
      return bootstrap;
    }
    return 0;	// Couldn't find script.
  }

  celBehaviourXml* behave = new celBehaviourXml (entity, object_reg);
  behave->SetName (name);
  behave->SetBehaviourLayer (this);
  behave->SetScript (script);
  behave->SendMessage ("init", 0);
  return behave;
}


