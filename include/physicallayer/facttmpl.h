/*
    Crystal Space Entity Layer
    Copyright (C) 2001 by Jorrit Tyberghein
    This file was written by Matze Braun <MatzeBraun@gmx.de>
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
  
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
  
    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef __FACTTMPL_H__
#define __FACTTMPL_H__

/**
 * This macro declares a factory class for a property class. Note that your
 * Property Class name must be of the form celPcMyName if you've given
 * MyName as Factory Name.
 */
#define CEL_DECLARE_FACTORY(name)					    \
class celPf##name : public scfImplementation2<celPf##name,		    \
  iCelPropertyClassFactory, iComponent>					    \
{									    \
private:								    \
  iObjectRegistry* object_reg;						    \
									    \
public:									    \
  celPf##name (iBase* parent) : scfImplementationType (this, parent) { }    \
  virtual ~celPf##name () { }						    \
  bool Initialize (iObjectRegistry* object_reg);			    \
  virtual const char* GetName() const;					    \
  virtual csPtr<iCelPropertyClass> CreatePropertyClass (const char* name);  \
};

/** 
 * This macro implements a property class factory, the first argument is the
 * name you've given at the declaration, the 2nd argument is the string the
 * factory will be registered with.
 * Note: You have to export the classtable yourself as one could have multiple
 * factories in 1 file. The name to export for the factory MyFactory is:
 *   celPfMyFactory
 */
#define CEL_IMPLEMENT_FACTORY(name, strname)				    \
SCF_IMPLEMENT_FACTORY (celPf##name)					    \
bool celPf##name::Initialize (iObjectRegistry* object_reg)		    \
{									    \
  celPf##name::object_reg = object_reg;					    \
  csRef<iCelPlLayer> pl = csQueryRegistry<iCelPlLayer> (object_reg);	    \
  if (!pl) return false;						    \
  pl->RegisterPropertyClassFactory(this, 0);				    \
  return true;								    \
}									    \
const char* celPf##name::GetName() const				    \
{									    \
  return strname;							    \
}									    \
csPtr<iCelPropertyClass> celPf##name::CreatePropertyClass(const char* name) \
{									    \
  iCelPropertyClass* pc = new celPc##name (object_reg);	    		    \
  pc->SetName (name); 							    \
  return csPtr<iCelPropertyClass> (pc); 				    \
}									    \

/**
 * This macro is a temporary hack! It allows us to remain backwards
 * compatible with old propclass names.
 */
#define CEL_IMPLEMENT_FACTORY_ALT(name, strname, altname)		    \
SCF_IMPLEMENT_FACTORY (celPf##name)					    \
bool celPf##name::Initialize (iObjectRegistry* object_reg)		    \
{									    \
  celPf##name::object_reg = object_reg;					    \
  csRef<iCelPlLayer> pl = csQueryRegistry<iCelPlLayer> (object_reg);	    \
  if (!pl) return false;						    \
  pl->RegisterPropertyClassFactory(this, altname);			    \
  return true;								    \
}									    \
const char* celPf##name::GetName() const				    \
{									    \
  return strname;							    \
}									    \
csPtr<iCelPropertyClass> celPf##name::CreatePropertyClass(const char* name) \
{									    \
  iCelPropertyClass* pc = new celPc##name (object_reg);	  		    \
  pc->SetName (name); 							    \
  return csPtr<iCelPropertyClass> (pc); 				    \
}									    \

#endif
