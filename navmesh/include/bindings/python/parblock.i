/*
  Extensions for iCelParameterBlock so it can be used similar to a dict.
  use as follows:
	pars = celGenericParameterBlock (3)
	pars.SetParameterDef(0,id_control,"control")
	pars.SetParameterDef(1,id_x,"x")
	pars.SetParameterDef(2,id_y,"y")
	pars[id_control] = "hello"
	pars[id_x] = 1000.0
	pars.keys()
	pars.iterkeys()
	if id_control in pars:
		print pars[id_control]
	pars.GetParameterValue(id_control) # same as parblock[id_control]
	pars.GetParameterByIndex(0)
	pars.GetParameterName(0)
	pars.GetParameterID(0)
*/


%extend iCelParameterBlock {
	csStringID GetParameterIDByIndex(size_t idx) {
		csStringID id;
		celDataType t;
		id = self->GetParameterDef(idx, t);
		return id;
	}
	bool __contains__(csStringID id) {
		if (self->GetParameter(id))
			return true;
		return false;
	}
	%pythoncode %{
		def value_iterator(parblock):
			for idx in xrange(len(parblock)):
				yield parblock.GetParameterByIndex(idx)
		def key_iterator(parblock):
			for idx in xrange(len(parblock)):
				yield parblock.GetParameterIDByIndex(idx)
		def GetParameterValue(self,id):
			return self.GetParameter(id)
		def __iter__(self):
			return self.value_iterator()
		def iterkeys(self):
			return self.key_iterator()
		def __getitem__(self,i):
			return self.GetParameter(i)
		def keys(self):
			keys = []
			for idx in xrange(len(self)):
				keys.append(self.GetParameterIDByIndex(idx))
			return keys
		def has_key(self,id):
			return self.__contains__(id)
		%}
	CEL_PAR_SET(bool,bool);
	CEL_PAR_SET(int8,int8);
	CEL_PAR_SET(int16,int16);
	CEL_PAR_SET(long,int32);
	CEL_PAR_SET(uint8,uint8);
	CEL_PAR_SET(uint16,uint16);
	CEL_PAR_SET(unsigned long,uint32);
	CEL_PAR_SET(float,float);
	CEL_PAR_SET(const char *,const char *);
	CEL_PAR_SET(csVector3&,csVector3&);
	CEL_PAR_SET(csVector2&,csVector2&);
	CEL_PAR_SET(csColor&,csColor&);
	CEL_PAR_SET(iCelPropertyClass*,iCelPropertyClass*);
	CEL_PAR_SET(iCelEntity*,iCelEntity*);

	/*void __delitem__ (size_t i) { self->DeleteAt(i); }*/
	int __len__ () { return self->GetParameterCount(); }
}

