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

#ifndef __CEL_PF_HOVER__
#define __CEL_PF_HOVER__

// CS Includes
#include "cstypes.h"
#include "csutil/ref.h"
#include "csutil/scf.h"

struct iPcMesh;

/**
 * Hover stabiliser property class.
 */
struct iPcHover : public virtual iBase
{
  SCF_INTERFACE (iPcHover, 0, 0, 1);

  /**
   * The height for hit beams. Above this height, all height tests
   * return infinity
   */
  virtual void SetHeightBeamCutoff (float chm) = 0;

  /**
   * The offsets to the sides of the objects, to test height to calculate
   * angular orientation with respect to the ground.
   * Smaller is more accurate, but too small and the roll angles will
   * differ in bigger steps (the height beams resolution won't be accurate
   * enough)
   */
  virtual void SetAngularBeamOffset (float abo) = 0;

  /**
   * Height at which to perform the angular correction
   */
  virtual void SetAngularCutoffHeight (float ach) = 0;

  /**
   * How strongly does the object align itself with the terrain?
   * Higher values means that an object going down a hill will snap
   * to the slope. Smaller values and it will slowly align
   */
  virtual void SetAngularCorrectionStrength (float mul) = 0;
  /**
   * Set the various PID factors to affect hovering behaviour.
   */
  virtual void SetFactors (float p, float i, float d) = 0;
  /**
   * Set the height at which the object should hover.
   */
  virtual void SetHoverHeight (float height) = 0;

  /**
   * Turn hovering on (on by default).
   */
  virtual void HoverOn () = 0;

  /**
   * Turn hovering off, until HoverOn () is called.
   */
  virtual void HoverOff () = 0;

  /**
   * Get's the last calculated height for object.
   */
  virtual float GetHeight () = 0;
};

#endif
