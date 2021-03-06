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

#ifndef __BLPYTHON_H__
#define __BLPYTHON_H__

#include <Python.h>

#include "csutil/scf_implementation.h"
#include "csutil/weakref.h"
#include "iutil/eventh.h"
#include "iutil/comp.h"
#include "ivaria/script.h"
#include "csutil/csinput.h"
#include "behaviourlayer/bl.h"
#include "behaviourlayer/behave.h"
#include "physicallayer/pl.h"
#include "physicallayer/messaging.h"

extern "C" {
  extern void init_cspace ();
  extern CS_IMPORT_SYM PyObject* csWrapTypedObject (void *, const char *, int own);
}

class celBlPython : public scfImplementation4<celBlPython, iCelBlLayer,
  iComponent, iScript, iEventHandler>
{
public:  
  celBlPython (iBase *iParent);
  virtual ~celBlPython ();

  static celBlPython* shared_instance;
  iObjectRegistry* object_reg;
  csWeakRef<iCelPlLayer> pl;
  csRef<iScript> cspython;
  bool use_debugger;
  bool do_verbose;
  bool deprecation_warning;
  virtual const char* GetName () const { return "blpython"; }
  virtual iCelBehaviour* CreateBehaviour (iCelEntity* entity, const char* name);
  iCelPlLayer* GetPL ();

  bool Store (const char* name, void* data, void* tag);
  void ShowError ();
  void Print (bool Error, const char *msg);

  virtual bool HandleEvent(iEvent&);

  // Implement iComponent interface.
  virtual bool Initialize (iObjectRegistry* p);

  // Implement iScript interface.
  virtual bool RunText (const char *iStr);
  virtual bool LoadModule(const char *iStr);
  virtual bool LoadModule(const char *path, const char *name);

  /*
    @@@ New functions not yet implemented
  */
  bool LoadModuleNative(const char*, const char*) { return false; }
  csPtr<iScriptValue> Call(const char*, const csRefArray<iScriptValue,
      CS::Memory::AllocatorMalloc>&) { return 0 ;}
  csPtr<iScriptValue> RValue(int) { return 0; }
  csPtr<iScriptValue> RValue(float) { return 0; }
  csPtr<iScriptValue> RValue(double) { return 0; }
  csPtr<iScriptValue> RValue(const char*) { return 0; }
  csPtr<iScriptValue> RValue(bool) { return 0; }
  csPtr<iScriptValue> RValue(iScriptObject*) { return 0; }
  csPtr<iScriptObject> New(const char*, const csRefArray<iScriptValue,
      CS::Memory::AllocatorMalloc>&) { return 0; }
  bool Store(const char*, iScriptValue*) { return false; }
  csPtr<iScriptValue> Retrieve(const char*) { return 0; }

  bool Store(const char *name, int data)
      { return false; }
  bool Store(const char *name, float data)
      { return false; }
  bool Store(const char *name, double data)
      { return false; }
  bool Store(const char *name, const char *data)
      { return false; }
  bool Store(const char *name, iScriptObject *data)
      { return false; }
  bool SetTruth(const char *name, bool data)
      { return false; }
  bool Retrieve(const char *name, int &data) const
      { return false; }
  bool Retrieve(const char *name, float &data) const
      { return false; }
  bool Retrieve(const char *name, double &data) const
      { return false; }
  bool Retrieve(const char *name, char **data) const
      { return false; }
  bool Retrieve(const char *name, void **data, const char *type) const
      { return false; }
  bool Retrieve(const char *name, csRef<iScriptObject> &data) const
      { return false; }
  bool Retrieve(const char *name, csRef<iString> &data) const
      { return false; }
  bool GetTruth(const char *name, bool &data) const
      { return false; }
  bool Remove(const char *name)
      { return false; }

  // Implement iEventHandler interface.
  CS_EVENTHANDLER_NAMES("cel.behaviourlayer.python")
  CS_EVENTHANDLER_NIL_CONSTRAINTS
};

class celPythonBehaviour : public scfImplementation2<
	celPythonBehaviour, iCelBehaviour, iMessageReceiver>
{
private:
  celBlPython* scripter;
  PyObject* py_entity;
  PyObject* py_object;
  char* name;
  int api_version;

public:
  celPythonBehaviour (celBlPython* scripter, PyObject* py_ent,
  	PyObject* py_behaviour, const char* name);
  virtual ~celPythonBehaviour ();

  virtual const char* GetName () const { return name; }
  virtual iCelBlLayer* GetBehaviourLayer () const { return scripter; }
  virtual bool SendMessageV (const char* msg_id,
  	iCelPropertyClass* pc, celData& ret,
  	iCelParameterBlock* params, va_list arg);
  virtual void* GetInternalObject () { return (void*)py_object; }

  //--- For iMessageReceiver -----------------------------------------------
  virtual bool ReceiveMessage (csStringID msg_id, iMessageSender* sender,
      celData& ret, iCelParameterBlock* params);
};

void InitPytocel();

#endif // __BLPYTHON_H__

