/*
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

//extern "C" {
#include "Python.h"
//}
#include "cssysdef.h"
#include "cssys/sysfunc.h"
#include "blpython/blpython.h"
#include "csutil/csstring.h"
#include "csutil/util.h"
#include "ivaria/reporter.h"
#include "iutil/objreg.h"
#include "pl/entity.h"
#include "pl/pl.h"

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_IBASE(celBlPython)
  SCF_IMPLEMENTS_INTERFACE(iCelBlLayer)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE(iComponent)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE(iScript)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celBlPython::eiComponent)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celBlPython::eiScript)
  SCF_IMPLEMENTS_INTERFACE (iScript)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_FACTORY(celBlPython)

SCF_EXPORT_CLASS_TABLE(blpython)
  SCF_EXPORT_CLASS(celBlPython, "cel.behaviourlayer.python",
    "Crystal Entity Layer Python Behaviour Layer")
SCF_EXPORT_CLASS_TABLE_END

celBlPython* celBlPython::shared_instance = NULL;

celBlPython::celBlPython (iBase *iParent) :
	object_reg (NULL)
{
  SCF_CONSTRUCT_IBASE (iParent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiScript);
  shared_instance = this;
}

celBlPython::~celBlPython ()
{
  Py_Finalize ();
  object_reg = NULL;
}

extern "C" {
  extern void SWIG_MakePtr(char *_c, const void *_ptr, char *type);
}

bool celBlPython::Initialize (iObjectRegistry* object_reg)
{
  celBlPython::object_reg = object_reg;

  Py_SetProgramName ("Crystal Entity Layer -- Python");
  Py_Initialize ();
  InitPytocel ();

  char path[256];
  char* temp;
  temp = csGetConfigPath ();
  strncpy (path, temp, 255);
  strcat (path, "/");
  delete[] temp;

  if (!LoadModule ("sys")) return false;

  csString cmd;
  cmd << "sys.path.append('" << path << "scripts/python/')";
  if (!RunText (cmd)) return false;
  if (!RunText ("sys.path.append('scripts/')")) return false;
#ifdef TOP_SRCDIR
  if (!RunText ("sys.path.append('" TOP_SRCDIR "/scripts/')")) return false;
#endif // TOP_SRCDIR
#ifdef SCRIPTSDIR
  if (!RunText ("sys.path.append('" SCRIPTSDIR "/')")) return false;
#endif // SCRIPTSDIR
  
  if (!LoadModule ("pdb")) return false;
  if (!LoadModule ("blcelc")) return false;
  if (!LoadModule ("blcel")) return false;

  // Store the object registry pointer in 'blcel.object_reg'.
  Store ("blcel.object_reg_ptr", object_reg, (void*)"_iObjectRegistry_p");
  RunText (
  	"blcel.object_reg=blcel.iObjectRegistryPtr(blcel.object_reg_ptr)");

  csRef<iCelPlLayer> pCELPhysicalLayer (
  	CS_QUERY_REGISTRY( object_reg, iCelPlLayer ));
  // Store the physical layer pointer in 'blcel.physicallayer'.
  pCELPhysicalLayer->IncRef ();	// Avoid smart pointer release@@@ LEAK
  Store ("blcel.physicallayer_ptr", pCELPhysicalLayer, (void*)"_iCelPlLayer_p");
  RunText (
  	"blcel.physicallayer=blcel.iCelPlLayerPtr(blcel.physicallayer_ptr)");
	pCELPhysicalLayer->DecRef();

  return true;
}

iCelBehaviour* celBlPython::CreateBehaviour (iCelEntity* entity,
	const char* name)
{
  char cmd[256];
  char entityPtr[256];
  SWIG_MakePtr (entityPtr, entity, "_iCelEntity_p");
  // @@@ Don't use GetName() but something more unique...
  sprintf (cmd, "%s=%s.%s(blcel.iCelEntityPtr(\"%s\"))", entity->GetName (),
  	name, name, entityPtr);
  LoadModule (name);
  RunText (cmd);
  celPythonBehaviour* bh = new celPythonBehaviour (this,
  	entity, entity->GetName ());
  return bh;
}

void celBlPython::ShowError ()
{
  if (PyErr_Occurred ())
  {
    PyErr_Print ();
    Print (true, "ERROR!");
  }
}

bool celBlPython::RunText (const char* Text)
{
  csString str(Text);
  bool worked = !PyRun_SimpleString (str.GetData ());
  if (!worked) 
  {
    printf ("Error running text '%s'\n", Text);
    fflush (stdout);
    //PyRun_SimpleString ("pdb.pm()");
  }
  ShowError ();
  return worked;
}

bool celBlPython::Store (const char* name, void* data, void* tag)
{
  char command[256];
  char sysPtr[100];
  SWIG_MakePtr (sysPtr, data, (char*)tag);
  sprintf (command, "%s=\"%s\"", name, sysPtr);
  RunText (command);

  return true;
}

bool celBlPython::LoadModule (const char* name)
{
  csString s;
  s << "import " << name;
  return RunText (s);
}

void celBlPython::Print (bool Error, const char *msg)
{
  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (!rep)
  {
    csPrintf ("%s\n", msg);
    fflush (stdout);
  }
  else
  {
    if (Error)
      rep->Report (CS_REPORTER_SEVERITY_ERROR, "cel.behaviourlayer.python",
      	"CrystalScript Error: %s", msg);
    else
      rep->Report (CS_REPORTER_SEVERITY_NOTIFY, "cel.behaviourlayer.python",
      	"%s", msg);
  }
}

//----------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE(celPythonBehaviour)
  SCF_IMPLEMENTS_INTERFACE(iCelBehaviour)
SCF_IMPLEMENT_IBASE_END

celPythonBehaviour::celPythonBehaviour (celBlPython* scripter,
	iCelEntity* entity, const char* name)
{
  SCF_CONSTRUCT_IBASE (NULL);
  celPythonBehaviour::scripter = scripter;
  celPythonBehaviour::entity = entity;
  celPythonBehaviour::name = csStrNew (name);
  char buf[512];
  SWIG_MakePtr (buf, entity, "_iCelEntity_p");
  entityPtr = csStrNew (buf);
}

celPythonBehaviour::~celPythonBehaviour ()
{
  delete[] name;
  delete[] entityPtr;
}

bool celPythonBehaviour::SendMessage (const char* msg_id, iBase* msg_info, ...)
{
  va_list arg;
  va_start (arg, msg_info);
  bool rc = SendMessageV (msg_id, msg_info, arg);
  va_end (arg);
  return rc;
}

bool celPythonBehaviour::SendMessageV (const char* msg_id, iBase* msg_info,
	va_list arg)
{
  (void)arg;
  char buf[512];
  char msg_info_ptr[100];
  SWIG_MakePtr (msg_info_ptr, msg_info, "_iBase_p");
  // @@@ 'name' will be replaced with more unique stuff.
  sprintf (buf, "%s.%s(\"%s\",\"%s\")", name, msg_id, entityPtr, msg_info_ptr);
  scripter->RunText (buf);

  return true;
}


