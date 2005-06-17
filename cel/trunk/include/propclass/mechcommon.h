#ifndef __CEL_PF_MECHANICS_COMMON_H__
#define __CEL_PF_MECHANICS_COMMON_H__

enum celForceType {
  CEL_FT_REACTIONARY,	//This type of force is created by reactionary forces.
	  		//(will always affect the object.)
  CEL_FT_FLUID,		//This type of force acts on a fluid.
			//(requires that the object is in a fluid.)
  CEL_FT_SURFACE	//This type of force acts on a surface.
			//(requires that the object is in contact with a surface.)
};

#endif //__CEL_PF_MECHANICS_COMMON_H__

