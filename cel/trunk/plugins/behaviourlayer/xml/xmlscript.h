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

#ifndef __CEL_BLXML_SCRIPT__
#define __CEL_BLXML_SCRIPT__

#include "cstypes.h"
#include "csutil/scf.h"
#include "csutil/util.h"
#include "csutil/parray.h"
#include "csutil/hashhandlers.h"
#include "csutil/hash.h"

/**
 * An event handler in a script.
 */
class celXmlScriptEventHandler
{
private:
  const char* name;

public:
  celXmlScriptEventHandler ();
  ~celXmlScriptEventHandler ();

  void SetName (const char* n) { delete[] name; name = csStrNew (n); }
  const char* GetName () { return name; }
};

/**
 * A script.
 */
class celXmlScript
{
private:
  char* name;
  csPDelArray<celXmlScriptEventHandler> event_handlers;
  csHash<celXmlScriptEventHandler*,const char*,
  	csConstCharHashKeyHandler> event_handlers_hash;

public:
  celXmlScript ();
  ~celXmlScript ();

  void SetName (const char* n) { delete[] name; name = csStrNew (n); }
  const char* GetName () { return name; }

  celXmlScriptEventHandler* CreateEventHandler (const char* name);
  celXmlScriptEventHandler* GetEventHandler (const char* name);
};

#endif // __CEL_BLXML_SCRIPT__

