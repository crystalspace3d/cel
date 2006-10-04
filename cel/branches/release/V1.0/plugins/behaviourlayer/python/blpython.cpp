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

#include <Python.h>
#include <marshal.h>

#include "cssysdef.h"
#include "csutil/sysfunc.h"
#include "plugins/behaviourlayer/python/blpython.h"
#include "csutil/csstring.h"
#include "csutil/util.h"
#include "csutil/syspath.h"
#include "iutil/objreg.h"
#include "iutil/cmdline.h"
#include "iutil/eventq.h"
#include "csutil/event.h"
#include "csutil/eventnames.h"
#include "ivaria/reporter.h"
#include "physicallayer/entity.h"
#include "physicallayer/pl.h"

extern unsigned char pycel_py_wrapper[]; // pycel.py file compiled and marshalled
extern size_t pycel_py_wrapper_size;


CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_IBASE(celBlPython)
  SCF_IMPLEMENTS_INTERFACE(iCelBlLayer)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE(iComponent)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE(iScript)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE(iEventHandler)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celBlPython::eiComponent)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celBlPython::eiScript)
  SCF_IMPLEMENTS_INTERFACE (iScript)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celBlPython::eiEventHandler)
  SCF_IMPLEMENTS_INTERFACE (iEventHandler)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

SCF_IMPLEMENT_FACTORY (celBlPython)

celBlPython* celBlPython::shared_instance = 0;

celBlPython::celBlPython (iBase *iParent) :
	object_reg (0)
{
  SCF_CONSTRUCT_IBASE (iParent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiScript);
  SCF_CONSTRUCT_EMBEDDED_IBASE(scfiEventHandler);
  shared_instance = this;
}

celBlPython::~celBlPython ()
{
  csRef<iEventQueue> queue = CS_QUERY_REGISTRY(object_reg, iEventQueue);
  if (queue.IsValid())
    queue->RemoveListener(&scfiEventHandler);
  Py_Finalize ();
  object_reg = 0;
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiComponent);
  SCF_DESTRUCT_EMBEDDED_IBASE (scfiScript);
  SCF_DESTRUCT_EMBEDDED_IBASE(scfiEventHandler);
  SCF_DESTRUCT_IBASE ();
}

bool celBlPython::Initialize (iObjectRegistry* object_reg)
{
  celBlPython::object_reg = object_reg;

  csRef<iCommandLineParser> cmdline(
    CS_QUERY_REGISTRY(object_reg, iCommandLineParser));
  use_debugger = cmdline->GetOption("python-enable-debugger") != 0;

  Py_SetProgramName ("Crystal Entity Layer -- Python");
  Py_Initialize ();
  InitPytocel ();

  char path[256];
  strncpy (path, csGetConfigPath (), 255);
  strcat (path, "/");

  if (!LoadModule ("sys")) return false;

  csString cmd;
  cmd << "sys.path.append('" << path << "scripts/python/')";
  if (!RunText (cmd)) return false;
  cmd.Clear();
  cmd << "sys.path.append('" << path << "scripts/')";
  if (!RunText (cmd)) return false;
  
  if (!RunText ("sys.path.append('scripts/python/')")) return false;
  if (!RunText ("sys.path.append('scripts/')")) return false;

#ifdef TOP_SRCDIR
  if (!RunText ("sys.path.append('" TOP_SRCDIR "/scripts/')")) return false;
#endif // TOP_SRCDIR
#ifdef SCRIPTSDIR
  if (!RunText ("sys.path.append('" SCRIPTSDIR "/')")) return false;
#endif // SCRIPTSDIR

  if (use_debugger && !LoadModule ("pdb")) return false;
  if (!LoadModule ("cspace")) return false;
  if (!LoadModule ("blcelc")) return false;

  // Store the object registry pointer in 'blcel.object_reg'.
  Store ("blcelc.object_reg_ptr", object_reg, (void *) "iObjectRegistry *");
  // Store the object registry pointer in 'blcel.object_reg'.
  //RunText ("blcelc.object_reg=blcelc.iObjectRegistryPtr(blcelc.object_reg_ptr)");

  csRef<iCelPlLayer> pl = CS_QUERY_REGISTRY (object_reg, iCelPlLayer);
  // Store the physical layer pointer in 'blcel.physicallayer'.
  Store ("blcelc.physicallayer_ptr", pl, (void *) "iCelPlLayer *");
  //RunText ("blcelc.physicallayer=blcelc.iCelPlLayerPtr(blcelc.physicallayer_ptr)");

  // Unmarshall embedded pycel.py
  PyObject* s_mainModule;
  s_mainModule = PyImport_AddModule("__main__");
  if (!s_mainModule)
  {
    printf("Couldn't get __main__ module");
    return false;
  }
  Py_INCREF(s_mainModule);
  printf("Unmarshall pycel.py\n");
  PyObject* pycelModuleCode = PyMarshal_ReadObjectFromString((char*)pycel_py_wrapper, (int)pycel_py_wrapper_size);
  if (!pycelModuleCode)
  {
    printf("Error in embedded pycel.py code\n");
    return false;
  }
  PyObject* pycelModule = PyImport_ExecCodeModule("pycel", pycelModuleCode);
  Py_DECREF(pycelModuleCode); // don't need this at this point
  if (!pycelModule)
  {
    printf("Error compiling embedded pycel.py code\n");
    PyRun_SimpleString ("pdb.pm()");
    return false;
  }
  if (PyModule_AddObject(s_mainModule, "pycel", pycelModule))
  {
    printf("Error adding pycel module to __main__\n");
    return false;
  }
  
  // Register event queue
  csRef<iEventQueue> queue = CS_QUERY_REGISTRY(object_reg, iEventQueue);
  if (queue.IsValid())
    queue->RegisterListener(&scfiEventHandler, csevCommandLineHelp(object_reg));

  return true;
}


iCelBehaviour* celBlPython::CreateBehaviour (iCelEntity* entity,
	const char* name)
{
  PyObject *py_module, *py_dict, *py_func, *py_args;
  PyObject *py_entity, *py_object;

  csString realname;
  char* slash = strrchr (name, '/');
  if (slash)
  {
    csString path;
    path.Append (name, slash-name+1);
    realname = slash+1;

    csString pythonscript = "sys.path.append('";
    pythonscript += path;
    pythonscript += "/')";
    if (!RunText (pythonscript)) return false;
  }
  else
  {
    realname = name;
  }

  py_module = PyImport_ImportModule (const_cast<char*>(realname.GetData()));
  if (py_module != 0) 
  {
    py_dict = PyModule_GetDict (py_module);
    py_func = PyDict_GetItemString (py_dict, realname.GetData ());
    if (py_func && PyCallable_Check(py_func))
    {
      py_args = PyTuple_New(1);
      py_entity = csWrapTypedObject (entity, "_p_iCelEntity", 0);
      PyTuple_SetItem (py_args, 0, py_entity);
      py_object = PyObject_CallObject(py_func, py_args);
      if (!py_object)    
      {
        PyRun_SimpleString ("pdb.pm()");
        return 0;
      }
    }
    else    
    {
      printf ("Error: object \"%s\" is not callable'\n", realname.GetData ());
      return 0;
    }
  }
  else
  {
    printf ("Error: failed to load module \"%s\"\n", realname.GetData ());
    PyRun_SimpleString ("pdb.pm()");
    return 0;
  }

  celPythonBehaviour* bh = new celPythonBehaviour (this,
  	py_entity, py_object, realname.GetData ());
  entity->SetBehaviour (bh);

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
  bool ok = !PyRun_SimpleString (str.GetData ());
  if (!ok) 
  {    
    printf ("Error running text '%s'\n", Text);    
    fflush (stdout);
    if (use_debugger)
      PyRun_SimpleString ("pdb.pm()");
  }  
  ShowError ();
  return ok;
}

bool celBlPython::Store (const char* name, void* data, void* tag)
{
  bool ok = false;
  PyObject *obj = csWrapTypedObject(data, (const char*)tag, 0);
  char *mod_name = csStrNew(name);
  char *var_name = strrchr(mod_name, '.');
  if (var_name)
  {
    *var_name = '\0';
    ++var_name;
    PyObject *module = PyImport_ImportModule(mod_name);
    PyModule_AddObject(module, var_name, obj);
    ok = true;
  }
  delete[] mod_name;
  return ok;
}

bool celBlPython::LoadModule (const char *name)
{
  csString s;
  s << "import " << name;
  return RunText (s);  
}

void celBlPython::Print (bool Error, const char *msg)
{
  csRef<iReporter> rep = CS_QUERY_REGISTRY (object_reg, iReporter);
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

bool celBlPython::HandleEvent(iEvent& e)
{
  bool handled = false;
  if (e.Name == csevCommandLineHelp (object_reg))
  {
#undef indent
#define indent "                     "
    printf("Options for celBlPython plugin:\n"
           "  -python-enable-debugger\n"
           indent "When Python exception is thrown, launch Python debugger\n");
#undef indent
    handled = true;
  }
  return handled;
}

//----------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE(celPythonBehaviour)
  SCF_IMPLEMENTS_INTERFACE(iCelBehaviour)
SCF_IMPLEMENT_IBASE_END

celPythonBehaviour::celPythonBehaviour (celBlPython *scripter,
	PyObject *py_entity, PyObject *py_object, const char *name)
{
  SCF_CONSTRUCT_IBASE (0);
  celPythonBehaviour::scripter = scripter;
  celPythonBehaviour::py_entity = py_entity;
  celPythonBehaviour::py_object = py_object;
  celPythonBehaviour::name = csStrNew (name);
}

celPythonBehaviour::~celPythonBehaviour ()
{
  Py_DECREF (py_object);
  Py_DECREF (py_entity);

  delete[] name;
  SCF_DESTRUCT_IBASE ();
}

bool celPythonBehaviour::SendMessage (const char* msg_id,
	iCelPropertyClass* pc,
	celData& ret, iCelParameterBlock* params, ...)
{
  va_list arg;
  va_start (arg, params);
  bool rc = SendMessageV (msg_id, pc, ret, params, arg);
  va_end (arg);
  return rc;
}

bool celPythonBehaviour::SendMessageV (const char* msg_id,
	iCelPropertyClass* /*pc*/,
	celData& data, iCelParameterBlock* params, va_list arg)
{ 
/*  
  // Ugly but usefull - not need to restart application, only behaviour sending message "reload" :)
  if (!strcmp(msg_id, "reload"))
  {
    PyObject *py_module, *py_dict, *py_func, *py_args;

    py_module = PyImport_ImportModule((char *)name);
    py_module = PyImport_ReloadModule(py_module);
    if (py_module != 0)
    {
      py_dict = PyModule_GetDict (py_module);
      py_func = PyDict_GetItemString (py_dict, (char *)name);
      if (py_func && PyCallable_Check(py_func))
      {
        py_args = PyTuple_New(1);
        PyTuple_SetItem (py_args, 0, py_entity);
        py_object = PyObject_CallObject(py_func, py_args);
        if (!py_object)    
        {
          PyRun_SimpleString ("pdb.pm()");
          return 0;
        }
      }
    }
    return true;
  }
*/
  PyObject *pymessage_info = csWrapTypedObject (params, "_p_iCelParameterBlock", 0);

  PyObject *method = PyString_FromString (msg_id);
  PyObject *result = PyObject_CallMethodObjArgs (py_object, method, 
       py_entity, pymessage_info, 0);
  if (!result)
    PyRun_SimpleString ("pdb.pm()");
  else
  {
    if (PyString_Check(result))
	    data.Set(PyString_AS_STRING(result));
    else if (result == Py_True)
	    data.Set(true);
    else if (result == Py_False)
	    data.Set(false);
    else if (PyFloat_Check(result))
	    data.Set((float)PyFloat_AsDouble(result));
    else if (PyInt_Check(result))
	    data.Set((int32)PyInt_AsLong(result));
    else if (PyLong_Check(result))
	    data.Set((uint32)PyLong_AsUnsignedLong(result));
    else {
	    // XXX how to wrap pointer types??
    }
    Py_DECREF(result);
  }
  Py_DECREF (method);
  Py_DECREF (pymessage_info);
    
  return true;
}
