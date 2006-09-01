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
	pars.names()
	pars.iterkeys()
	pars.iternames()
	if id_control in pars:
		print pars[id_control]
	pars.GetParameterValue(id_control) # same as parblock[id_control]
	pars.GetParameterValueByIndex(0)
	pars.GetParameterName(0)
	pars.GetParameterID(0)
*/


%extend iCelParameterBlock {
	csStringID GetParameterIDByIndex(size_t idx) {
		csStringID id;
		celDataType t;
		const char *name = self->GetParameter(idx, id, t);
		return id;
	}
	const char * GetParameterNameByIndex(size_t idx) {
		csStringID id;
		celDataType t;
		return self->GetParameter(idx, id, t);
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
		def name_iterator(parblock):
			for idx in xrange(len(parblock)):
				yield parblock.GetParameterNameByIndex(idx)
		def GetParameterValue(self,id):
			return self.GetParameter(id)
		def __iter__(self):
			return self.value_iterator()
		def iterkeys(self):
			return self.key_iterator()
		def iternames(self):
			return self.name_iterator()
		def __getitem__(self,i):
			return self.GetParameter(i)
		def keys(self):
			keys = []
			for idx in xrange(len(self)):
				keys.append(self.GetParameterIDByIndex(idx))
			return keys
		def names(self):
			keys = []
			for idx in xrange(len(self)):
				keys.append(self.GetParameterNameByIndex(idx))
			return keys
		def has_key(self,id):
			return self.__contains__(id)
		%}
	CEL_PAR_SET(bool);
	CEL_PAR_SET(int8);
	CEL_PAR_SET(int16);
	CEL_PAR_SET(int32);
	CEL_PAR_SET(uint8);
	CEL_PAR_SET(uint16);
	CEL_PAR_SET(uint32);
	CEL_PAR_SET(float);
	CEL_PAR_SET(const char *);
	CEL_PAR_SET(csVector3&);
	CEL_PAR_SET(csVector2&);
	CEL_PAR_SET(csColor&);
	CEL_PAR_SET(iCelPropertyClass*);
	CEL_PAR_SET(iCelEntity*);

	/*void __delitem__ (size_t i) { self->DeleteAt(i); }*/
	int __len__ () { return self->GetParameterCount(); }
}

