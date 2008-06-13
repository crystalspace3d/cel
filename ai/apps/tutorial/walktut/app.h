#ifndef APP_H
#define APP_H

#include <physicallayer/entity.h>
#include <physicallayer/pl.h>

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

  csRef<iCelPlLayer> pl;
  csRef<iCelBlLayer> bl;
  csRef<iCelEntity> level_entity;
  csRef<iCelEntity> player_entity;

  bool OnKeyboard (iEvent&);
  void ProcessFrame ();
  void FinishFrame ();

  bool LoadLevel ();
  bool CreatePlayer ();

public:
  MainApp ();
  virtual ~MainApp ();

  virtual bool OnInitialize (int argc, char* argv[]);
  virtual bool Application ();
};


#endif
