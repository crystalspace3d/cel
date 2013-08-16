/*
    Copyright (C) 2010 by Leonardo Rodrigo Domingues

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
#ifndef BEHAVE_H
#define BEHAVE_H

#include <physicallayer/pl.h>
#include <behaviourlayer/bl.h>
#include <behaviourlayer/behave.h>
#include <iutil/csinput.h>
#include <propclass/actormove.h>
#include <propclass/meshsel.h>
#include <propclass/mesh.h>
#include <propclass/mover.h>
#include <propclass/camera.h>
#include <propclass/inv.h>
#include <tools/celhpf.h>
#include <tools/celnavmesh.h>


class BehaviourLayer : public scfImplementation1<BehaviourLayer, iCelBlLayer>
{
private:
  csRef<iCelPlLayer> physicalLayer;
  csRef<iObjectRegistry> objectRegistry;
  
  // Members used to transfer data between the behaviour layer and the application
  csRef<iCelHNavStruct> navStruct;
  csRef<iCelHPath> path;

public:
  BehaviourLayer (iCelPlLayer* physicalLayer, iObjectRegistry* objectRegistry);
  virtual ~BehaviourLayer ();

  virtual const char* GetName () const;
  virtual iCelBehaviour* CreateBehaviour (iCelEntity* entity, const char* name);
  void SetNavStruct (iCelHNavStruct* navStruct);
  iCelHNavStruct* GetNavStruct () const;
  void SetPath (iCelHPath* path);
  iCelHPath* GetPath () const;
};

class BehaviourCommon : public scfImplementation1<BehaviourCommon, iCelBehaviour>
{
protected:
  csRef<iCelEntity> entity;
  csRef<BehaviourLayer> behaviourLayer;
  csRef<iCelPlLayer> physicalLayer;

public:
  BehaviourCommon (iCelEntity* entity, BehaviourLayer* bl, iCelPlLayer* pl);
  virtual ~BehaviourCommon ();

  /**
   * This is a specific version of SendMessage() that accepts
   * an integer instead of a csStringID. Subclasses of ccBehaviourBase
   * will only have to override this version. The normal iCelBehaviour
   * versions of SendMessage() are implemented in this class and
   * will redirect to this version.
   */
  virtual bool SendMessage (csStringID msg_id, iCelPropertyClass* pc, celData& ret, 
                            iCelParameterBlock* params, va_list arg);

  virtual iCelBlLayer* GetBehaviourLayer () const;
  virtual bool SendMessageV (const char* msg_id, iCelPropertyClass* pc, celData& ret, 
                             iCelParameterBlock* params, va_list arg);
  virtual void* GetInternalObject ();
};

class BehaviourLevel : public BehaviourCommon
{
public:
  BehaviourLevel (iCelEntity* entity, BehaviourLayer* behaviourLayer, iCelPlLayer* physicalLayer) 
      : BehaviourCommon (entity, behaviourLayer, physicalLayer) { }
  virtual ~BehaviourLevel () { }

  virtual const char* GetName () const;
  virtual bool SendMessage (csStringID msg_id, iCelPropertyClass* pc, celData& ret, 
                            iCelParameterBlock* params, va_list arg);
};

class BehaviourPlayer : public BehaviourCommon
{
private:
  csStringID id_pccommandinput_forward1;
  csStringID id_pccommandinput_forward0;
  csStringID id_pccommandinput_backward1;
  csStringID id_pccommandinput_backward0;
  csStringID id_pccommandinput_rotateleft1;
  csStringID id_pccommandinput_rotateleft0;
  csStringID id_pccommandinput_rotateright1;
  csStringID id_pccommandinput_rotateright0;
  csStringID id_pccommandinput_cammode1;
  csStringID id_pccommandinput_drop1;
  csStringID id_pccommandinput_setposition;
  csStringID id_pcmover_arrived;

  void GetLinearMovement();
  csWeakRef<iPcLinearMovement> pcLinearMovement;

  void GetActorMove ();
  csWeakRef<iPcActorMove> pcActorMove;

  void GetMover ();
  csWeakRef<iPcMover> pcMover;

  void GetMesh ();
  csWeakRef<iPcMesh> pcMesh;

  // Hierarchical pathfinding
  csRef<iCelHPath> path;

  csRef<iObjectRegistry> objectRegistry;
  csRef<iCamera> camera;

public:
  BehaviourPlayer (iCelEntity* entity, BehaviourLayer* behaviourLayer, iCelPlLayer* physicalLayer, 
                   iObjectRegistry* objectRegistry);
  virtual ~BehaviourPlayer () { }

  virtual const char* GetName () const;
  virtual bool SendMessage (csStringID msg_id, iCelPropertyClass* pc, celData& ret, 
                            iCelParameterBlock* params, va_list arg);
};

class BehaviourBox : public BehaviourCommon
{
private:
  csStringID id_box_arrived;
  csVector3 point1;
  csRef<iSector> point1Sector;
  csVector3 point2;
  csRef<iSector> point2Sector;
  bool oneIsCurrent; // True if one is the current destination

  void GetLinearMovement();
  csWeakRef<iPcLinearMovement> pcLinearMovement;

  void GetActorMove ();
  csWeakRef<iPcActorMove> pcActorMove;

  void GetMover ();
  csWeakRef<iPcMover> pcMover;

  void GetMesh ();
  csWeakRef<iPcMesh> pcMesh;

public:
  BehaviourBox (iCelEntity* entity, BehaviourLayer* behaviourLayer, iCelPlLayer* physicalLayer, 
                   iObjectRegistry* objectRegistry);
  virtual ~BehaviourBox () { }

  virtual const char* GetName () const;
  virtual bool SendMessage (csStringID msg_id, iCelPropertyClass* pc, celData& ret, 
                            iCelParameterBlock* params, va_list arg);
};

#endif
