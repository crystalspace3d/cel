/*
    Crystal Space Entity Layer
    Copyright (C) 2001 by Jorrit Tyberghein
  
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
  
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
  
    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef __CEL_PF_SOUND__
#define __CEL_PF_SOUND__

#include "cstypes.h"
#include "csutil/scf.h"

struct iSoundListener;
struct iSoundSource;

/**
 * This is the sound listener property class. There can really be only
 * one listener at any moment. It represents the listener (i.e. player of the game).
 * <p>
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * <ul>
 * <li>SetDirection: parameters 'front' (vector3), 'top' (vector3).
 * </ul>
 * <p>
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property:
 * <ul>
 * <li>front (vector3, read/write): front direction.
 * <li>top (vector3, read/write): top direction.
 * <li>position (vector3, read/write): position.
 * <li>velocity (vector3, read/write): velocity.
 * <li>distancefactor (float, read/write): distance factor.
 * <li>rollofffactor (float, read/write): rolloff factor.
 * <li>dopplerfactor (float, read/write): doppler factor.
 * <li>headsize (float, read/write): headsize.
 * <li>environment (string, read/write): sound environment. One of:
 *     "generic", "paddedcell", "room", "bathroom", "livingroom", "stoneroom",
 *     "auditorium", "concerthall", "cave", "arena", "carpetedhallway",
 *     "hallway", "stonecorridor", "alley", "forest", "city", "mountains",
 *     "quarry", "plain", "parkinglot", "sewerpipe", "underwater",
 *     "drugged", "dizzy", "psychotic".
 * </ul>
 */
struct iPcSoundListener : public virtual iBase
{
  SCF_INTERFACE (iPcSoundListener, 0, 0, 1);

  /// Get the sound listener.
  virtual iSoundListener* GetSoundListener () = 0;

  /// Set environment.
  virtual void SetEnvironment (const char* env) = 0;

  /// Get environment.
  virtual const char* GetEnvironment () = 0;
};

/*
 * This is the sound source property class.
 * <p>
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * <ul>
 * <li>Play: parameters 'method' (int). 0=normal, 1=restart, 2=loop.
 * <li>Stop: no parameters.
 * </ul>
 * <p>
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property:
 * <ul>
 * <li>soundname (string, read/write): name of the sound that will be used.
 * <li>volume (float, read/write): volume between 0 and 1.
 * <li>frequencyfactor (float, read/write): frequency factor: 1=normal, >1 faster.
 * <li>mode3d (int, read/write): Possible values are 0 for disable 3d,
 *     1 for sound relative to listener, and 2 for absolute sound position.
 * <li>position (vector3, read/write): position.
 * <li>velocity (vector3, read/write): velocity.
 * <li>minimumdistance (float, read/write): minimum play distance.
 * <li>maximumdistance (float, read/write): maximum play distance.
 * </ul>
 */
struct iPcSoundSource : public virtual iBase
{
  SCF_INTERFACE (iPcSoundSource, 0, 0, 1);

  /// Get the sound source.
  virtual iSoundSource* GetSoundSource () = 0;

  /// Set the name of the sound wrapper to use.
  virtual void SetSoundName (const char* name) = 0;
  /// Get the name of the sound wrapper that is used.
  virtual const char* GetSoundName () const = 0;
};

#endif // __CEL_PF_SOUND__

