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

  csString statusLine;
  csRef<iFont> font;
  int colorWhite;
  int colorBlack;
  int colorRed;

  /// Physics.
  csRef<iDynamics> dyn;
  csRef<iDynamicSystem> dynSys;
  csRef<CS::Physics::Bullet::iDynamicSystem> bullet_dynSys;

  bool OnKeyboard (iEvent&);
  bool OnMouseMove (iEvent&);
  bool OnMouseDown (iEvent&);
  void Frame ();

  bool InitWindowSystem ();
  bool InitPhysics ();
  bool CreateLevel ();
  bool CreateSky ();
  bool CreatePlayer ();
  bool CreateFactories ();
  bool FillDynamicWorld ();

  iCelEntity* FindHitEntity (int x, int y);
  iDynamicObject* FindHitDynObj (int x, int y);
  iRigidBody* FindHitBody (int x, int y, csVector3& start, csVector3& end,
      csVector3& isect);

  void PickUpDynObj (iDynamicObject* dynobj);

public:
  ElcmTest ();
  virtual ~ElcmTest ();

  iGraphics3D* GetG3D () const { return g3d; }
  iCEGUI* GetCEGUI () const { return cegui; }
  void WriteStatusLine ();
  void UpdateStatusLine (iDynamicObject* dynobj);

  bool ReceiveMessage (csStringID msg_id, iMessageSender* sender,
      celData& ret, iCelParameterBlock* params);
  void SelectEntity (iCelEntity* entity, bool left);
  void SelectTemplate (iCelEntityTemplate* tpl, bool left);
  void SelectEntity (iCelEntity* entity);
  void SelectTemplate (iCelEntityTemplate* tpl);

  virtual bool OnInitialize (int argc, char* argv[]);
  virtual bool Application ();
  virtual void OnExit ();

  CS_EVENTHANDLER_NAMES("application.elcmtest")

  /* Declare that we want to receive events *after* the CEGUI plugin. */
  virtual const csHandlerID * GenericPrec (csRef<iEventHandlerRegistry> &r1, 
    csRef<iEventNameRegistry> &r2, csEventID event) const 
  {
    static csHandlerID precConstraint[2];
    
    precConstraint[0] = r1->GetGenericID("crystalspace.cegui");
    precConstraint[1] = CS_HANDLERLIST_END;
    return precConstraint;
  }

  CS_EVENTHANDLER_DEFAULT_INSTANCE_CONSTRAINTS
};


#endif
