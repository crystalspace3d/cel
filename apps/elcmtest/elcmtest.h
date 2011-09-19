#ifndef ELCMTEST_H
#define ELCMTEST_H

#include <CEGUI.h>
#include "ivaria/icegui.h"
#include <physicallayer/entity.h>
#include <physicallayer/pl.h>
#include <propclass/dynworld.h>
#include <tools/elcm.h>
#include <tools/uitools/inventory.h>
#include <tools/uitools/inventory2.h>

class ElcmTest;
class FramePrinter;

class CeguiPrinter : public scfImplementation1<CeguiPrinter,iCelTimerListener>
{
private:
  ElcmTest* parent;

public:
  CeguiPrinter (ElcmTest* parent) : scfImplementationType (this), parent (parent) { }
  virtual ~CeguiPrinter () { }
  virtual void TickEveryFrame ();
  virtual void TickOnce () { }
};

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

  csRef<CeguiPrinter> ceguiPrinter;
  csRef<iCEGUI> cegui;
  csRef<iUIInventory> uiInventory;
  csRef<iUIInventory2> uiInventory2;

  csRef<iMessageReceiver> receiver;
  csStringID msgInventory;

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

  iGraphics3D* GetG3D () const { return g3d; }
  iCEGUI* GetCEGUI () const { return cegui; }

  bool ReceiveMessage (csStringID msg_id, iMessageSender* sender,
      celData& ret, iCelParameterBlock* params);

  virtual bool OnInitialize (int argc, char* argv[]);
  virtual bool Application ();
  virtual void OnExit ();
};


#endif
