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

#ifdef _MSC_VER
#include <io.h>
#include <stdarg.h>
#ifndef DEBUG_PYTHON
#undef _DEBUG
#define RESTORE__DEBUG
#endif
#endif
#include <Python.h>
#ifdef RESTORE__DEBUG
#define _DEBUG
#undef RESTORE__DEBUG
#endif

#include <marshal.h>

#include "celtool/celpaths.h"

#include "cssysdef.h"
#include "csutil/sysfunc.h"
#include "plugins/behaviourlayer/python/blpython.h"
#include "csutil/csstring.h"
#include "csutil/util.h"
#include "csutil/syspath.h"
#include "iutil/objreg.h"
#include "iutil/cmdline.h"
#include "iutil/eventq.h"
#include "iutil/plugin.h"
#include "iutil/verbositymanager.h"
#include "csutil/event.h"
#include "csutil/eventnames.h"
#include "ivaria/reporter.h"
#include "physicallayer/entity.h"
#include "physicallayer/pl.h"
#include "physicallayer/propclas.h"

extern "C"
{
#if defined(CS_COMPILER_MSVC)
  // MSVC will always use the shipped copy.
  #include "swigpyruntime.h"
#else
  /* *Must* be pointy include. The right file (generated when swig is present,
     shipped copy otherwise) is determined by include paths specified via
     the compiler command line. */
  #include <swigpyruntime.h>
#endif
}


extern const unsigned char pycel_py_wrapper[]; // pycel.py file compiled and marshalled
extern const size_t pycel_py_wrapper_size;

/* Define this to have Python initialized (and finalized) through cspython
 * instead our own code. (This should really be the case as otherwise
 * Py_Initialize() and Py_Finalize() will be called multiple times which is,
 * from observation, Not Good.) */
#define INIT_PY_THROUGH_CSPYTHON

SCF_IMPLEMENT_FACTORY (celBlPython)

celBlPython* celBlPython::shared_instance = 0;

celBlPython::celBlPython (iBase *iParent) :
  scfImplementationType (this, iParent), object_reg (0)
{
  shared_instance = this;
}

celBlPython::~celBlPython ()
{
  csRef<iEventQueue> queue = csQueryRegistry<iEventQueue> (object_reg);
  //@@@ Circular ref: leak
  if (queue.IsValid())
    queue->RemoveListener(this);
#ifndef INIT_PY_THROUGH_CSPYTHON
  Py_Finalize ();
#endif
  object_reg = 0;
}

bool celBlPython::Initialize (iObjectRegistry* object_reg)
{
  celBlPython::object_reg = object_reg;

  csRef<iCommandLineParser> cmdline(
    csQueryRegistry<iCommandLineParser> (object_reg));
  use_debugger = cmdline->GetOption("python-enable-debugger") != 0;

  csRef<iVerbosityManager> verbosity_mgr = 
	  csQueryRegistry<iVerbosityManager> (object_reg);
  do_verbose = verbosity_mgr->Enabled("blpython");
  
  deprecation_warning = true;
  
#ifdef INIT_PY_THROUGH_CSPYTHON
  cspython = csLoadPluginCheck<iScript> (object_reg, 
    "crystalspace.script.python");
#endif

#ifndef INIT_PY_THROUGH_CSPYTHON
  Py_SetProgramName ("Crystal Entity Layer -- Python");
  Py_Initialize ();
  // some parts of python api require sys.argv to be filled.
  // so strange errors will appear if we dont do the following
  char *(argv[2]) = {"", NULL};
  PySys_SetArgv(1, argv);
#endif
  
  InitPytocel ();

  static const char* const _scriptSubDirs[] = {
    "scripts/python/", 
    "scripts/", 
    0};
  csPathsList scriptSubDirs (_scriptSubDirs);
  
  csPathsList scriptsPaths;
    
  csPathsList* cel_paths = CEL::GetPlatformInstallationPaths();
  scriptsPaths.AddUniqueExpanded (*cel_paths * scriptSubDirs);
  delete cel_paths;
  
  csPathsList* cs_paths =
    csInstallationPathsHelper::GetPlatformInstallationPaths();
  scriptsPaths.AddUniqueExpanded (*cs_paths * scriptSubDirs);
  delete cs_paths;
  
#ifndef INIT_PY_THROUGH_CSPYTHON
  if (!LoadModule ("sys")) return false;
#endif

  scriptsPaths.AddUniqueExpanded (csPathsList (".") * scriptSubDirs);
    
#ifdef TOP_SRCDIR
  scriptsPaths.AddUniqueExpanded (TOP_SRCDIR "/scripts/");
#endif // TOP_SRCDIR
#ifdef SCRIPTSDIR
  scriptsPaths.AddUniqueExpanded (SCRIPTSDIR "/scripts/");
#endif // SCRIPTSDIR

  for (size_t i = 0; i < scriptsPaths.GetSize(); i++)
  {
    const char* path = scriptsPaths[i].path;
    if (do_verbose)
      Print (false, csString().Format ("Adding path: %s", path));
    csString cmd;
    cmd << "sys.path.append('" << path << "')";
    if (!RunText (cmd)) return false;
  }

#ifndef INIT_PY_THROUGH_CSPYTHON
  if (use_debugger && !LoadModule ("pdb")) return false;
  if (!LoadModule ("cspace")) return false;
#endif
  if (!LoadModule ("blcelc")) return false;

#ifndef INIT_PY_THROUGH_CSPYTHON
  Store("cspace.__corecvar_iSCF_SCF", iSCF::SCF, (void*)"iSCF *");
  RunText("cspace.SetSCFPointer(cspace.__corecvar_iSCF_SCF)");
#endif
  // Store the object registry pointer in 'blcel.object_reg'.
  Store ("blcelc.object_reg_ptr", object_reg, (void *) "iObjectRegistry *");
  // Store the object registry pointer in 'blcel.object_reg'.
  //RunText ("blcelc.object_reg=blcelc.iObjectRegistryPtr(blcelc.object_reg_ptr)");

  csRef<iCelPlLayer> pl = csQueryRegistry<iCelPlLayer> (object_reg);
  // Store the physical layer pointer in 'blcel.physicallayer'.
  Store ("blcelc.physicallayer_ptr", pl, (void *) "iCelPlLayer *");
  //RunText ("blcelc.physicallayer=blcelc.iCelPlLayerPtr(blcelc.physicallayer_ptr)");

  // Unmarshall embedded pycel.py
  PyObject* s_mainModule;
  s_mainModule = PyImport_AddModule("__main__");
  if (!s_mainModule)
  {
    Print(true,"Couldn't get __main__ module");
    return false;
  }
  Py_INCREF(s_mainModule);
  PyObject* pycelModuleCode = PyMarshal_ReadObjectFromString((char*)pycel_py_wrapper, (int)pycel_py_wrapper_size);
  if (!pycelModuleCode)
  {
    Print(true,"Error in embedded pycel.py code");
    return false;
  }
  PyObject* pycelModule = PyImport_ExecCodeModule(
    const_cast<char*>("pycel"), pycelModuleCode);
  Py_DECREF(pycelModuleCode); // don't need this at this point
  if (!pycelModule)
  {
    Print(true,"Error compiling embedded pycel.py code");
    ShowError();
    return false;
  }
  if (PyModule_AddObject(s_mainModule, "pycel", pycelModule))
  {
    Print(true,"Error adding pycel module to __main__");
    return false;
  }
  // Register event queue
  csRef<iEventQueue> queue = csQueryRegistry<iEventQueue> (object_reg);
  // @@@ Circular ref: leak
  if (queue.IsValid())
    queue->RegisterListener(this, csevCommandLineHelp(object_reg));

  return true;
}

iCelPlLayer* celBlPython::GetPL ()
{
  if (!pl)
    pl = csQueryRegistry<iCelPlLayer> (object_reg);
  return pl;
}

PyObject* csWrapTypedObject(void* objectptr, const char *typetag,
		  int own)
{
    swig_type_info *ti = SWIG_TypeQuery (typetag);
    PyObject *obj = SWIG_NewPointerObj (objectptr, ti, own);
    return obj;
}


iCelBehaviour* celBlPython::CreateBehaviour (iCelEntity* entity,
	const char* name)
{
  PyObject *py_module, *py_dict, *py_func, *py_args;
  PyObject *py_entity, *py_object;

  csString realname;
  const char* slash = strrchr (name, '/');
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
        ShowError ();
        return 0;
      }
    }
    else    
    {
      csPrintf("Error: object \"%s\" is not callable'\n", realname.GetData ());
      return 0;
    }
  }
  else
  {
    csPrintf("Error: failed to load module \"%s\"\n", realname.GetData ());
    ShowError();
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
    if (use_debugger)
    {
      Print (true, "debugger will be launched on command line!");
      PyRun_SimpleString ("pdb.pm()");
    }
  }
}

bool celBlPython::RunText (const char* Text)
{
  csString str(Text);
  bool ok = !PyRun_SimpleString (str.GetData ());
  if (!ok) 
  {    
    csPrintf("Error running text '%s'\n", Text);    
    fflush (stdout);
    ShowError();
  }  
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

bool celBlPython::LoadModule (const char *path, const char *name)
{
  /// @@@ (ge) will fix this later, once I know more.
  return false;
}

void celBlPython::Print (bool Error, const char *msg)
{
  csRef<iReporter> rep = csQueryRegistry<iReporter> (object_reg);
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
    csPrintf("Options for celBlPython plugin:\n"
           "  -python-enable-debugger\n"
           indent "When Python exception is thrown, launch Python debugger\n");
#undef indent
    handled = true;
  }
  return handled;
}

//----------------------------------------------------------------------------

celPythonBehaviour::celPythonBehaviour (celBlPython *scripter,
	PyObject *py_entity, PyObject *py_object, const char *name) :
  scfImplementationType (this)
{
  celPythonBehaviour::scripter = scripter;
  celPythonBehaviour::py_entity = py_entity;
  celPythonBehaviour::py_object = py_object;
  celPythonBehaviour::name = csStrNew (name);

  // check callback interface version
  api_version = 1; // XXX to be changed to 2 in cel 1.4
  if (!PyObject_HasAttrString(py_object, "api_version"))
  {
    if (!scripter->deprecation_warning)
      return;
    csPrintf("DEPRECATION WARNING:\n");
    csPrintf("%s: You should specify behaviour callback version\n",name);
    csPrintf(" Add 'api_version = <number>' to the behaviour class body.\n");
    csPrintf(" Also it is recommended to start using version 2 of the api.\n");
    csPrintf(" Check python section of the cel manual for more information.\n\n");
    scripter->deprecation_warning = false;
    return;
  }
  PyObject *pyapi_version = PyObject_GetAttrString(py_object,"api_version");
  if (PyInt_Check(pyapi_version))
    api_version = PyInt_AsLong(pyapi_version);

  Py_DECREF (pyapi_version);
}

celPythonBehaviour::~celPythonBehaviour ()
{
  Py_DECREF (py_object);
  Py_DECREF (py_entity);

  delete[] name;
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

bool celPythonBehaviour::ReceiveMessage (csStringID msg_id,
    iMessageSender* sender, celData& ret, iCelParameterBlock* params)
{
  // @@@ For now we ignore new messages unless we decide what to do with
  // the 'dots' in them.
  //va_list arg;
  //csRef<iCelPropertyClass> pc = scfQueryInterface<iCelPropertyClass> (
      //sender);
  //bool rc = SendMessageV (scripter->GetPL ()->FetchString (msg_id), pc, ret, params, arg);
  //return rc;
  return false;
}

bool celPythonBehaviour::SendMessageV (const char* msg_id,
	iCelPropertyClass* pc,
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
  PyObject *method = PyString_FromString (msg_id);

  // Check if attribute exists
  if (!PyObject_HasAttr(py_object, method))
  {
    if (scripter->do_verbose)
    {
      csPrintf("%s: behaviour has no method called '%s'\n",name,msg_id);
    }
    Py_DECREF (method);
    return false;
  }

  // Run the method
  PyObject *result;
  PyObject *pymessage_info = csWrapTypedObject (params, "_p_iCelParameterBlock", 0);
  if (api_version == 1)
  {
    result = PyObject_CallMethodObjArgs (py_object, method, 
       py_entity, pymessage_info, 0);
  }
  else
  {
    PyObject *pymessage_pc = csWrapTypedObject (pc, "_p_iCelPropertyClass", 0);
    result = PyObject_CallMethodObjArgs (py_object, method, 
       pymessage_pc, pymessage_info, 0);
    Py_DECREF (pymessage_pc);
  }

  // Check result
  if (!result)
  {
    scripter->ShowError();
    Py_DECREF (method);
    Py_DECREF (pymessage_info);
    return false;
  }
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
