%define CELDATA_RETURN(data)
		PyObject *obj = Py_None;
		if (data)
		{
		    switch(data->type)
		    {
			case CEL_DATA_FLOAT:
				obj = PyFloat_FromDouble((float)data->value.f);
				break;
			case CEL_DATA_BOOL:
				obj = SWIG_From_bool((bool)data->value.bo);
				break;
			case CEL_DATA_STRING:
			{
				char *res;
				res = (char*)((iString const *)(data->value.s)->GetData());
				obj = SWIG_FromCharPtr(res);
				break;
			}
			case CEL_DATA_VECTOR2:
			{
				csVector2 *res;
				res = new csVector2(data->value.v.x,data->value.v.y);
				obj = SWIG_NewPointerObj((void*)(res), SWIGTYPE_p_csVector2, 1);
				break;
			}
			case CEL_DATA_VECTOR3:
			{
				csVector3 *res;
				res = new csVector3(data->value.v.x,data->value.v.y,data->value.v.z);
				obj = SWIG_NewPointerObj((void*)(res), SWIGTYPE_p_csVector3, 1);
				break;
			}
			case CEL_DATA_ENTITY:
			{
				iCelEntity *res;
				res = (iCelEntity *)(data->value.ent);
				obj = SWIG_NewPointerObj((void*)(res), SWIGTYPE_p_iCelEntity, 0);
				break;
			}
			case CEL_DATA_PCLASS:
			{
				iCelPropertyClass *res;
				res = (iCelPropertyClass *)(data->value.pc);
				obj = SWIG_NewPointerObj((void*)(res), SWIGTYPE_p_iCelPropertyClass, 0);
				break;
			}
			case CEL_DATA_ACTION:
			{
				char *res;
				res = (char*)((iString const *)(data->value.s)->GetData());
				obj = SWIG_FromCharPtr(res);
				break;
			}
			case CEL_DATA_IBASE:
			{
				iBase *res;
				res = (iBase *)(data->value.pc);
				obj = SWIG_NewPointerObj((void*)(res), SWIGTYPE_p_iBase, 0);
				break;
			}
			case CEL_DATA_COLOR:
			{
				csColor *res;
				res = new csColor(data->value.col.red,data->value.col.green,data->value.col.blue);
				obj = SWIG_NewPointerObj((void*)(res), SWIGTYPE_p_csColor, 1);
				break;
			}
			case CEL_DATA_WORD:
			{
				obj = SWIG_From_int((int)data->value.w);
				break;
			}
			case CEL_DATA_LONG:
			{
				obj = SWIG_From_long((long)data->value.l);
				break;
			}
			case CEL_DATA_ULONG:
			{
				obj = PyLong_FromUnsignedLong((unsigned long)data->value.ul);
				break;
			}
			{
				break;
			}
			/* Still to be done (and some more) */
			case CEL_DATA_BYTE:
				/*data->value.b (int8)*/
			case CEL_DATA_UWORD:
				/*data->value.uw (uint16)*/
			case CEL_DATA_PARAMETER:
				/*data->value.par (iString+celDataType)*/
			default:
                                Py_INCREF(Py_None);
				obj = Py_None;
				break;
		    }
		}
%enddef
%typemap(out) const celData*
{
	CELDATA_RETURN($1)
	$result = obj;
}
%define CEL_PAR_SET(par_type)
	void __setitem__ (csStringID idx, par_type c) 
	{ 
	  if (self->GetParameter(idx))
	  ((celData*)self->GetParameter(idx))->Set(c); 
	}
%enddef

