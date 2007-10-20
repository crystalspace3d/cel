//=============================================================================
// celWrapPtr
//=============================================================================

CS_WRAP_PTR_TYPEMAP(celWrapPtr)

%typemap(out) celWrapPtr
{
  $result = _celWrapPtr_to_Python($1);
}

//=============================================================================
// Helper macros.
//=============================================================================

#undef CEL_PC_FIX_INHERITANCE
%define CEL_PC_FIX_INHERITANCE(pcType)
%extend pcType {
   %pythoncode %{
   _PC = None
   def __getattr__(self,attr):
        if hasattr(iCelPropertyClass,attr):
            _PC = cspace.SCF_QUERY_INTERFACE(self,iCelPropertyClass)
            return getattr(_PC,attr)
        else:
            return self.GetterFallback(attr)
   def __setattr__(self,attr,value):
        if hasattr(pcType,attr):
            return object.__setattr__(self,attr,value)
        elif hasattr(iCelPropertyClass,attr):
            _PC = cspace.SCF_QUERY_INTERFACE(self,iCelPropertyClass)
            setattr(_PC,attr,value)
        else:
            return self.SetterFallback(attr,value)
%}
}
%enddef

//-----------------------------------------------------------------------------
#undef CELLIST_METHODS
%define CELLIST_METHODS(classname,typename)
	PYLIST_BASE_FUNCTIONS(classname,typename*,size_t,GetCount,Get,Add,Remove,Find)
	PYLIST_BYNAME_FUNCTIONS(classname,typename*,FindByName)
%enddef

//-----------------------------------------------------------------------------

// this serves to wrap list methods in classes as a fake array.
#undef CEL_FAKE_ARRAY
%define CEL_FAKE_ARRAY(pref,contenttype,countmethod,getmethod,findmethod,delmethod,addmethod)
%pythoncode %{
class contenttype ## pref ## FakeArray:
	def __init__(self,parent): self.parent = parent
	def __contains__(self,obj):
		if self.parent.findmethod(obj): return True
		else: return False
	def __repr__(self): return "List of "+str(#contenttype)
	def __len__(self): return self.parent.countmethod()
	def __delitem__(self,val):
		if type(val) == type(""):
			obj = self.parent.findmethod(val)
			if obj: return self.parent.delmethod(obj)
			else: raise IndexError(val+" not in list")
		else: return self.parent.delmethod(val)
	def __noappend__(self,obj):
		print "Append not supported by this list"
	def content_iterator(self):
		for idx in xrange(len(self)):
			yield self.parent.getmethod(idx)
	def __iter__(self): return self.content_iterator()
	def __getitem__(self,val):
		if type(val) == type(""): return self.parent.findmethod(val)
		else: return self.parent.getmethod(val)
	def append(self,obj): return self.parent. ## addmethod(obj)
%}
%enddef

%include "datatype.i"

//-----------------------------------------------------------------------------

/* shadow Remove method so pcclass specific interfaces can be used */
%feature("shadow") iCelPropertyClassList::Remove
%{
  def Remove(self,propclass):
    if not (isinstance(propclass,int) or isinstance(propclass,
            iCelPropertyClass)):
      propclass = cspace.SCF_QUERY_INTERFACE(propclass,iCelPropertyClass)
    return _blcelc.iCelPropertyClassList_Remove(self,propclass)
%}

//-----------------------------------------------------------------------------


