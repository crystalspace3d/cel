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
  virtual bool SendMessage (const char* msg_id, iCelPropertyClass* pc, celData& ret, 
                            iCelParameterBlock* params, ...);
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
  csStringID id_pcinventory_addchild;
  csStringID id_pcinventory_removechild;
  csStringID id_pcmover_arrived;

  void GetLinearMovement();
  csWeakRef<iPcLinearMovement> pcLinearMovement;

  void GetActorMove ();
  csWeakRef<iPcActorMove> pcActorMove;

  void GetMover ();
  csWeakRef<iPcMover> pcMover;

  void GetInventory ();
  csWeakRef<iPcInventory> pcInventory;

  void GetMesh ();
  csWeakRef<iPcMesh> pcMesh;

  void ShowInventory ();
  void Drop ();

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

#endif
