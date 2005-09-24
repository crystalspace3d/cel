/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein
  
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

struct iSndSysListener;
struct iSndSysSource;

/**
 * This is the sound listener property class. There can really be only
 * one listener at any moment. It represents the listener (i.e. player of the
 * game).
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
 * <li>distancefactor (float, read/write): distance factor.
 * <li>rollofffactor (float, read/write): rolloff factor.
 * <li>headsize (float, read/write): headsize.
 * </ul>
 */
struct iPcSoundListener : public virtual iBase
{
  SCF_INTERFACE (iPcSoundListener, 0, 0, 1);

  /// Get the sound listener.
  virtual iSndSysListener* GetSoundListener () = 0;
};

/*
 * This is the sound source property class.
 * <p>
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * <ul>
 * <li>Unpause: no parameters.
 * <li>Pause: no parameters.
 * </ul>
 * <p>
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property:
 * <ul>
 * <li>soundname (string, read/write): name of the sound that will be used.
 * <li>volume (float, read/write): 0=off, 1=normal, >1 is above normal.
 * <li>directionalradiation (float, read/write): directional radiation.
 * <li>position (vector3, read/write): position.
 * <li>minimumdistance (float, read/write): minimum play distance.
 * <li>maximumdistance (float, read/write): maximum play distance.
 * <li>loop (bool, read/write): true=loop, false=don't loop.
 * </ul>
 */
struct iPcSoundSource : public virtual iBase
{
  SCF_INTERFACE (iPcSoundSource, 0, 0, 1);

  /// Get the sound source.
  virtual iSndSysSource* GetSoundSource () = 0;

  /// Set the name of the sound wrapper to use.
  virtual void SetSoundName (const char* name) = 0;
  /// Get the name of the sound wrapper that is used.
  virtual const char* GetSoundName () const = 0;
};

#endif // __CEL_PF_SOUND__

