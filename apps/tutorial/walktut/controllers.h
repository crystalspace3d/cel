#ifndef CONTROLLERS_H
#define CONTROLLERS_H

#include <physicallayer/pl.h>
#include <propclass/actormove.h>
#include <propclass/meshsel.h>
#include <propclass/mesh.h>
#include <propclass/camera.h>
#include <propclass/inv.h>

#include "physicallayer/facttmpl.h"
#include "physicallayer/propclas.h"
#include "celtool/stdpcimp.h"

CEL_DECLARE_FACTORY (PlayerController)
CEL_DECLARE_FACTORY (BadOneController)

class celPcPlayerController : public scfImplementationExt0<
		       celPcPlayerController, celPcCommon>
{
private:
  static csStringID id_child;

  // For actions.
  enum actionids
  {
    action_drop = 0,
    action_add,
    action_delete,
    action_pickup,
  };

  static PropertyHolder propinfo;

  void GetActorMove ();
  csWeakRef<iPcActorMove> pcactormove;

  void GetMesh ();
  csWeakRef<iPcMesh> pcmesh;

  void GetInventory ();
  csWeakRef<iPcInventory> pcinventory;

  void ShowInventory ();
  void Drop ();
  void PickUp (const char* childName);

public:
  celPcPlayerController (iObjectRegistry* object_reg);
  virtual ~celPcPlayerController ();

  virtual bool PerformActionIndexed (int idx,
      iCelParameterBlock* params, celData& ret);
};

class celPcBadOneController : public scfImplementationExt0<
		       celPcBadOneController, celPcCommon>
{
private:
  // For actions.
  enum actionids
  {
    action_restart = 0,
  };

  static PropertyHolder propinfo;

  void Restart ();

  csRef<csPath> path;
  void ReadPath ();

public:
  celPcBadOneController (iObjectRegistry* object_reg);
  virtual ~celPcBadOneController ();

  virtual bool PerformActionIndexed (int idx,
      iCelParameterBlock* params, celData& ret);
};

#endif
