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

#ifndef __CEL_PF_ANIMATION__
#define __CEL_PF_ANIMATION__

#include "cstypes.h"
#include "imesh/nskeleton.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/animation.h"
#include "tools/animsys.h"

struct iCelEntity;
struct iDocumentNode;
struct iObjectRegistry;
struct iGraphics3D;

/**
 * Factory for test.
 */
CEL_DECLARE_FACTORY (Animation)

/**
 * This is a test property class.
 */
class celPcAnimation : public scfImplementationExt1<
    celPcAnimation, celPcCommon, iPcAnimation>
{
public:
  celPcAnimation (iObjectRegistry* object_reg);
  virtual ~celPcAnimation ();

  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformActionIndexed (int idx,
      iCelParameterBlock* params, celData& ret);

  // Callback to update the node tree
  virtual void TickEveryFrame ();

  virtual bool Setup ();
  virtual void DrawSkeleton (iGraphics3D* g3d);
  virtual bool Load (const char* path, const char* file);
  virtual CEL::Animation::iNode* FindNodeByName (const char* name);

private:
  // For SendMessage parameters.
  static csStringID id_message;
  celOneParameterBlock* params;

  // For actions.
  enum actionids
  {
    action_print = 0
  };

  // For properties.
  enum propids
  {
    propid_counter = 0,
    propid_max
  };
  static PropertyHolder propinfo;

  csRef<Skeleton::iSkeleton> skel;
  csRef<CEL::Animation::iNode> rootnode;
  csRefArray<CEL::Animation::iNode> allnodes;

  // Other fields.
  int counter;
  size_t max;

  csRef<iMessageDispatcher> dispatcher_print;

  csStringHash xmltokens;
#define CS_TOKEN_ITEM_FILE "plugins/propclass/animation/anim.tok"
#include "cstool/tokenlist.h"
  void ParseNode (iDocumentNode* node, const csRef<CEL::Animation::iAnimationSystem> &animsys,
    csRef<CEL::Animation::iNode> parent);

  void ParseCondition (iDocumentNode* node, const csRef<CEL::Animation::iAnimationSystem> &animsys,
    csRef<CEL::Animation::iNode> parent);
  void ParseCondition (iDocumentNode* node, const csRef<CEL::Animation::iAnimationSystem> &animsys,
    csRef<CEL::Animation::iCondition> parent);
  csRef<CEL::Animation::iCondition> ParseConditionBase (iDocumentNode* node,
    const csRef<CEL::Animation::iAnimationSystem> &animsys, const char* type);

  void ParseResult (iDocumentNode* node, const csRef<CEL::Animation::iAnimationSystem> &animsys,
    csRef<CEL::Animation::iCondition> parent);

  bool ParseParameter (iDocumentNode* node, const char* &paramname, celData &paramval);
};

#endif // __CEL_PF_ANIMATION__
