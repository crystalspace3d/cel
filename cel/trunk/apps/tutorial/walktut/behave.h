#ifndef BEHAVE_H
#define BEHAVE_H

#include <physicallayer/pl.h>
#include <behaviourlayer/bl.h>
#include <behaviourlayer/behave.h>
#include <propclass/actormove.h>
#include <propclass/meshsel.h>
#include <propclass/mesh.h>
#include <propclass/camera.h>
#include <propclass/inv.h>


class BehaviourLayer : public iCelBlLayer
{
private:
  iCelPlLayer* pl;

public:
  BehaviourLayer (iCelPlLayer* pl);
  virtual ~BehaviourLayer ();

  SCF_DECLARE_IBASE;

  virtual const char* GetName () const { return "behaviourlayer"; }
  virtual iCelBehaviour* CreateBehaviour (iCelEntity* entity, const char* name);
};

class BehaviourCommon : public iCelBehaviour
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

  SCF_DECLARE_IBASE;

  virtual iCelBlLayer* GetBehaviourLayer () const { return bl; }
  virtual bool SendMessage (const char* msg_id,
	iCelPropertyClass* pc,
  	celData& ret, iCelParameterBlock* params, ...);
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

public:
  BehaviourPlayer (iCelEntity* entity, BehaviourLayer* bl, iCelPlLayer* pl);
  virtual ~BehaviourPlayer () { }

  virtual const char* GetName () const { return "player_behave"; }
  virtual bool SendMessage (csStringID msg_id,
	iCelPropertyClass* pc,
  	celData& ret, iCelParameterBlock* params, va_list arg);
};

#endif
