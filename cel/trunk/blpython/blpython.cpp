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
//#include "Python.h"
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

SCF_IMPLEMENT_FACTORY (celBlPython)

#if 0
  SCF_EXPORT_CLASS_TABLE (blpython)
  SCF_EXPORT_CLASS(celBlPython, "cel.behaviourlayer.python",
    "Crystal Entity Layer Python Behaviour Layer")
  SCF_EXPORT_CLASS_TABLE_END
#endif

celBlPython* celBlPython::shared_instance = 0;

celBlPython::celBlPython (iBase *iParent) :
	object_reg (0)
{
  SCF_CONSTRUCT_IBASE (iParent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiScript);
  shared_instance = this;
}

celBlPython::~celBlPython ()
{
  Py_Finalize ();
  object_reg = 0;
}

/*
extern "C" {
  struct swig_type_info;
  extern swig_type_info * SWIG_TypeQuery (const char *);
  extern PyObject * SWIG_NewPointerObj (void *, swig_type_info *, int own);
  extern char * SWIG_PackData (char *c, void *, int);  
  extern void init_cspace ();
}
*/

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
//  if (!RunText ("sys.path.append('scripts/')")) return false;
#ifdef TOP_SRCDIR
  if (!RunText ("sys.path.append('" TOP_SRCDIR "/scripts/')")) return false;
#endif // TOP_SRCDIR
#ifdef SCRIPTSDIR
  if (!RunText ("sys.path.append('" SCRIPTSDIR "/')")) return false;
#endif // SCRIPTSDIR

  if (!LoadModule ("pdb")) return false;  
  if (!LoadModule ("blcelc")) return false;

  // Store the object registry pointer in 'blcel.object_reg'.
  Store ("blcelc.object_reg_ptr", object_reg, (void*)"_p_iObjectRegistry");
  RunText ("blcelc.object_reg=blcelc.iObjectRegistryPtr(blcelc.object_reg_ptr)");
  csRef<iCelPlLayer> pCELPhysicalLayer (CS_QUERY_REGISTRY( object_reg, iCelPlLayer ));
                                   
  // Store the physical layer pointer in 'blcel.physicallayer'.
  pCELPhysicalLayer->IncRef ();	// Avoid smart pointer release@@@ LEAK
  Store ("blcelc.physicallayer_ptr", pCELPhysicalLayer, (void*)"_p_iCelPlLayer");
  RunText ("blcelc.physicallayer=blcelc.iCelPlLayerPtr(blcelc.physicallayer_ptr)");
  pCELPhysicalLayer->DecRef();

  return true;
}


iCelBehaviour* celBlPython::CreateBehaviour (iCelEntity* entity, const char* name)
{
  PyObject *py_name, *py_module, *py_dict, *py_func, *py_args;
  PyObject *py_entity, *py_object;

  py_module = PyImport_ImportModule((char *)name);
  if (py_module != NULL) 
  {
    py_dict = PyModule_GetDict (py_module);
    py_func = PyDict_GetItemString (py_dict, (char *)name);
    if (py_func && PyCallable_Check(py_func))
    {
      py_args = PyTuple_New(1);
      swig_type_info *ti = SWIG_TypeQuery ("_p_iCelEntity");
      py_entity = SWIG_NewPointerObj(entity, ti, 0);
      PyTuple_SetItem (py_args, 0, py_entity);
      py_object = PyObject_CallObject(py_func, py_args);
      if (!py_object)    
      {
        PyRun_SimpleString ("pdb.pm()");
        return NULL;
      }
    }
    else    
    {
      printf ("Error: object \"%s\" is not callable'\n", name);
      return NULL;
    }
  }
  else
  {
    printf ("Error: failed to load module \"%s\"\n", name);
    PyRun_SimpleString ("pdb.pm()");
    return NULL;
  }

  celPythonBehaviour* bh = new celPythonBehaviour (this,
  	py_entity, py_object, name);

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
  swig_type_info *ti = SWIG_TypeQuery ((char *)tag);
  PyObject *obj = SWIG_NewPointerObj (data, ti, 0);
  char *mod_name = csStrNew (name);
  char * var_name = strrchr (mod_name, '.');
  if(!var_name)
    return false;
  *var_name = 0;
  ++var_name;
  PyObject *module = PyImport_ImportModule(mod_name);
  PyModule_AddObject (module, (char *)var_name, obj);

  delete[] mod_name;

  return true;
}

bool celBlPython::LoadModule (const char *name)
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

celPythonBehaviour::celPythonBehaviour (celBlPython *scripter, PyObject *py_entity,
  	PyObject *py_object, const char *name)
{
  SCF_CONSTRUCT_IBASE (NULL);
  celPythonBehaviour::scripter = scripter;
  celPythonBehaviour::py_entity = py_entity;
  celPythonBehaviour::py_object = py_object;
  celPythonBehaviour::name = csStrNew (name);
  tibase = SWIG_TypeQuery ("_p_iBase");  
}

celPythonBehaviour::~celPythonBehaviour ()
{
  Py_DECREF (py_object);
  Py_DECREF (py_entity);

  delete[] name;
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
  PyObject *pymessage_info = SWIG_NewPointerObj (msg_info, tibase, 0);

  PyObject *method = PyString_FromString (msg_id);

  PyObject *result = PyObject_CallMethodObjArgs (py_object, method, py_entity, pymessage_info, NULL);

  if (!result)
    PyRun_SimpleString ("pdb.pm()");
  else
    Py_DECREF(result);

  Py_DECREF (method);
  Py_DECREF (pymessage_info);
    
  return true;
}


                                     