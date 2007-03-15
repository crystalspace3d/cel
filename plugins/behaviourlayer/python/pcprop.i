/* Wrap property value as python object */
%extend iPcProperties {
  PyObject *GetPropertyValueIndex(size_t idx)
  {
    PyObject *obj;
    celDataType proptype = self->GetPropertyType(idx);
    switch(proptype)
    {
      case CEL_DATA_FLOAT:
        obj = PyFloat_FromDouble((float)self->GetPropertyFloat(idx));
        break;
      case CEL_DATA_LONG:
        obj = SWIG_From_long((long)self->GetPropertyLong(idx));
        break;
      case CEL_DATA_BOOL:
        obj = SWIG_From_bool((bool)self->GetPropertyBool(idx)); 
        break;
      case CEL_DATA_STRING:
        obj = SWIG_FromCharPtr(self->GetPropertyString(idx)); 
        break;
      case CEL_DATA_VECTOR2:
        {
        csVector2 *res;
        res = new csVector2();
        self->GetPropertyVector(idx,*res);
        obj = SWIG_NewPointerObj((void*)(res), SWIGTYPE_p_csVector3, 1);
        break;
        }
      case CEL_DATA_VECTOR3:
        {
        csVector3 *res;
        res = new csVector3();
        self->GetPropertyVector(idx,*res);
        obj = SWIG_NewPointerObj((void*)(res), SWIGTYPE_p_csVector3, 1);
        break;
        }
      case CEL_DATA_COLOR:
        {
        csColor *res;
        res = new csColor();
        self->GetPropertyColor(idx,*res);
        obj = SWIG_NewPointerObj((void*)(res), SWIGTYPE_p_csColor, 1);
        break;
        }
      case CEL_DATA_PCLASS:
        {
        iCelPropertyClass *res = self->GetPropertyPClass(idx);
        obj = SWIG_NewPointerObj((void*)(res), SWIGTYPE_p_iCelPropertyClass, 0);
        break;
        }
      case CEL_DATA_ENTITY:
        {
        iCelEntity * res = self->GetPropertyEntity(idx);
        obj = SWIG_NewPointerObj((void*)(res), SWIGTYPE_p_iCelEntity, 0);
        break;
        }
      case CEL_DATA_IBASE:
        {
        iBase *res = self->GetPropertyIBase(idx);
        obj = SWIG_NewPointerObj((void*)(res), SWIGTYPE_p_iBase, 0);
        break;
        }
      default:
        Py_INCREF(Py_None);
        obj = Py_None;
    }
    return obj;
  }
}
/* Dict like interface for pcproperties */
%extend iPcProperties {
  size_t __len__() { return self->GetPropertyCount(); }
  void clear() { return self->Clear(); }
  bool __contains__(const char *name) {
    if (self->GetPropertyIndex(name) == -1)
      return false;
    else
      return true;
  }
  %pythoncode %{
  def __delitem__(self,item):
      if isinstance(item,str):
        item = self.GetPropertyIndex(item)
        if item == -1:
          return
      self.ClearProperty(item)
  def __setitem__(self,item,value): 
      if isinstance(item,str):
        return self.SetProperty(item,value) 
      return self.SetPropertyIndex(item,value)
  def __getitem__(self,item):
      if isinstance(item,str):
          item = self.GetPropertyIndex(item)
          if item == -1:
            return None
      return self.GetPropertyValueIndex(item)
%}
}
