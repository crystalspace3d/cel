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

#include "iutil/eventh.h"
#include "iutil/comp.h"
#include "ivaria/script.h"
#include "csutil/csinput.h"
#include "behaviourlayer/bl.h"
#include "behaviourlayer/behave.h"

#include "Python.h"

extern "C" {
  struct swig_type_info;
  extern swig_type_info * SWIG_TypeQuery (const char *);
  extern PyObject * SWIG_NewPointerObj (void *, swig_type_info *, int own);
  extern char * SWIG_PackData (char *c, void *, int);  
  extern void init_cspace ();
}

class celBlPython : public iCelBlLayer
{
public:  
  celBlPython (iBase *iParent);
  virtual ~celBlPython ();

  static celBlPython* shared_instance;
  iObjectRegistry* object_reg;

  virtual bool Initialize (iObjectRegistry* object_reg);
  virtual const char* GetName () const { return "blpython"; }
  virtual iCelBehaviour* CreateBehaviour (iCelEntity* entity, const char* name);

  bool RunText (const char *Text);
  bool LoadModule (const char *Text);
  bool Store (const char* name, void* data, void* tag);
  
  void ShowError ();
  void Print (bool Error, const char *msg);

  SCF_DECLARE_IBASE;

  // Implement iComponent interface.
  struct eiComponent : public iComponent
  {
    SCF_DECLARE_EMBEDDED_IBASE (celBlPython);
    virtual bool Initialize (iObjectRegistry* p)
    { return scfParent->Initialize (p); }
  } scfiComponent;

  // Implement iScript interface.
  struct eiScript : public iScript
  {
    SCF_DECLARE_EMBEDDED_IBASE (celBlPython);
    virtual bool Initialize (iObjectRegistry *object_reg)
    { return scfParent->Initialize(object_reg); }
    virtual bool RunText (const char *iStr)
    { return scfParent->RunText(iStr); }
    virtual bool LoadModule(const char *iStr)
    { return scfParent->LoadModule(iStr); }
    virtual bool Store(const char *name, void *data, void *tag)
    { return scfParent->Store(name, data, tag); }

    /*
      @@@ New functions not yet implemented
    */
    bool Call(const char *name, const char *fmt, ...)
      { return false; }
    bool Call(const char *name, int &ret, const char *fmt, ...)
      { return false; }
    bool Call(const char *name, float &ret, const char *fmt, ...)
      { return false; }
    bool Call(const char *name, double &ret, const char *fmt, ...)
      { return false; }
    bool Call(const char *name, char **ret, const char *fmt, ...)
      { return false; }
    bool Call(const char *name, void **ret, const char *fmt, ...)
      { return false; }
    bool Call(const char *name, csRef<iScriptObject> &ref, const char *fmt, ...)
      { return false; }
    csPtr<iScriptObject> NewObject(const char *type, const char *fmt, ...) const
      { return 0; }
    bool Store(const char *name, int data)
      { return false; }
    bool Store(const char *name, float data)
      { return false; }
    bool Store(const char *name, double data)
      { return false; }
    bool Store(const char *name, char *data)
      { return false; }
    bool Store(const char *name, void *data, const char *type)
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
    bool GetTruth(const char *name, bool &data) const
      { return false; }
    bool Remove(const char *name)
      { return false; }
  } scfiScript;
};

class celPythonBehaviour : public iCelBehaviour
{
private:
  celBlPython* scripter;
  PyObject* py_entity;
  PyObject* py_object;
  char* name;
  swig_type_info *tibase;
  swig_type_info *tiparams;

public:
  celPythonBehaviour (celBlPython* scripter, PyObject* py_ent,
  	PyObject* py_behaviour, const char* name);
  virtual ~celPythonBehaviour ();

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return name; }
  virtual iCelBlLayer* GetBehaviourLayer () const { return scripter; }
  virtual bool SendMessage (const char* msg_id, celData& ret,
  	iCelParameterBlock* params, ...);
  virtual bool SendMessageV (const char* msg_id, celData& ret,
  	iCelParameterBlock* params, va_list arg);
  virtual void* GetInternalObject () { return (void*)py_object; }
};

void InitPytocel();

#endif // __BLPYTHON_H__

