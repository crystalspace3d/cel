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

//extern "C" {
//  extern void SWIG_MakePtr(char *_c, const void *_ptr, char *type);
//}

extern "C" {
  struct swig_type_info;
  extern swig_type_info * SWIG_TypeQuery(const char *);
  extern PyObject * SWIG_NewPointerObj(void *, swig_type_info *, int own);
  extern char * SWIG_PackData(char *c, void *, int);  
  extern void init_cspace();
}

bool NameObject (char *buf, void* data, const char * tag)
{  
  char *r = buf;
  strcpy(r, "blcelc._");
  r += 8;
  r = SWIG_PackData (r,&data, sizeof(void *));
  strcpy (r, tag);
  return true;
}

bool NameEntity (char *buf, iCelEntity* ent)
{  
  char *r = buf;  
  *(r++) = '_';
  r = SWIG_PackData (r,&ent, sizeof(iCelEntity*));
  strcpy (r, ent->GetName());
  return true;
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
  char cmd[256];

  // @@@ Don't use GetName() but something more unique.

  char objname[100];
  NameObject(objname, entity, "_p_iCelEntity");
  Store(objname, entity, "_p_iCelEntity");

  // Unique
  char entityname[100];
  NameEntity(entityname, entity);

  LoadModule(name);
  sprintf (cmd, "%s=%s.%s(blcelc.iCelEntityPtr(%s))", entityname,
  	name, name, objname);
  
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
    printf("-----------------------------------------------------------\n");
    printf ("Error running text '%s'\n", Text);
    printf("-----------------------------------------------------------\n");
    fflush (stdout);
    //PyRun_SimpleString ("pdb.pm()");
  }  
  ShowError ();
  return worked;
}

bool celBlPython::Store (const char* name, void* data, void* tag)
{
  swig_type_info * ti = SWIG_TypeQuery((char*)tag);
  PyObject * obj = SWIG_NewPointerObj(data, ti, 0);
  char *mod_name = csStrNew(name);
  char * var_name = strrchr(mod_name, '.');
  if(!var_name)
    return false;
  *var_name = 0;
  ++var_name;
  PyObject * module = PyImport_ImportModule(mod_name);
  PyModule_AddObject(module, (char*)var_name, obj);

  delete[] mod_name;

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
  
  char entityname[100];
  char objectname[100];

  NameObject(objectname, entity, name);
  NameEntity(entityname, entity);
  
  ((celBlPython *)scripter)->Store (objectname, entity, "_p_iCelEntity");
    
  entityPtr = csStrNew (objectname);
  entityPythonName = csStrNew (entityname);
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
  if (msg_info)
  {    
    NameObject(msg_info_ptr, msg_info, "_p_iBase");
    ((celBlPython *)scripter)->Store(msg_info_ptr, entity, "_p_iBase");
  }
  else  
    sprintf(msg_info_ptr, "None");
  
  sprintf (buf, "%s.%s(%s,None)", entityPythonName, msg_id, entityPtr, msg_info_ptr);
  scripter->RunText (buf);

  return true;
}


                                     