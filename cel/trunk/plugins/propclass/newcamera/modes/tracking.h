/*
    Crystal Space Entity Layer
    Copyright (C) 2007 by Amir Taaki

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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __CEL_LARA_TRACK_CAMERA_MODE_FACTORY__
#define __CEL_LARA_TRACK_CAMERA_MODE_FACTORY__

// CS Includes
#include "csgeom/transfrm.h"
#include "iutil/virtclk.h"

// CEL Includes
#include "propclass/newcamera.h"

struct iMovable;
struct iCelPlLayer;
struct iCollideSystem;

namespace celCameraMode
{

class Tracking : public scfImplementation2<Tracking, iPcmNewCamera::Tracking,
  scfFakeInterface<iPcmNewCamera::General> >
{
private:
  // calculate spring force based on spring stretched length
  float SpringForce (const float movement);
  // pan camera around a target
  void PanAroundPlayer (const csVector3 &playpos, float elapsedsecs);
  // do your collision detection maagick!
  void FindCorrectedTransform (float elapsedsecs);

  const csVector3 &GetAnchorPosition ();
  const csVector3 &GetAnchorDirection ();
  // this function should absolutely never be called unless
  // targetstate == TARGET_OBJ
  const csVector3 &GetTargetPosition ();

  iPcNewCamera* parent;
  csVector3 origin, target, up;
  csVector3 corrorigin, corrtarget;

  // the target we track if targetstate == TARGET_OBJ
  iMovable* tracktarget;
  TargetState targetstate;

  struct Accelerator
  {
    void Accelerate (int direction, float elapsedsecs);
    float topspeed;
    float speed, accel;
  } pan, tilt;
  // panning direction and speed
  PanDirection pandir;
  // tilt direction and speed
  TiltDirection tiltdir;

  // because you don't want to be looking at the targets feet
  float targetyoffset;
  // offset for origin position. Described as elevation angle and distance outwards
  struct
  {
    float angle, dist;
  } posoff;
  // the in and out springs relaxed normal length
  float relaxspringlen;
  // the minimum value for calculated spring... to avoid stupid values
  float minspring;

  // -- collision detection related stuff
  // did we correct the camera last frame? used for interpolating outwards movement
  bool was_corrected;
  // speed of zoom out correction
  float zoomoutcorrspeed;

  // used to lookup entities
  csWeakRef<iCelPlLayer> pl;
  // to compute elapsed time
  csRef<iVirtualClock> vc;
  // used for hitbeam in collision detection
  csRef<iCollideSystem> cdsys;

  // Has this camera been initialised yet?
  bool init_reset;
public:
  Tracking (iBase* p);
  Tracking (iCelPlLayer* pl, iVirtualClock* vc, iCollideSystem* cdsys);
  virtual ~Tracking ();

  bool SetTargetEntity (const char* name);
  void SetTargetState (TargetState targetstate);
  //virtual void SetTargetMesh (const char* name);
  //virtual void SetTargetPosition (const csVector3 &pos);
  void SetTargetYOffset (float targetyoffset);

  void Pan (PanDirection pdir);
  PanDirection GetPanDirection () const;
  void SetPanSpeed (float pspeed);
  float GetPanSpeed () const;
  void SetPanAcceleration (float paccel);
  float GetPanAcceleration () const;

  void Tilt (TiltDirection tdir);
  TiltDirection GetTiltDirection () const;
  void SetTiltSpeed (float tspeed);
  float GetTiltSpeed () const;
  void SetTiltAcceleration (float taccel);
  float GetTiltAcceleration () const;

  TargetState GetTargetState ();

  void SetZoomOutCorrectionSpeed (float zoomspeed);
  float GetZoomOutCorrectionSpeed () const;

  void SetOffsetAngle (float angle);
  float GetOffsetAngle () const;
  void SetOffsetDistance (float dist);
  float GetOffsetDistance () const;
  void SetFollowSpringLength (float slen);
  float GetFollowSpringLength () const;
  void SetFollowMinimumSpringFactor (float smin);
  float SetFollowMinimumSpringFactor () const;
  bool DrawAttachedMesh () const;
  bool DecideCameraState ();

  bool ResetCamera ();

  // Interface Functions
  void SetParentCamera (iPcNewCamera* camera);
  bool UseSpringPos () const;
  bool UseSpringOrigin () const;
  bool UseSpringTarget () const;
  bool UseSpringUp () const;
  bool AllowCollisionDetection () const;
  bool GetCollisionDetection () const;
  float GetSpringCoefficient () const;
  void SetSpringCoefficient (float s);
  float GetOriginSpringCoefficient () const;
  void SetOriginSpringCoefficient (float s);
  float GetTargetSpringCoefficient () const;
  void SetTargetSpringCoefficient (float s);
  float GetUpSpringCoefficient () const;
  void SetUpSpringCoefficient (float s);
  const csVector3 &GetPosition () const;
  const csVector3& GetOrigin () const;
  const csVector3 &GetTarget () const;
  const csVector3 &GetUp () const;
};

}

#endif // __CEL_LARA_TRACK_CAMERA_MODE_FACTORY__
