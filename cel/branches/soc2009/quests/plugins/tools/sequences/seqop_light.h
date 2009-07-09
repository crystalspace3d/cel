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

#ifndef __CEL_TOOLS_SEQOP_LIGHT__
#define __CEL_TOOLS_SEQOP_LIGHT__

#include "csutil/util.h"
#include "csutil/refarr.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "iutil/virtclk.h"
#include "iengine/light.h"
#include "tools/sequences.h"

#include "propclass/light.h"

struct iObjectRegistry;
struct iEvent;

/**
 * A standard seqop type to animate a light.
 * This seqop type listens to the name 'cel.seqops.light'.
 */
CEL_DECLARE_SEQOPTYPE_NEW(Light,"cel.seqops.light")

/**
 * The 'light' seqop factory.
 */
class celLightSeqOpFactory : public scfImplementation2<
	celLightSeqOpFactory, iSeqOpFactory,
	iLightSeqOpFactory>
{
private:
  celLightSeqOpType* type;
  csString entity_par;
  csString tag_par;
  csString rel_red_par;
  csString rel_green_par;
  csString rel_blue_par;
  csString abs_red_par;
  csString abs_green_par;
  csString abs_blue_par;

public:
  celLightSeqOpFactory (celLightSeqOpType* type);
  virtual ~celLightSeqOpFactory ();

  virtual csPtr<iSeqOp> CreateSeqOp (
      const celParams& params);
  virtual bool Load (iDocumentNode* node);

  //----------------- iLightSeqOpFactory -----------------------
  virtual void SetEntityParameter (const char* entity, const char* tag = 0);
  virtual void SetRelColorParameter (const char* red, const char* green,
  	const char* blue);
  virtual void SetAbsColorParameter (const char* red, const char* green,
  	const char* blue);
};

/**
 * The 'light' seqop.
 */
class celLightSeqOp : public scfImplementation1<
	celLightSeqOp, iSeqOp>
{
private:
  celLightSeqOpType* type;
  csString entity;
  csString tag;
  csColor rel;
  csColor abs;
  bool do_abs;
  bool do_rel;

  csColor start;
  csWeakRef<iLight> light;

  void FindLight ();

public:
  celLightSeqOp (celLightSeqOpType* type,
  	const celParams& params,
	const char* entity_par, const char* tag_par,
	const char* rel_red_par, const char* rel_green_par,
	const char* rel_blue_par,
	const char* abs_red_par, const char* abs_green_par,
	const char* abs_blue_par);
  virtual ~celLightSeqOp ();

  virtual bool Load (iCelDataBuffer* databuf);
  virtual void Save (iCelDataBuffer* databuf);
  virtual void Init ();
  virtual void Do (float time);
};

#endif // __CEL_TOOLS_SEQOP_LIGHT__

