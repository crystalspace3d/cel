

#define CEL_DECLARE_FACTORY(name)					    \
class celPf##name : public iCelPropertyClassFactory			    \
{									    \
private:								    \
  iObjectRegistry* object_reg;						    \
									    \
public:									    \
  celPf##name (iBase* parent);						    \
  virtual ~celPf##name ();						    \
  bool Initialize (iObjectRegistry* object_reg);			    \
  SCF_DECLARE_IBASE;							    \
  virtual const char* GetName() const;					    \
  virtual iCelPropertyClass* CreatePropertyClass ();			    \
									    \
  struct Component : public iComponent					    \
  {									    \
    SCF_DECLARE_EMBEDDED_IBASE (celPf##name);				    \
    virtual bool Initialize (iObjectRegistry* p);			    \
  } scfiComponent;							    \
};

#define CEL_IMPLEMENT_FACTORY(name, strname)				    \
SCF_IMPLEMENT_FACTORY (celPf##name)					    \
SCF_IMPLEMENT_IBASE (celPf##name)					    \
  SCF_IMPLEMENTS_INTERFACE (iCelPropertyClassFactory)			    \
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)			    \
SCF_IMPLEMENT_IBASE_END							    \
SCF_IMPLEMENT_EMBEDDED_IBASE (celPf##name::Component)			    \
  SCF_IMPLEMENTS_INTERFACE (iComponent)					    \
SCF_IMPLEMENT_EMBEDDED_IBASE_END					    \
celPf##name::celPf##name (iBase *parent)				    \
{									    \
  SCF_CONSTRUCT_IBASE (parent);						    \
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);				    \
}									    \
celPf##name::~celPf##name()						    \
{									    \
}									    \
bool celPf##name::Initialize (iObjectRegistry* object_reg)		    \
{									    \
  celPf##name::object_reg = object_reg;					    \
  iCelPlLayer *pl = CS_QUERY_REGISTRY(object_reg, iCelPlLayer);		    \
  if (!pl) return false;						    \
  pl->RegisterPropertyClassFactory(this);				    \
  return true;								    \
}									    \
const char* celPf##name::GetName() const				    \
{									    \
  return strname;							    \
}									    \
iCelPropertyClass* celPf##name::CreatePropertyClass()			    \
{									    \
  return new celPc##name (object_reg);					    \
}									    \
bool celPf##name::Component::Initialize (iObjectRegistry* p)		    \
{									    \
  return scfParent->Initialize(p);					    \
					    \
}  



