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

#include "cstool/csdemoapplication.h"

#include "physicallayer/pl.h"
#include "behaviourlayer/bl.h"

class LifeSimulator : public CS::Demo::DemoApplication
{
 private:
  bool CreateScene ();
  bool CreateFrankieEntity (const csVector3 pos);

  bool TraceMouseBeam (csVector3& position);

  csRef<iCelPlLayer> physicalLayer;
  csRef<iCelBlLayer> behaviourLayer;
  csRef<iCollideSystem> collideSystem;

 public:
  LifeSimulator ();

  //-- CS::Demo::DemoApplication
  void PrintHelp ();
  bool OnInitialize (int argc, char* argv[]);
  void Frame ();
  bool OnKeyboard (iEvent &ev);

  //-- csApplicationFramework
  bool Application ();
};

#endif // __ISLAND_H__
