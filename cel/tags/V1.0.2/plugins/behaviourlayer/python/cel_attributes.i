/* Macros used to generate the right set of accessor methods to transform
 * getters/setters into Python attributes.
 *
 * Class - the class name
 * type - the type of the variable
 * name - the name you wish to use in the python bindings
 * setmethod - the set method in Class
 * getmethod - the get method in Class
 * cel_attribute: Create a getter/setter. Both setmethod and getmethod are
    optional, if none is specified then Get and Set will be prepended to the
    attribute name to find the functions. If only setter is specified then
    the attribute will be read only. If both are specified then theyll be used.
 * cel_multi_attr: For multiple arguments setter/getters, still not tested.
 */

%define %cel_attribute_impl(Class, type, name, getmethod, ...)
#if #__VA_ARGS__ != ""
%feature("shadow") Class::name##_set {
  __swig_setmethods__[#name] = _blcelc.##Class##_##name##_set
  name = property(_blcelc.##Class##_##name##_get, _blcelc.##Class##_##name##_set, None,
                  "Class.name -> type\n\nThis is equivalent to calling the C++ cel methods:\n\tget: type Class::getmethod()\n\tset: void Class::__VA_ARGS__(type)")};
%feature("shadow") Class::name##_get {}
%extend Class
{
    void name##_set(type _val) { self->__VA_ARGS__(_val); }
}
/*%ignore Class::__VA_ARGS__;*/
#else
%feature("shadow") Class::name##_get {
  name = property(_blcelc.##Class##_##name##_get, None, None,
                  "Class.name -> type  (read-only)\n\nThis is equivalent to calling the C++ cel method:\n\tget: type Class::getmethod()")};
#endif
%extend Class
{
    type name##_get() { return (type)(self->getmethod()); }
}
/*%ignore Class::getmethod;*/
%enddef

%define %cel_attribute(Class, type, name, ...)
#if #__VA_ARGS__ != ""
%cel_attribute_impl(Class, type, name, __VA_ARGS__)
#else
%cel_attribute_impl(Class, type, name, Get##name, Set##name)
#endif
%enddef

%define %cel_multi_attr(Class, name, getmethod, setmethod)

%feature("shadow") Class::setmethod {
  name = property(_blcelc.##Class##___##getmethod, fix_args(_blcelc.##Class##___##setmethod), None,
                  "Class.name -> type\n\nThis is equivalent to calling the C++ cel methods:\n\tget: Class::getmethod()\n\tset: void Class::setmethod(...)")};
%feature("shadow") Class::getmethod {}
%rename (__##setmethod) Class::setmethod;
%rename (__##getmethod) Class::getmethod;
%enddef



%define %cel_string(Class, name, value)
%ignore Class::name;
%extend Class
{
    %pythoncode %{ name = value %}
};
%enddef

###########################3
# OLD METHODS
# these should be removed or changed to be like the new ones above

%define GETTER_METHOD(classname,varname,getfunc)
%extend classname {
 %pythoncode %{
  __swig_getmethods__[varname] = _blcelc. ## classname ## _ ## getfunc  %}
}
%enddef

%define SETTER_METHOD(classname,varname,setfunc)
%extend classname {
 %pythoncode %{
  __swig_setmethods__[varname] = _blcelc. ## classname ## _ ## setfunc  %}
}
%enddef

%define GETSET_METHODS2(classname, varname, getfunc, setfunc)
SETTER_METHOD(classname, varname, setfunc)
GETTER_METHOD(classname, varname, getfunc)
%enddef

%define GETSET_METHODS(classname, varname, parname)
SETTER_METHOD(classname, varname, Set ## parname)
GETTER_METHOD(classname, varname, Get ## parname)
%enddef


