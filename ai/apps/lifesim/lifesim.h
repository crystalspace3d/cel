/*
  Copyright (C) 2011 Christian Van Brussel, Eutyche Mukuama, Dodzi de Souza
      Communications and Remote
      Sensing Laboratory of the School of Engineering at the 
      Universite catholique de Louvain, Belgium
      http://www.tele.ucl.ac.be

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this library; if not, write to the Free
  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef __LIFESIMULATOR_H__
#define __LIFESIMULATOR_H__

#define WATER_LEVEL 50.0

#include "cstool/demoapplication.h"
#include "ivaria/collider.h"
#include <isndsys/ss_manager.h>
#include <isndsys/ss_renderer.h>
#include <imap/loader.h>

#include "physicallayer/pl.h"
#include "behaviourlayer/bl.h"

class LifeSimulator : public CS::Utility::DemoApplication
{
 private:
  bool CreateScene ();
  bool CreateFrankieEntity (const csVector3 pos);
  bool CreateSheepEntity (const csVector3 pos);
  bool CreateRamEntity (const csVector3 pos);
  bool CreateBirdEntity (const csVector3 pos);
  bool CreateRatEntity (const csVector3 pos);
  bool CreateButterFlyEntity (const csVector3 pos);
  
  bool TraceMouseBeam (csVector3& position);

  csRef<iCelPlLayer> physicalLayer;
  csRef<iCelBlLayer> behaviourLayer;
  csRef<iCollideSystem> collideSystem;
  csRef<iLoader> loader;
  csRef<iSndSysRenderer> sndrdr;
  csRef<iSndSysManager> sndmgr;

 public:
  LifeSimulator ();

  //-- CS::Utility::DemoApplication
  void PrintHelp ();
  bool OnInitialize (int argc, char* argv[]);
  void Frame ();
  bool OnKeyboard (iEvent &ev);

  //-- csApplicationFramework
  bool Application ();
};

#endif // __LIFESIMULATOR_H__
