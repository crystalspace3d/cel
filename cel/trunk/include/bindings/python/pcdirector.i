/*
 This file has base classes for python pcclasses.
*/

/*
 Handling of errors inside director calls. This code prints any python
 that might happen, and makes cel crash, as it doesnt handle exceptions
 at the moment.
*/
%feature("director:except") {
    if ($error != NULL) {
        PyErr_Print ();
        throw Swig::DirectorMethodException();
    }
}

/* 
 iPcPython interface for python pcclasses 
 It allows to query the python object from the iPropertyClass
 interface.
*/
INTERFACE_PRE(iPcPython)
%inline %{

struct iPcPython : public virtual iBase
{
  SCF_INTERFACE (iPcPython, 0, 0, 1);
  virtual void SetPythonObject(PyObject *py_self) = 0;
  virtual PyObject *GetPythonObject() = 0;
};

%}

INTERFACE_POST(iPcPython)
/*CEL_PC_FIX_INHERITANCE(iPcPython)*/

/* 
 pyPcCommon base class for python pcclasses.
 Basically implements the iPcPython interface, but its very
 probable in the future it might do more.
*/
%template (swigPyPcCommon) scfImplementationExt1<pyPcCommon, PcCommon, iPcPython>;

%warnfilter(473) pyPcCommon;  // avoid warnings on returning pointers from directors.

%feature("director") pyPcCommon;
%feature("nodirector") pyPcCommon::IncRef;
%feature("nodirector") pyPcCommon::DecRef;
%feature("nodirector") pyPcCommon::GetRefCount;
%feature("nodirector") pyPcCommon::AddRefOwner;
%feature("nodirector") pyPcCommon::RemoveRefOwner;
%feature("nodirector") pyPcCommon::QueryInterface;
%feature("nodirector") pyPcCommon::GetInterfaceMetadata;
%feature("nodirector") pyPcCommon::Save;
%feature("nodirector") pyPcCommon::Load;
%feature("nodirector") pyPcCommon::SetEntity;
%feature("nodirector") pyPcCommon::GetPersistentData;
%feature("nodirector") pyPcCommon::PropertyClassesHaveChanged;
%feature("nodirector") pyPcCommon::SaveFirstPass;
%feature("nodirector") pyPcCommon::SetName;
%feature("nodirector") pyPcCommon::GetName;
%feature("nodirector") pyPcCommon::SetTag;
%feature("nodirector") pyPcCommon::GetTag;
%feature("nodirector") pyPcCommon::GetEntity;
%feature("nodirector") pyPcCommon::LoadFirstPass;
%feature("nodirector") pyPcCommon::AddPropertyChangeCallback;
%feature("nodirector") pyPcCommon::RemovePropertyChangeCallback;
%feature("nodirector") pyPcCommon::GetPersistentData;
%feature("nodirector") pyPcCommon::SetPersistentData;
%feature("nodirector") pyPcCommon::SetPythonObject;
%feature("nodirector") pyPcCommon::GetPythonObject;

%inline %{
struct pyPcCommon : public scfImplementationExt1<pyPcCommon, PcCommon, iPcPython>
{
        pyPcCommon(iObjectRegistry* object_reg) : scfImplementationType (this, object_reg) {}
        virtual ~pyPcCommon (){}
        PyObject *pySelf;
        virtual void SetPythonObject(PyObject *py_self)
        {
           pySelf = py_self;
        }
        virtual PyObject *GetPythonObject()
        {
           Py_INCREF(pySelf);
           return pySelf;
        }
};
%}



