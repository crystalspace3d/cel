#ifndef ELCMTEST_H
#define ELCMTEST_H

#include <physicallayer/entity.h>
#include <physicallayer/pl.h>

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

  csRef<iCelPlLayer> pl;
  csRef<iCelEntity> playerEntity;

  bool OnKeyboard (iEvent&);
  void Frame ();

  bool CreateLevel ();
  bool CreatePlayer ();

public:
  ElcmTest ();
  virtual ~ElcmTest ();

  virtual bool OnInitialize (int argc, char* argv[]);
  virtual bool Application ();
  virtual void OnExit ();
};


#endif
