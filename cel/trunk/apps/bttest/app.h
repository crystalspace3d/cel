#ifndef APP_H
#define APP_H

#include "cstool/demoapplication.h" 
#include <physicallayer/entity.h>
#include <physicallayer/pl.h>
#include "tools/behaviourtree.h"

class FramePrinter;

class MainApp : public CS::Utility::DemoApplication 
{
private:
  csRef<iCelPlLayer> pl;
  csRef<iCelBlLayer> bl;
  csRef<iCelEntity> level_entity;
  csRef<iCelEntity> player_entity;

  bool OnKeyboard (iEvent&);
  void Frame ();

  bool LoadLevel ();
  bool CreatePlayer ();

  void CreateBehaviourTree ();
  void LoadBehaviourTreeFromXML();


public:
  MainApp ();
  virtual ~MainApp ();

  void PrintHelp ();
  virtual bool OnInitialize (int argc, char* argv[]);
  virtual bool Application ();
  virtual void OnExit ();
};


#endif