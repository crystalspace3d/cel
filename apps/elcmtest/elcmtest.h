#ifndef ELCMTEST_H
#define ELCMTEST_H

#include <CEGUI.h>
#include "ivaria/icegui.h"
#include <physicallayer/entity.h>
#include <physicallayer/pl.h>
#include <propclass/dynworld.h>
#include <tools/elcm.h>

class FramePrinter;

class ElcmTest : public csApplicationFramework,
                public csBaseEventHandler
{
private:
  csRef<iGraphics3D> g3d;
  csRef<iEngine> engine;
  csRef<iLoader> loader;
  csRef<iVFS> vfs;
  csRef<iVirtualClock> vc;
  csRef<iKeyboardDriver> kbd;
  csRef<FramePrinter> printer;
  csRef<iCollideSystem> cdsys;

  csRef<iCelPlLayer> pl;
  csRef<iELCM> elcm;
  csRef<iCelEntity> playerEntity;
  csRef<iCelEntity> worldEntity;
  csRef<iPcDynamicWorld> dynworld;
  iCamera* camera;
  iSector* sector;

  csRef<iCEGUI> cegui;

  /// Physics.
  csRef<iDynamics> dyn;
  csRef<iDynamicSystem> dynSys;
  csRef<CS::Physics::Bullet::iDynamicSystem> bullet_dynSys;

  bool OnKeyboard (iEvent&);
  void Frame ();

  bool InitWindowSystem ();
  bool InitPhysics ();
  bool CreateLevel ();
  bool CreatePlayer ();
  bool CreateFactories ();
  bool FillDynamicWorld ();

public:
  ElcmTest ();
  virtual ~ElcmTest ();

  virtual bool OnInitialize (int argc, char* argv[]);
  virtual bool Application ();
  virtual void OnExit ();
};


#endif
