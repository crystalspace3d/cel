/*
 This file has base classes for scripting pcclasses.
*/

%template(PcCommonBase) scfImplementation2<celPcCommon, iCelPropertyClass, iCelTimerListener>;
%immutable Property::desc;
%include "celtool/stdpcimp.h"

%warnfilter(473) PcCommonFactory;    // avoid warnings on returning pointers from directors.

%feature("director") PcCommonFactory;
%feature("nodirector") PcCommonFactory::IncRef;
%feature("nodirector") PcCommonFactory::DecRef;
%feature("nodirector") PcCommonFactory::GetRefCount;
%feature("nodirector") PcCommonFactory::AddRefOwner;
%feature("nodirector") PcCommonFactory::RemoveRefOwner;
%feature("nodirector") PcCommonFactory::QueryInterface;
%feature("nodirector") PcCommonFactory::GetInterfaceMetadata;
%feature("nodirector") PcCommonFactory::CreatePropertyClass;

%template (swigPcCommonFactory) scfImplementation1<PcCommonFactory, iCelPropertyClassFactory>;
%template (swigPcCommon) scfImplementationExt0<PcCommon, celPcCommon>;
%include "physicallayer/facttmpl.h"

%inline %{
/*
Base class for scripting pcclasses.
Only for taking care of overwriting save and load for the moment, otherwise
celPcCommon would be good.
*/
struct PcCommon : public scfImplementationExt0<PcCommon, celPcCommon>
{
    PcCommon(iObjectRegistry* object_reg) : scfImplementationType (this, object_reg) {}
    virtual ~PcCommon (){}
    virtual csPtr<iCelDataBuffer> Save()
    {
        return pl->CreateDataBuffer(0);
    }
    virtual bool Load(iCelDataBuffer*)
    {
        PcCommon *x= new PcCommon(object_reg); 
        return false;
    }
};

/*
Base class for scripting pcfactories.
Note the hack on CreatePropertyClass. Swig at the moment doesnt seem to allow
returning csPtr types... In any case client code shouldnt need to care about this, as
all factories do the same basically, and languages can provide a base implementation.
(see pycel.py).
*/
struct PcCommonFactory : public scfImplementation1<PcCommonFactory, iCelPropertyClassFactory>
{
    PcCommonFactory() : scfImplementationType(this) {}
    virtual ~PcCommonFactory() {}
    virtual const char* GetName() const { return ""; };
    virtual csPtr<iCelPropertyClass> CreatePropertyClass (const char* name) { return CreateScriptPropertyClass(name); } ;
    virtual iCelPropertyClass *CreateScriptPropertyClass (const char* name) { return 0; } ;
};

%}
