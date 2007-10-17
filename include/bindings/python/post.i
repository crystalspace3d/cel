%extend iCelPropertyClass {
   %pythoncode %{
   def GetterFallback(self,attr):
        raise AttributeError
   def SetterFallback(self,attr,value):
        raise AttributeError
%}
}

//-----------------------------------------------------------------------------

// extension to create a parameter block from a dict, list or tuple
%extend iCelPlLayer {
	%pythoncode %{
	def CreateParameterBlock(self,valdict):
		"""Create a celGenericParameterBlock from a dict, list or
		tuple"""
		parblock = celGenericParameterBlock(len(valdict))
		for idx,valkey in enumerate(valdict):
			keyid = self.FetchStringID("cel.parameter."+valkey)
			parblock.SetParameterDef (idx,keyid,valkey)
			if type(valdict) == type({}):
				parblock[keyid] = valdict[valkey]
		return parblock	%}
}

%extend iCelPlLayer {
	%pythoncode %{
	def GetEntities(self): return iCelEntityPlFakeArray(self)
	def GetEntityTemplates(self): return iCelEntityTemplatePlFakeArray(self)
	def GetPcFactories(self): return iCelPropertyClassFactoryPlFakeArray(self)
	def GetBehaviourLayers(self): return iCelBlLayerPlFakeArray(self)
	EntityTemplates = _swig_property(GetEntityTemplates)
	PcFactories = _swig_property(GetPcFactories)
	BehaviourLayers = _swig_property(GetBehaviourLayers)
	Entities = _swig_property(GetEntities) %}
}

//-----------------------------------------------------------------------------

//iParameterBlock Extensions
%include "parblock.i"
%extend iCelBehaviour {
  PyObject *GetPythonObject()
  {
    PyObject* obj = (PyObject*)(self->GetInternalObject());
    Py_INCREF (obj);
    return obj;
  }
  PyObject *SendMessage(const char* msg_id, iCelPropertyClass* prop, iCelParameterBlock* params)
  {
       celData ret;
       if(self->SendMessage (msg_id,prop,ret,params))
       {
         CELDATA_RETURN((&ret));
	 return obj;
       }
       else
         Py_INCREF(Py_None);
	 return Py_None;
  }
}

//-----------------------------------------------------------------------------

%extend iPcInventory {
	%pythoncode %{
	def GetEntities(self): return iCelEntityInvFakeArray(self)
	Entities = _swig_property(GetEntities) %}
}

//-----------------------------------------------------------------------------

%include "pcprop.i"

//-----------------------------------------------------------------------------

// helpers to redirect output to cel console

%pythoncode %{
class CelConsoleOut:
	"""Class that can be assigned to sys.stdout or sys.stderr"""
	def __init__(self,oreg):
		self.oreg = oreg
	def write(self,s):
		csQueryRegistry_iCelConsole(self.oreg).GetOutputConsole().PutText(str(s))

class CelConsoleOutOverride:
	"""Class that redirects stdout and stderr to celconsole"""
	def __init__(self,oreg):
		import sys
		self.oldstdout = sys.stdout
		self.oldstderr = sys.stderr
		sys.stdout = CelConsoleOut(oreg)
		sys.stderr = CelConsoleOut(oreg)
	def __del__(self):
		import sys
		sys.stdout = self.oldstdout
		sys.stderr = self.oldstderr
%}
