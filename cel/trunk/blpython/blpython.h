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
#include "csutil/csvector.h"
#include "bl/bl.h"
#include "bl/behave.h"

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
  } scfiScript;
};

class celPythonBehaviour : public iCelBehaviour
{
private:
  celBlPython* scripter;
  iCelEntity* entity;
  char* name;
  char* entityPtr;

public:
  celPythonBehaviour (celBlPython* scripter, iCelEntity* entity,
  	const char* name);
  virtual ~celPythonBehaviour ();

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return name; }
  virtual iCelBlLayer* GetBehaviourLayer () const { return scripter; }
  virtual bool SendMessage (const char* msg_id, iBase* msg_info, ...);
  virtual bool SendMessageV (const char* msg_id, iBase* msg_info, va_list arg);
};

void InitPytocel();

#endif // __BLPYTHON_H__

