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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "cssysdef.h"
#include "csutil/csstring.h"
#include "blpython/blpython.h"

//extern "C" {
#include "Python.h"
//}

// The NextStep compiler does not allow C++ expressions in `extern "C"'
// functions.  This thin cover function works around that limitation.
static inline celBlPython* shared_blpython() { return celBlPython::shared_instance; }

extern "C" PyObject* pytocel_printout(PyObject *self, PyObject* args)
{
  char *command;

  (void)self;
  if (PyArg_ParseTuple (args, "s", &command))
    shared_blpython ()->Print (0, command);
  
  Py_INCREF (Py_None);
  return Py_None;
}

extern "C" PyObject* pytocel_printerr (PyObject *self, PyObject* args)
{
  char *command;

  (void)self;
  if (PyArg_ParseTuple (args, "s", &command))
    shared_blpython ()->Print (1, command);
  
  Py_INCREF (Py_None);
  return Py_None;
}

PyMethodDef PytocelMethods[]={
  {"printout", pytocel_printout, METH_VARARGS, ""},
  {"printerr", pytocel_printerr, METH_VARARGS, ""},
  {NULL, NULL, 0, ""}
};

extern "C" void initblcelc ();

//TODO temporary

void InitPytocel ()
{
  Py_InitModule ("pytocel", PytocelMethods);
  initblcelc ();
//TODO temporary
}

