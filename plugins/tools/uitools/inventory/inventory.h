/*
    Crystal Space Entity Layer
    Copyright (C) 2011 by Jorrit Tyberghein
  
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

#ifndef __CEL_UITOOLS_INVENTORY_IMP__
#define __CEL_UITOOLS_INVENTORY_IMP__

#include "csutil/util.h"

#include "tools/uitools/inventory.h"

struct iEngine;

class celUIInventory : public scfImplementation2<celUIInventory,
  iUIInventory, iComponent>
{
private:
  iObjectRegistry* object_reg;

public:
  celUIInventory (iBase* parent);
  virtual ~celUIInventory ();
  virtual bool Initialize (iObjectRegistry* object_reg);
};

#endif // __CEL_UITOOLS_INVENTORY_IMP__

