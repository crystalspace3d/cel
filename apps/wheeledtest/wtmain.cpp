/*
    Copyright (C) 2005 by Jorrit Tyberghein

    This file is part of CEL.

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <cssysdef.h>

#include "csutil/sysfunc.h"
#include "cstool/initapp.h"

#include "wheeledtest.h"

CS_IMPLEMENT_APPLICATION

int main (int argc, char** argv)
{
  // Now create our main application class which will do the basic
  // initialization and event handling for the entire game. 
  WheeledTest cc;
  // Start the app.
  // The underlying csApplicationFramework also does some core 
  // initialization. It will setup the configuration manager, event queue,
  // object registry, and a lot more. The object registry is very important,
  // and it is stored in your main application class (again, by 
  // csApplicationFramework).
  return cc.Main (argc, argv);
}
