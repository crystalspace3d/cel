/*
    Crystal Space Entity Layer
    Copyright (C) 2004 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_PATHFINDER__
#define __CEL_PF_PATHFINDER__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/pathfinder.h"
#include "propclass/linmove.h"
#include "propclass/actormove.h"
#include "propclass/mesh.h"
#include "iengine/engine.h"
#include "csutil/randomgen.h"
#include "tools/celgraph.h"
#include "propclass/steer.h"

struct iCelEntity;
struct iObjectRegistry;
/**
 * Factory for Path Finder.
 */
CEL_DECLARE_FACTORY (PathFinder)

/**
 * This is the PathFinder property class.
 */
class celPcPathFinder : public scfImplementationExt1<celPcPathFinder,
  celPcCommon, iPcPathFinder>
{
private: 
  csWeakRef<iEngine> engine;
  csWeakRef<iPcLinearMovement> pclinmove;
  csWeakRef<iPcActorMove> pcactormove;
  csWeakRef<iPcMesh> pcmesh;
  csWeakRef<iCelGraph> celgraph;
  csWeakRef<iPcSteer> pcsteer;

  csRef<iMessageDispatcher> dispatcher_interrupted;

  // For actions.
  static csStringID id_sectorname;
  static csStringID id_position;
  static csStringID id_pursue_max_prediction;
  static csStringID id_is_active;
  static csStringID id_min_distance;

  // for receive message
  static csStringID id_msg_arrived;
  static csStringID id_msg_interrupted;

  enum actionids
  {
    action_seek=0,
    action_wander,
    action_pursue,
    action_interrupt,
    action_cyclic,
    action_one_way,
    action_two_way
  };

  // For properties.
  enum propids
  {
    propid_position,
    propid_pursue_max_prediction,
    propid_min_distance,
    propid_is_active,
  };

  static PropertyHolder propinfo;

  // Normal fields.
  csRef <iCelPath> cur_path;
  iSector* sector;
  csVector3 position;
  int current_action, delay_recheck;
  size_t wander_distance;
  bool is_active;
  csRef <iCelNode> goal;

  float pursue_max_prediction, min_distance;

  iCelEntity* pursue_target;

  csRandomGen random;

  void FindSiblingPropertyClasses ();
  void SendMessage (const char* msgold, const char* msg,
      csRef<iMessageDispatcher>& dispatcher, const char* meshname = 0);
  virtual bool FollowPath ();    

  static csStringID id_meshname;
  celOneParameterBlock* params;

 public:
  celPcPathFinder (iObjectRegistry* object_reg);

  virtual bool ReceiveMessage (csStringID msg_id, iMessageSender* sender,
                celData& ret, iCelParameterBlock* params);

  virtual ~celPcPathFinder ();

  virtual bool Seek (iSector* sector, const csVector3& position);

  virtual bool Wander (size_t distance);

  virtual bool Pursue (iCelEntity* target, float max_prediction);

  virtual bool FollowCyclicPath (iCelPath* path);

  virtual bool FollowOneWayPath (iCelPath* path);

  virtual bool FollowTwoWayPath (iCelPath* path);

  virtual void Interrupt ();

  virtual void SetDelayRecheck(int delay);

  virtual void SetMinDistance(int distance);

  virtual const csVector3& GetPosition () const { return position; }

  virtual bool PerformActionIndexed (int idx, iCelParameterBlock* params,
    celData& ret);

  virtual void TickOnce ();

  virtual void StopTracking ();

  virtual bool IsActive () {return is_active;}

  virtual void SetGraph (iCelGraph* graph);
};

#endif // __CEL_PF_PATHFINDER__
