#ifndef APP_H
#define APP_H

#include <physicallayer/entity.h>
#include <physicallayer/pl.h>
#include "tools/behaviourtree.h"

class FramePrinter;

class MainApp : public csApplicationFramework,
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

  csRef<iCelPlLayer> pl;
  csRef<iCelBlLayer> bl;
  csRef<iCelEntity> level_entity;
  csRef<iCelEntity> player_entity;

  bool OnKeyboard (iEvent&);
  void Frame ();

  bool LoadLevel ();
  bool CreatePlayer ();

  void CreateBehaviourTree ();

public:
  MainApp ();
  virtual ~MainApp ();

  virtual bool OnInitialize (int argc, char* argv[]);
  virtual bool Application ();
  virtual void OnExit ();
};


#endif