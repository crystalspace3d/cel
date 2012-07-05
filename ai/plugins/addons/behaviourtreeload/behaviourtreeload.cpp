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

#include "cssysdef.h"
#include "csutil/scanstr.h"
#include "cstool/initapp.h"
#include "iutil/objreg.h"
#include "iutil/document.h"
#include "iutil/object.h"
#include "iutil/plugin.h"
#include "ivaria/reporter.h"
#include "imap/services.h"
#include "iengine/mesh.h"

#include "physicallayer/pl.h"
#include "physicallayer/propclas.h"
#include "physicallayer/entity.h"
#include "behaviourlayer/bl.h"
#include "propclass/mesh.h"
#include "celtool/stdparams.h"

#include "behaviourtreeload.h"


SCF_IMPLEMENT_FACTORY (celAddOnBehaviourTreeLoader)

//---------------------------------------------------------------------------------------

celAddOnBehaviourTreeLoader::celAddOnBehaviourTreeLoader (iBase *parent) :
  scfImplementationType (this, parent)
{
  object_reg = 0;
}

celAddOnBehaviourTreeLoader::~celAddOnBehaviourTreeLoader ()
{
}

bool celAddOnBehaviourTreeLoader::Initialize (iObjectRegistry *object_reg)
{
  celAddOnBehaviourTreeLoader::object_reg = object_reg;
  return true;
}

csPtr<iBase> celAddOnBehaviourTreeLoader::Parse (iDocumentNode* node,
	iStreamSource*, iLoaderContext* ldr_context, iBase* context)
{
  csRef<iPluginManager> plugin_mgr = 
      csQueryRegistry<iPluginManager> (object_reg);

  root_node= csLoadPlugin<iBTNode> (plugin_mgr,
    "cel.behaviourtree.action");

  csRef<iRewardType> reward_type = csLoadPlugin<iRewardType> (plugin_mgr,
    "cel.rewards.debugprint");
  csRef<iRewardFactory> reward_factory = reward_type->CreateRewardFactory ();
  csRef<iDebugPrintRewardFactory> explicit_reward_factory = 
	scfQueryInterface<iDebugPrintRewardFactory> (reward_factory);

  csRef<iCelParameterBlock> params;
  params.AttachNew (new celVariableParameterBlock ());

  csRef<iBTAction> explicit_action_node = scfQueryInterface<iBTAction> (root_node);
  explicit_reward_factory->SetMessageParameter ("To do - complete celAddOnBehaviourTreeLoader::Parse");
  // @@@ TODO: fix the quest parameter 0!
  csRef<iReward> reward = reward_factory->CreateReward(0, params);
  explicit_action_node->AddReward (reward);

  return scfQueryInterface<iBase> (root_node);
}