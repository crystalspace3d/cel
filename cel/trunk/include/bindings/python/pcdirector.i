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
        //throw Swig::DirectorMethodException();
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

/* 
 pyPcCommon base class for python pcclasses.
 Implements the iPcPython interface, and has c++ base implementations
 for all the actions and properties functions which try to work
 transparently to python.
  - get/set property will access python attributes in the object.
  - performaction will run python methods in the object.
  - note at the moment it is not possible to query the number of
    properties and actions, but might be implemented in the future.
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
%feature("nodirector") pyPcCommon::QueryObject;
%feature("nodirector") pyPcCommon::SetProperty;
%feature("nodirector") pyPcCommon::GetInterfaceMetadata;
%feature("nodirector") pyPcCommon::Save;
%feature("nodirector") pyPcCommon::Load;
%feature("nodirector") pyPcCommon::GetPersistentData;
%feature("nodirector") pyPcCommon::SaveFirstPass;
%feature("nodirector") pyPcCommon::SetName;
%feature("nodirector") pyPcCommon::GetName;
%feature("nodirector") pyPcCommon::SetTag;
%feature("nodirector") pyPcCommon::GetTag;
%feature("nodirector") pyPcCommon::GetEntity;
%feature("nodirector") pyPcCommon::LoadFirstPass;
%feature("nodirector") pyPcCommon::AddPropertyChangeCallback;
%feature("nodirector") pyPcCommon::PropertyClassesHaveChanged;
%feature("nodirector") pyPcCommon::RemovePropertyChangeCallback;
%feature("nodirector") pyPcCommon::GetPersistentData;
%feature("nodirector") pyPcCommon::SetPersistentData;
%feature("nodirector") pyPcCommon::GetPropertyLong;
%feature("nodirector") pyPcCommon::GetPropertyFloat;
%feature("nodirector") pyPcCommon::GetPropertyBool;
%feature("nodirector") pyPcCommon::GetPropertyString;
%feature("nodirector") pyPcCommon::GetPropertyVector;
%feature("nodirector") pyPcCommon::GetPropertyColor;
%feature("nodirector") pyPcCommon::GetPropertyPClass;
%feature("nodirector") pyPcCommon::GetPropertyEntity;
%feature("nodirector") pyPcCommon::GetPropertyIBase;
%feature("nodirector") pyPcCommon::SetPythonObject;
%feature("nodirector") pyPcCommon::GetPythonObject;
%feature("nodirector") pyPcCommon::GetPL;

%inline %{

/****************************************************************
 * Macros for facilitating translation of types inside pyPcCommon */

/*
 * Cast argp1 to ptr_type, and set ret (celData) with the result.
*/
#define SWIG_TO_CELDATA(ptr_type)                       \
{                                                       \
  ptr_type *arg1 = (ptr_type *) 0 ;                     \
  arg1 = reinterpret_cast< ptr_type * >(argp1);         \
  ret.Set(arg1);                                        \
}

/*
 * Get property name from id, check if pySelf has the attribute, and if
 * so get it as py_attr. If it doesnt exist returns NotValue.
*/
#define GET_PROPERTY_BODY(NotValue)                                         \
           csString propname(pl->FetchString(id));                          \
           size_t dot_pos = propname.FindLast('.');                         \
           if(!PyObject_HasAttrString(pySelf,const_cast<char*>(propname.GetData()+dot_pos+1))) \
               return NotValue;                                             \
           PyObject *py_attr = PyObject_GetAttrString(pySelf,               \
                                        const_cast<char*>(propname.GetData()+dot_pos+1));      \
           Py_DECREF(py_attr);

/*
 * Get property name from id, and set the corresponding python attribute
 * with py_value
*/
#define SET_PROPERTY_BODY                                                   \
           csString propname(pl->FetchString(id));                          \
           size_t dot_pos = propname.FindLast('.');                         \
           PyObject_SetAttrString(pySelf,const_cast<char*>(propname.GetData()+dot_pos+1),      \
                                        py_value);                          \
           Py_DECREF(py_value);                                             \
           return true;                                               

/*
 * check if py_obj is of cppType type.
*/
#define CHECK_SWIG_TYPE(py_obj,cppType) SWIG_IsOK(SWIG_ConvertPtr(py_obj, &argp1,SWIGTYPE_p_ ## cppType, 0 ))

/*
 * checks if py_attr is of cppType and if so returns celDataType
*/
#define CHECK_PROPERTY_TYPE(cppType,celDataType)                            \
           if (CHECK_SWIG_TYPE(py_attr,cppType))                            \
             return CEL_DATA_ ## celDataType;

/*
 * Checks if py_attr is of cppType and if so returns a cpp pointer.
*/
#define RETURN_POINTER(cppType)                                             \
           void *argp1 = 0 ;                                                \
           if (CHECK_SWIG_TYPE(py_attr,cppType))                            \
              return reinterpret_cast<cppType * >(argp1);                   \
           return 0;

/*
 * Checks if py_attr is of cppType and if so sets v object.
 * also returns true or false depending on success.
*/
#define SET_REFERENCE_RETURN_BOOL(cppType)                                  \
           void *argp1 = 0 ;                                                \
           if (CHECK_SWIG_TYPE(py_attr,cppType))                            \
           {                                                                \
              cppType *arg1 = (cppType *) 0 ;                               \
              arg1 = reinterpret_cast<cppType * >(argp1);                   \
              v.Set(*arg1);                                                 \
              return true;                                                  \
           }                                                                \
           return false;

/****************************************************************
 * pyPcCommon implementation
*/
class pyPcCommon : public scfImplementationExt1<pyPcCommon, PcCommon, iPcPython>
{
        PyObject *pySelf;
     public:
        iObjectRegistry const * oreg;
        pyPcCommon(iObjectRegistry* object_reg) : scfImplementationType (this, object_reg), pySelf(0), oreg(object_reg)
        {
        }
        virtual ~pyPcCommon (){
          if (pySelf)
            Py_DECREF(pySelf);
        }
        /* iPcPython Implementation */
        virtual void SetPythonObject(PyObject *py_self)
        {
           Py_INCREF(py_self);
           pySelf = py_self;
        }
        virtual PyObject *GetPythonObject()
        {
           Py_INCREF(pySelf);
           return pySelf;
        }

        /* PerformAction Implementation */
        virtual bool PerformAction (csStringID actionID,iCelParameterBlock* params, celData& ret)
        {
           // Get the action name
           csString actname(pl->FetchString(actionID));
           size_t dot_pos = actname.FindLast('.');
           PyObject *py_method = PyString_FromString (actname.GetData()+dot_pos+1);
           // run the method
           PyObject *py_result;
           PyObject *py_params = SWIG_NewPointerObj (params, SWIGTYPE_p_iCelParameterBlock, 0);
           py_result = PyObject_CallMethodObjArgs (pySelf, py_method, py_params,0);
           Py_DECREF (py_params);
           Py_DECREF (py_method);
           // check for errors
           if (!py_result)
           {
             if (PyErr_Occurred ())
               PyErr_Print ();
             return false;
           }
           // fill return celData
           if (py_result == Py_None)
           {
           }
           else if (PyString_Check(py_result))
             ret.Set(PyString_AS_STRING(py_result));
           else if (py_result == Py_True)
             ret.Set(true);
           else if (py_result == Py_False)
             ret.Set(false);
           else if (PyFloat_Check(py_result))
             ret.Set((float)PyFloat_AsDouble(py_result));
           else if (PyInt_Check(py_result))
             ret.Set((int32)PyInt_AsLong(py_result));
           else if (PyLong_Check(py_result))
             ret.Set((uint32)PyLong_AsUnsignedLong(py_result));
           else {
             void *argp1 = 0 ;
             if (CHECK_SWIG_TYPE(py_result,iCelEntity))
               SWIG_TO_CELDATA(iCelEntity)
             else if (CHECK_SWIG_TYPE(py_result,iCelPropertyClass))
               SWIG_TO_CELDATA(iCelPropertyClass)
             else if (CHECK_SWIG_TYPE(py_result,iBase))
               SWIG_TO_CELDATA(iBase)
             else if (CHECK_SWIG_TYPE(py_result,csVector2))
               SWIG_TO_CELDATA(csVector2)
             else if (CHECK_SWIG_TYPE(py_result,csVector3))
               SWIG_TO_CELDATA(csVector3)
             else if (CHECK_SWIG_TYPE(py_result,csVector4))
               SWIG_TO_CELDATA(csVector4)
             else if (CHECK_SWIG_TYPE(py_result,csColor))
               SWIG_TO_CELDATA(csColor)
             else if (CHECK_SWIG_TYPE(py_result,csColor4))
               SWIG_TO_CELDATA(csColor4)
           }
           Py_DECREF(py_result);
           return true;
        }
        /* GetProperty*ByID Implementation */
        virtual long GetPropertyLongByID(csStringID id)
        {
           GET_PROPERTY_BODY(0)
           return PyLong_AsLong(py_attr);
        }
        virtual float GetPropertyFloatByID(csStringID id)
        {
           GET_PROPERTY_BODY(0)
           return PyFloat_AsDouble(py_attr);
        }
        virtual bool GetPropertyBoolByID(csStringID id)
        {
           GET_PROPERTY_BODY(0)
           if (py_attr == Py_False)
             return false;
           else
             return true;
        }
        /* Implement SetEntity to avoid infinite loops calling this
        function from python code (to get the default behaviour) */
        virtual void SetEntity(iCelEntity* entity)
        {
                PcCommon::SetEntity(entity);
        }
        virtual const char* GetPropertyStringByID(csStringID id)
        {
           GET_PROPERTY_BODY(0)
           return PyString_AS_STRING(py_attr);

        }
        virtual bool GetPropertyVectorByID(csStringID id,csVector2& v)
        {
           GET_PROPERTY_BODY(0)
           SET_REFERENCE_RETURN_BOOL(csVector2)
        }
        virtual bool GetPropertyVectorByID(csStringID id,csVector3& v)
        {
           GET_PROPERTY_BODY(0)
           SET_REFERENCE_RETURN_BOOL(csVector3)
        }
        virtual bool GetPropertyColorByID(csStringID id,csColor& v)
        {
           GET_PROPERTY_BODY(0)
           SET_REFERENCE_RETURN_BOOL(csColor)
        }
        virtual iCelEntity *GetPropertyEntityByID(csStringID id)
        {
           GET_PROPERTY_BODY(0)
           RETURN_POINTER(iCelEntity)
        }
        virtual iCelPropertyClass *GetPropertyPClassByID(csStringID id)
        {
           GET_PROPERTY_BODY(0)
           RETURN_POINTER(iCelPropertyClass)
        }
        virtual iBase *GetPropertyIBaseByID(csStringID id)
        {
           GET_PROPERTY_BODY(0)
           RETURN_POINTER(iBase)
        }
        virtual void PropertyClassesHaveChanged () {};

        /* SetProperty(*) Implementation */
        virtual bool SetProperty (csStringID id, long value) {
           PyObject *py_value = PyLong_FromLong(value);
           SET_PROPERTY_BODY
        }
        virtual bool SetProperty (csStringID id, float value) {
           PyObject *py_value = PyFloat_FromDouble((double)value);
           SET_PROPERTY_BODY
        }
        virtual bool SetProperty (csStringID id, bool value) {
           PyObject *py_value;
           if (value)
           {
             py_value = Py_True;
             Py_INCREF(Py_True);
           }
           else
           {
             py_value = Py_False;
             Py_INCREF(Py_False);
           }
           SET_PROPERTY_BODY
        }
        virtual bool SetProperty (csStringID id, const char* value) {
           PyObject *py_value = PyString_FromString(value);
           SET_PROPERTY_BODY
        }
        virtual bool SetProperty (csStringID id, const csVector2& value) {
           csVector2 *res = new csVector2(value);
           PyObject *py_value = SWIG_NewPointerObj (res, SWIGTYPE_p_csVector2, 0);
           SET_PROPERTY_BODY
        }
        virtual bool SetProperty (csStringID id, const csVector3& value) {
           csVector3 *res = new csVector3(value);
           PyObject *py_value = SWIG_NewPointerObj (res, SWIGTYPE_p_csVector3, 0);
           SET_PROPERTY_BODY
        }
        virtual bool SetProperty (csStringID id, const csColor& value) {
           csColor *res = new csColor(value);
           PyObject *py_value = SWIG_NewPointerObj (res, SWIGTYPE_p_csColor, 0);
           SET_PROPERTY_BODY
        }
        virtual bool SetProperty (csStringID id,iCelPropertyClass* value) {
           PyObject *py_value = SWIG_NewPointerObj (value, 
                                          SWIGTYPE_p_iCelPropertyClass, 0);
           SET_PROPERTY_BODY
        }
        virtual bool SetProperty (csStringID id, iCelEntity* entity) {
           PyObject *py_value = SWIG_NewPointerObj (entity, 
                                          SWIGTYPE_p_iCelEntity, 0);
           SET_PROPERTY_BODY
        }
        virtual bool SetProperty (csStringID id, iBase* ibase) {
           PyObject *py_value = SWIG_NewPointerObj (ibase, 
                                          SWIGTYPE_p_iBase, 0);
           SET_PROPERTY_BODY
        }
        virtual size_t GetPropertyAndActionCount () {
           return 0;
        }
        /* GetPropertyOrActionType */
        virtual celDataType GetPropertyOrActionType (csStringID id)
        {
           GET_PROPERTY_BODY(CEL_DATA_NONE)
           if (PyString_Check(py_attr))
             return CEL_DATA_STRING;
           else if (py_attr == Py_True)
             return CEL_DATA_BOOL;
           else if (py_attr == Py_False)
             return CEL_DATA_BOOL;
           else if (PyFloat_Check(py_attr))
             return CEL_DATA_FLOAT;
           else if (PyInt_Check(py_attr))
             return CEL_DATA_LONG;
           else if (PyLong_Check(py_attr))
             return CEL_DATA_LONG;
           void *argp1 = 0 ;
           CHECK_PROPERTY_TYPE(iCelPropertyClass,PCLASS)
           CHECK_PROPERTY_TYPE(csColor,COLOR)
           CHECK_PROPERTY_TYPE(csColor4,COLOR4)
           CHECK_PROPERTY_TYPE(csVector2,VECTOR2)
           CHECK_PROPERTY_TYPE(csVector3,VECTOR3)
           CHECK_PROPERTY_TYPE(csVector4,VECTOR4)
           CHECK_PROPERTY_TYPE(iCelEntity,ENTITY)
           CHECK_PROPERTY_TYPE(iBase,IBASE)
           if (PyCallable_Check(py_attr))
              return CEL_DATA_ACTION;
           return CEL_DATA_NONE;
        }
        /* Other */
        virtual csPtr<iCelPlLayer> GetPL() {
           pl->IncRef();
           return csPtr<iCelPlLayer>(pl); 
        }
};
%}

%extend pyPcCommon
{
  %pythoncode %{
    def PerformActionIndexed(self,id,celParBlock,ret):
      print "indexed action"
    pl = property(GetPL)
%}
}


/*  iQuestSeqOpFactory */
CALLBACK_INTERFACE_HDR(pyMessageSender,iMessageSender)
%inline %{
class pyMessageSender :
      public scfImplementation1<pyMessageSender,iMessageSender>
{
public:
    pyMessageSender(iObjectRegistry* object_reg) : scfImplementationType (this) {}
    ~pyMessageSender() {}
    virtual void MessageDispatcherRemoved (iMessageDispatcher* dispatcher) {};
};
%}

/*  iMessageReceiver */
CALLBACK_INTERFACE_HDR(pyMessageReceiver,iMessageReceiver)
%inline %{
class pyMessageReceiver :
      public scfImplementation1<pyMessageReceiver,iMessageReceiver>
{
public:
    pyMessageReceiver(iObjectRegistry* object_reg) : scfImplementationType (this) {}
~pyMessageReceiver() {}
    virtual bool ReceiveMessage (csStringID msg_id, iMessageSender*  sender,celData& ret, iCelParameterBlock*  params) { return false; }
};
%}

/*  iPcPropertyListener */
CALLBACK_INTERFACE_HDR(pyPcPropertyListener,iPcPropertyListener)
%inline %{
class pyPcPropertyListener :
      public scfImplementation1<pyPcPropertyListener,iPcPropertyListener>
{
public:
    pyPcPropertyListener(iObjectRegistry* object_reg) : scfImplementationType (this) {}
~pyPcPropertyListener() {}
    virtual void PropertyChanged (iPcProperties*  pcprop, size_t idx) { }
};
%}

/*  iPcInventoryListener */
CALLBACK_INTERFACE_HDR(pyPcInventoryListener,iPcInventoryListener)
%inline %{
class pyPcInventoryListener :
      public scfImplementation1<pyPcInventoryListener,iPcInventoryListener>
{
public:
    pyPcInventoryListener(iObjectRegistry* object_reg) : scfImplementationType (this) {}
~pyPcInventoryListener() {}
    virtual void AddChild (iPcInventory*  inventory, iCelEntity*  entity) { }
    virtual void RemoveChild (iPcInventory*  inventory, iCelEntity*  entity) { }
};
%}


