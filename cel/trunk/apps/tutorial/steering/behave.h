#ifndef BEHAVE_H
#define BEHAVE_H

#include <physicallayer/pl.h>
#include <behaviourlayer/bl.h>
#include <behaviourlayer/behave.h>
#include <propclass/actormove.h>
#include <propclass/mover.h>
#include <propclass/steer.h>
#include <propclass/meshsel.h>
#include <propclass/mesh.h>
#include <propclass/camera.h>
#include <propclass/inv.h>
#include <tools/celhpf.h>
#include <tools/celnavmesh.h>


class BehaviourLayer :
  public scfImplementation1<BehaviourLayer, iCelBlLayer>
{
private:
  iCelPlLayer* pl;

  csRef<iCelHPath> path;

public:
  BehaviourLayer (iCelPlLayer* pl);
  virtual ~BehaviourLayer ();

  virtual const char* GetName () const { return "behaviourlayer"; }
  virtual iCelBehaviour* CreateBehaviour (iCelEntity* entity, const char* name);

  void SetPath (iCelHPath* path);
  iCelHPath* GetPath () const;
};

class BehaviourCommon :
  public scfImplementation1<BehaviourCommon, iCelBehaviour>
{
protected:
  iCelEntity* entity;
  BehaviourLayer* bl;
  iCelPlLayer* pl;

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
  virtual bool SendMessage (csStringID msg_id,
	iCelPropertyClass* pc,
  	celData& ret, iCelParameterBlock* params, va_list arg);

  virtual iCelBlLayer* GetBehaviourLayer () const { return bl; }
  virtual bool SendMessageV (const char* msg_id,
	iCelPropertyClass* pc,
  	celData& ret, iCelParameterBlock* params, va_list arg);
  virtual void* GetInternalObject () { return 0; }
};

class BehaviourLevel : public BehaviourCommon
{
public:
  BehaviourLevel (iCelEntity* entity, BehaviourLayer* bl, iCelPlLayer* pl)
    : BehaviourCommon (entity, bl, pl) { }
  virtual ~BehaviourLevel () { }

  virtual const char* GetName () const { return "level_behave"; }
  virtual bool SendMessage (csStringID msg_id,
	iCelPropertyClass* pc,
  	celData& ret, iCelParameterBlock* params, va_list arg);
};

class BehaviourBox : public BehaviourCommon
{
private:
  csStringID id_pcmeshsel_down;

  void GetPlayer ();
  csWeakRef<iPcMeshSelect> pcmeshsel;
  iCelEntity* player;

  void PickUp ();

public:
  BehaviourBox (iCelEntity* entity, BehaviourLayer* bl, iCelPlLayer* pl);
  virtual ~BehaviourBox () { }

  virtual const char* GetName () const { return "box_behave"; }
  virtual bool SendMessage (csStringID msg_id,
	iCelPropertyClass* pc,
  	celData& ret, iCelParameterBlock* params, va_list arg);
};

class BehaviourBadOne : public BehaviourCommon
{
private:
  csStringID id_pctimer_wakeup;
  csStringID id_par_elapsedticks;

  csRef<csPath> path;
  void ReadPath ();

  void Restart ();

public:
  BehaviourBadOne (iCelEntity* entity, BehaviourLayer* bl, iCelPlLayer* pl);
  virtual ~BehaviourBadOne () { }

  virtual const char* GetName () const { return "badone_behave"; }
  virtual bool SendMessage (csStringID msg_id,
			    iCelPropertyClass* pc,
			    celData& ret, iCelParameterBlock* params, va_list arg);
};


class BehaviourSteering : public BehaviourCommon
{
 private:
  csStringID id_pcsteer_arrived;
  csStringID id_pccommandinput_arrival1;
  csStringID id_pccommandinput_ca1;
  csStringID id_pccommandinput_cohesion1;
  csStringID id_pccommandinput_separation1;
 csStringID id_pccommandinput_dm1;
  csStringID id_pccommandinput_seek1;
  csStringID id_pccommandinput_flee1;
  csStringID id_pccommandinput_wander1;
  csStringID id_pccommandinput_pursue1;
  
  bool ca, arrival, cohesion, separation, dm;
  
  csRef<iCelEntityList> entities;
  
 public:
  BehaviourSteering (iCelEntity* entity, BehaviourLayer* bl, iCelPlLayer* pl);
  virtual ~BehaviourSteering() {}
  virtual const char* GetName () const { return "steering_behave"; }
  virtual bool SendMessage (csStringID msg_id,
			    iCelPropertyClass* pc,
			    celData& ret, iCelParameterBlock* params, va_list arg);
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

  csStringID id_pcinventory_addchild;
  csStringID id_pcinventory_removechild;

  void GetActorMove ();
  csWeakRef<iPcActorMove> pcactormove;

  void GetInventory ();
  csWeakRef<iPcInventory> pcinventory;

  void GetMesh ();
  csWeakRef<iPcMesh> pcmesh;

  void ShowInventory ();
  void Drop ();

  // Hierarchical pathfinding
  csRef<iCelHPath> path;

public:
  BehaviourPlayer (iCelEntity* entity, BehaviourLayer* bl, iCelPlLayer* pl);
  virtual ~BehaviourPlayer () { }

  virtual const char* GetName () const { return "player_behave"; }
  virtual bool SendMessage (csStringID msg_id,
	iCelPropertyClass* pc,
  	celData& ret, iCelParameterBlock* params, va_list arg);
};

#endif
