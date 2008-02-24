/*
    Crystal Space Entity Layer
    Copyright (C) 2001 by Jorrit Tyberghein
  
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

#ifndef __CEL_BLTEST_BL__
#define __CEL_BLTEST_BL__

#include "csutil/scf_implementation.h"
#include "iutil/comp.h"
#include "behaviourlayer/bl.h"

struct iObjectRegistry;
struct iCelEntity;

/**
 * This is the Behaviour Layer itself.
 */
class celBlTest : public scfImplementation2<celBlTest, iCelBlLayer,
  iComponent>
{
private:
  iObjectRegistry* object_reg;

public:
  celBlTest (iBase* parent);
  virtual ~celBlTest ();

  virtual const char* GetName () const { return "bltest"; }
  virtual iCelBehaviour* CreateBehaviour (iCelEntity* entity, const char* name);

  virtual bool Initialize (iObjectRegistry* p);
};

#endif // __CEL_BLTEST_BL__
