/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_MECHANICS_COMMON_H__
#define __CEL_PF_MECHANICS_COMMON_H__

enum celForceType
{
  CEL_FT_REACTIONARY,	//This type of force is created by reactionary forces.
	  		//(will always affect the object.)
  CEL_FT_FLUID,		//This type of force acts on a fluid.
			//(requires that the object is in a fluid.)
  CEL_FT_SURFACE	//This type of force acts on a surface.
			//(requires that the object is in contact with a surface.)
};

#endif //__CEL_PF_MECHANICS_COMMON_H__

