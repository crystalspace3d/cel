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
    Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __CEL_PF_ENGFACT__
#define __CEL_PF_ENGFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "csutil/scf.h"
#include "csutil/weakref.h"
#include "csutil/refarr.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "propclass/camera.h"
#include "propclass/region.h"
#include "propclass/linmove.h"
#include "propclass/mesh.h"

struct iCelEntity;
struct iObjectRegistry;
struct iGraphics3D;
struct iEngine;
struct iVirtualClock;
struct iKeyboardDriver;
struct iMouseDriver;
class csView;
class celPcCamera;

/**
 * Factory for engine stuff.
 */
CEL_DECLARE_FACTORY(Camera)
CEL_DECLARE_FACTORY(Region)

struct CameraData
{
  csVector3 worldPos;
  csVector3 worldTar;
  csVector3 worldUp;

  float pitch;
  float yaw;
  float roll; // not implemented yet (no need atm)

  float defaultPitch;
  float defaultYaw;
  float defaultRoll;

  float distance; // the distance between position and target
  float minDistance;
  float maxDistance;

  float turnSpeed;

  float springCoef;
  float InertialDampeningCoef;
  float springLength;

  float swingCoef;
};

/**
 * Camera algorithm class. This is a pure virtual abstract
 * class which is inherited 
 */
class CameraAlgorithm
{
protected:
  celPcCamera* parent;

public:
  CameraAlgorithm (celPcCamera* parent)
  {
    CameraAlgorithm::parent = parent;
  }
  virtual ~CameraAlgorithm () { }

  /**
   * Do the camera calculations but don't apply them to the
   * actual camera yet.
   */
  virtual void DoCameraCalculations (const csTicks elapsed_ticks,
  	const csVector3& actor_pos, const csVector3& actor_eye,
	const float actor_yrot) = 0;

  /**
   * Setup this mode.
   */
  virtual void SetupMode () = 0;

  /**
   * Get the name for this mode
   */
  virtual const char* GetName () = 0;
};

class CAFirstPerson : public CameraAlgorithm
{
public:
  CAFirstPerson (celPcCamera* parent) : CameraAlgorithm (parent) { }

  virtual void DoCameraCalculations (const csTicks elapsed_ticks,
  	const csVector3& actor_pos, const csVector3& actor_eye,
	const float actor_yrot);
  virtual void SetupMode ();
  virtual const char* GetName () { return "firstperson"; }
};

class CAThirdPerson : public CameraAlgorithm
{
public:
  CAThirdPerson (celPcCamera* parent) : CameraAlgorithm (parent) { }

  virtual void DoCameraCalculations (const csTicks elapsed_ticks,
  	const csVector3& actor_pos, const csVector3& actor_eye,
	const float actor_yrot);
  virtual void SetupMode ();
  virtual const char* GetName () { return "thirdperson"; }
};

class CAM64ThirdPerson : public CameraAlgorithm
{
public:
  CAM64ThirdPerson (celPcCamera* parent) : CameraAlgorithm (parent) { }

  virtual void DoCameraCalculations (const csTicks elapsed_ticks,
  	const csVector3& actor_pos, const csVector3& actor_eye,
	const float actor_yrot);
  virtual void SetupMode ();
  virtual const char* GetName () { return "m64_thirdperson"; }
};

class CALaraThirdPerson : public CameraAlgorithm
{
public:
  CALaraThirdPerson (celPcCamera* parent) : CameraAlgorithm (parent) { }

  virtual void DoCameraCalculations (const csTicks elapsed_ticks,
  	const csVector3& actor_pos, const csVector3& actor_eye,
	const float actor_yrot);
  virtual void SetupMode ();
  virtual const char* GetName () { return "lara_thirdperson"; }
};

class CAFreeLook : public CameraAlgorithm
{
public:
  CAFreeLook (celPcCamera* parent) : CameraAlgorithm (parent) { }

  virtual void DoCameraCalculations (const csTicks elapsed_ticks,
  	const csVector3& actor_pos, const csVector3& actor_eye,
	const float actor_yrot);
  virtual void SetupMode ();
  virtual const char* GetName () { return "freelook"; }
};

/**
 * This is a camera property class.
 */
class celPcCamera : public celPcCommon
{
private:
  csRef<iGraphics3D> g3d;
  csRef<iEngine> engine;
  csRef<iKeyboardDriver> kbd;
  csRef<iMouseDriver> mouse;
  csRef<iVirtualClock> vc;
  csRef<iView> view;
  bool use_cd;
  bool rect_set;
  int rect_x, rect_y, rect_w, rect_h;

public://@@@
  bool modeset_needed;
  iPcCamera::CameraMode prev_cammode;
  iPcCamera::CameraMode cammode;
  CameraAlgorithm* camalgo;

  csWeakRef<iPcRegion> region;

  csWeakRef<iPcLinearMovement> pclinmove;
  csWeakRef<iPcMesh> pcmesh;
  void FindSiblingPropertyClasses ();

  CameraData camData[iPcCamera::CameraMode_count];
  // velocity stuff
  float pitchVelocity;
  float yawVelocity;

  // some camera settings
  // NOTE: the offsets are not in the format you think they are, due to speed
  // issues the offset is restricted to being directly behind the actor
  // (no yaw).
  csVector3 firstPersonPositionOffset;
  csVector3 thirdPersonPositionOffset;

  /*
   * If the squared difference between the actual camera position and the
   * ideal position is less than this, then we're no longer in the transition
   * phase.
   */
  float transitionThresholdSquared;
  bool inTransitionPhase;

 /**
  * Used to make sure the camera starts off at the player pos and not
  * outside of the map somewhere.
  */
  bool cameraHasBeenPositioned;

  /// Perform collision detection between the camera target and position.
  bool useCameraCD;

  /**
   * Performs collision detection between the camera position and the player
   * so that objects (ie, a wall) don't block your view
   * @param pseudoTarget the first end of the line segment to test
   * @param pseudoPosition the second end of the line segment to test
   * @param sector the sector to start from (should be the sector where
   * pseudoTarget resides)
   * @return the new world position where it first collides without the world
   */
  csVector3 CalcCollisionPos (const csVector3& pseudoTarget,
  	const csVector3& pseudoPosition, iSector* sector) const;

  void DoCameraTransition ();

  /// Calculates new yaw, pitch, and roll values from the target and position.
  void CalculateFromYawPitchRoll (int mode = -1);

  /**
   * Similar to CalculateFromYawPitchRoll but keep target vector and
   * calculate new position.
   */
  void CalculatePositionFromYawPitchRoll (int mode = -1);

  /**
   * Ensures that the camera distance is within the range for the camera mode
   * @param mode Optional - the camera mode to get/set the data (leave blank
   * for current)
   */
  void EnsureCameraDistance (int mode = -1);

  /**
   * Handles the interpolation between the actual camera data and the ideal
   * camera data for the current mode.
   * @param isElastic when true, the camera will follow Hooke's law and
   * dampening mechanics to interpolate, otherwise the interpolation is instant
   * @param deltaIdeal the change between the ideal data of last frame and
   * this one, used for dampening
   */
  void DoElasticPhysics (bool isElastic,
	const csTicks elapsedTicks,
	const CameraData& deltaIdeal,
	iSector* sector);

  // Get the last position from either linmove or mesh.
  void GetLastPosition (csVector3& actor_pos,
	float& actor_yrot, iSector*& actor_sector);

  /**
   * SetMode() will delay the setting of the mode until later because
   * code may call SetMode() when the mesh and linmove are not yet
   * set up. So this function should be called before rendering to
   * see if there really is a mode to change.
   */
  void CheckModeChange ();

  bool clear_zbuf, clear_screen;

  void SetupEventHandler ();

public:
  celPcCamera (iObjectRegistry* object_reg);
  virtual ~celPcCamera ();

  bool HandleEvent (iEvent& ev);

  iCamera* GetCamera () const;
  iView* GetView () const { return view; }
  bool SetRegion (iPcRegion* region, bool point, const char* name);
  void SetRectangle (int x, int y, int w, int h);

  bool SetMode (iPcCamera::CameraMode cammode, bool use_cd = true);
  iPcCamera::CameraMode GetMode () const { return cammode; }
  bool SetModeName (const char* m, bool use_cd = true);
  const char* GetModeName () const;
  iPcCamera::CameraMode GetNextMode () const;

  iPcLinearMovement* GetLinMove ()
  {
    FindSiblingPropertyClasses ();
    return pclinmove;
  }

  /**
   * Sets the current position of the camera (different for each mode)
   * @param pos The new camera position for the current mode
   */
  void SetPosition (const csVector3& pos, int mode = -1);

  /**
   * Returns the current position of the camera (different for each mode)
   * @return The current camera position for the current mode
   */
  const csVector3& GetPosition (int mode = -1) const;

  /**
   * Sets the current target of the camera (different for each mode)
   * @param tar The new camera target for the current mode
   */
  void SetTarget (const csVector3& tar, int mode = -1);

  /**
   * Returns the current position of the camera (different for each mode)
   * @return The current camera target for the current mode
   */
  const csVector3& GetTarget (int mode = -1) const;

  /**
   * Sets the current up vector of the camera (different for each mode)
   * @param up The new camera up vector for the current mode
   */
  void SetUp (const csVector3& up, int mode = -1);

  /**
   * Returns the current up vector of the camera (different for each mode)
   * @return The current camera up vector for the current mode
   */
  const csVector3& GetUp (int mode = -1) const;

  /**
   * Sets the pitch (up/down) of the camera
   * @param pitch the new pitch of the camera
   * @param mode Optional - the camera mode to apply it to (leave blank for
   * current)
   */
  void SetPitch (float pitch, int mode = -1);

  /**
   * Moves the pitch (up/down) of the camera
   * @param deltaPitch the amount to move from the current pitch
   * @param mode Optional - the camera mode to apply it to (leave blank for
   * current)
   */
  void MovePitch (float deltaPitch, int mode = -1);

  /**
   * Returns the pitch (up/down) of the camera
   * @param mode Optional - the camera mode to get it from (leave blank for
   * current)
   * @return the pitch (up/down) of the camera
   */
  float GetPitch (int mode = -1) const;

  /**
   * Sets the pitch (up/down) velocity of the camera
   * @param pitchVel the velocity of the pitch
   */
  void SetPitchVelocity (float pitchVel);

  /**
   * Gets the pitch (up/down) velocity of the camera
   * @return the pitch (up/down) of the camera
   */
  float GetPitchVelocity () const;

  /**
   * Sets the yaw (left/right) of the camera
   * @param yaw the new yaw of the camera
   * @param mode Optional - the camera mode to apply it to (leave blank for
   * current)
   */
  void SetYaw (float yaw, int mode = -1);

  /**
   * Moves the yaw (left/right) of the camera
   * @param deltaYaw the amount to move from the current yaw
   * @param mode Optional - the camera mode to apply it to (leave blank for
   * current)
   */
  void MoveYaw (float deltaYaw, int mode = -1);

  /**
   * Returns the yaw (left/right) of the camera
   * @param mode Optional - the camera mode to get it from (leave blank for
   * current)
   * @return the yaw (left/right) of the camera
   */
  float GetYaw (int mode = -1) const;

  /**
   * Sets the yaw (up/down) velocity of the camera
   * @param yawVel the velocity of the yaw
   */
  void SetYawVelocity (float yawVel);

  /**
   * Gets the yaw (up/down) velocity of the camera
   * @return the yaw (up/down) of the camera
   */
  float GetYawVelocity () const;

  /**
   * Sets the distance from the camera position to its target
   * @param distance the new distance
   * @param mode Optional - the camera mode to apply it to (leave blank for
   * current)
   */
  void SetDistance (float distance, int mode = -1);

  /**
   * Moves the distance from the camera position to its target
   * @param deltaYaw the amount to displace the current distance
   * @param mode Optional - the camera mode to apply it to (leave blank for
   * current)
   */
  void MoveDistance (float deltaDistance, int mode = -1);

  /**
   * Returns the distance from the camera position to its target
   * @param mode Optional - the camera mode to get it from (leave blank for
   * current)
   * @return the distance from the camera position to its target
   */
  float GetDistance (int mode = -1) const;

  /**
   * Resets the actual camera data to the player position and stuff
   */
  void ResetActualCameraData ();

  /**
   * Moves decides, based on the camera mode, whether the camera should be
   * rotated along-side the player
   * @return true if the camera should be rotated, false otherwise
   */
  bool RotateCameraWithPlayer () const;

  /**
   * Returns the forward vector of the camera
   * @param mode Optional - the camera mode to get the data from (leave blank
   * for current)
   * @return the forward vector
   */
  csVector3 GetForwardVector (int mode = -1) const;

  /**
   * Returns the forward vector of the camera
   * @param mode Optional - the camera mode to get the data from (leave blank
   * for current)
   * @return the forward vector
   */
  csVector3 GetRightVector (int mode = -1) const;

  /**
   * Returns the min distance from the camera position to the target
   * @param mode Optional - the camera mode to get the data from (leave blank
   * for current)
   * @return the min distance
   */
  float GetMinDistance (int mode = -1) const;

  /**
   * Sets the min distance from the camera position to the target
   * @param dist the min distance
   * @param mode Optional - the camera mode to set the data (leave blank for
   * current)
   */
  void SetMinDistance (float dist, int mode = -1);

  /**
   * Returns the max distance from the camera position to the target
   * @param mode Optional - the camera mode to get the data from (leave blank
   * for current)
   * @return the max distance
   */
  float GetMaxDistance (int mode = -1) const;

  /**
   * Sets the max distance from the camera position to the target
   * @param dist the max distance
   * @param mode Optional - the camera mode to set the data (leave blank for
   * current)
   */
  void SetMaxDistance (float dist, int mode = -1);

  /**
   * Returns the turning speed of the camera (ignored for most camera modes)
   * @param mode Optional - the camera mode to get the data from (leave blank
   * for current)
   * @return the turning speed
   */
  float GetTurnSpeed (int mode = -1) const;

  /**
   * Sets the turning speed of the camera (ignored for most camera modes)
   * @param speed the turning speed
   * @param mode Optional - the camera mode to set the data (leave blank for
   * current)
   */
  void SetTurnSpeed (float speed, int mode = -1);

  /**
   * Returns the spring coefficient of the camera
   * @param mode Optional - the camera mode to get the data from (leave blank
   * for current)
   * @return the spring coefficient
   */
  float GetSpringCoef (int mode = -1) const;

  /**
   * Sets the spring coefficient of the camera
   * @param coef the spring coefficient
   * @param mode Optional - the camera mode to set the data (leave blank for
   * current)
   */
  void SetSpringCoef (float coef, int mode = -1);

  /**
   * Returns the dampening coefficient of the camera
   * @param mode Optional - the camera mode to get the data from (leave blank
   * for current)
   * @return the dampening coefficient
   */
  float GetDampeningCoef (int mode = -1) const;

  /**
   * Sets the dampening coefficient of the camera
   * @param coef the dampening coefficient
   * @param mode Optional - the camera mode to set the data (leave blank for
   * current)
   */
  void SetDampeningCoef (float coef, int mode = -1);

  /**
   * Returns the spring length of the camera
   * @param mode Optional - the camera mode to get the data from (leave blank
   * for current)
   * @return the spring length
   */
  float GetSpringLength (int mode = -1) const;

  /**
   * Sets the spring length of the camera
   * @param length the spring length
   * @param mode Optional - the camera mode to set the data (leave blank for
   * current)
   */
  void SetSpringLength (float length, int mode = -1);

  /**
   * Returns whether the camera is performing CD or not
   * @return true if using CD, false otherwise
   */
  bool CheckCameraCD () const;

  /**
   * Sets whether the camera is performing CD or not
   * @param useCD true if the camera should perform CD, false otherwise
   */
  void SetCameraCD (bool useCD);

  /**
   * Gets the camera transition threshold (the distance between camera
   * position and ideal where the camera ceases to be in transition)
   * @return the camera transition threshold
   */
  float GetTransitionThreshold () const;

  /**
   * Sets the camera transition threshold (the distance between camera
   * position and ideal where the camera ceases to be in transition)
   * @param threshold the camera transition threshold
   */
  void SetTransitionThreshold (float threshold);

  /**
   * Returns the default (starting) pitch of the specific camera mode
   * @param mode Optional - the camera mode to get the data from (leave blank
   * for current)
   * @return the default (starting) pitch
   */
  float GetDefaultPitch (int mode = -1) const;

  /**
   * Sets the default (starting) pitch for the specific camera mode
   * @param pitch the new default (starting) pitch
   * @param mode Optional - the camera mode to set the data (leave blank for
   * current)
   */
  void SetDefaultPitch (float pitch, int mode = -1);

  /**
   * Returns the default (starting) yaw of the specific camera mode
   * @param mode Optional - the camera mode to get the data from (leave blank
   * for current)
   * @return the default (starting) yaw
   */
  float GetDefaultYaw (int mode = -1) const;

  /**
   * Sets the default (starting) yaw for the specific camera mode
   * @param yaw the new default (starting) yaw
   * @param mode Optional - the camera mode to set the data (leave blank for
   * current)
   */
  void SetDefaultYaw (float yaw, int mode = -1);

  /**
   * Returns the swing coefficient of the specific camera mode
   * @param mode Optional - the camera mode to get the data from (leave blank
   * for current)
   * @return the swing coefficient
   */
  float GetSwingCoef (int mode = -1) const;

  /**
   * Sets the swing coefficient of the specific camera mode
   * @param swingCoef the new swing coefficient
   * @param mode Optional - the camera mode to get the data from (leave blank
   * for current)
   */
  void SetSwingCoef (float swingCoef, int mode = -1);

  void SetClearZBuffer (bool flag) { clear_zbuf = flag; }
  bool GetClearZBuffer () const { return clear_zbuf; }
  void SetClearScreen (bool flag) { clear_screen = flag; }
  bool GetClearScreen () const { return clear_screen; }

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pccamera"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  struct PcCamera : public iPcCamera
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcCamera);
    virtual bool SetRegion (iPcRegion* region, bool point = true,
	const char* name = 0)
    {
      return scfParent->SetRegion(region,point,name);
    }
    virtual bool SetMode (CameraMode m, bool use_cd = true)
    {
      return scfParent->SetMode (m, use_cd);
    }
    virtual CameraMode GetMode () const
    {
      return scfParent->GetMode ();
    }
    virtual bool SetModeName (const char* m, bool use_cd = true)
    {
      return scfParent->SetModeName (m, use_cd);
    }
    virtual const char* GetModeName () const
    {
      return scfParent->GetModeName ();
    }
    virtual CameraMode GetNextMode () const
    {
      return scfParent->GetNextMode ();
    }

    virtual void SetSpringParameters (float springCoef,
  	float intertialDampeningCoef, float springLength)
    {
      scfParent->SetSpringCoef (springCoef);
      scfParent->SetDampeningCoef (intertialDampeningCoef);
      scfParent->SetSpringLength (springLength);
    }
    virtual void SetMinMaxCameraDistance (float minDistance,
  	float maxDistance)
    {
      scfParent->SetMinDistance (minDistance);
      scfParent->SetMaxDistance (maxDistance);
    }
    virtual void SetTurnSpeed (float turnSpeed)
    {
      scfParent->SetTurnSpeed (turnSpeed);
    }
    virtual void SetSwingCoef (float swingCoef)
    {
      scfParent->SetSwingCoef (swingCoef);
    }
    virtual void SetFirstPersonOffset (const csVector3& offset)
    {
      scfParent->firstPersonPositionOffset = offset;
    }
    virtual void SetThirdPersonOffset (const csVector3& offset)
    {
      scfParent->thirdPersonPositionOffset = offset;
    }

    virtual void SetRectangle (int x, int y, int w, int h)
    {
      scfParent->SetRectangle (x, y, w, h);
    }
    virtual iCamera* GetCamera () const
    {
      return scfParent->GetCamera ();
    }
    virtual iView* GetView () const
    {
      return scfParent->GetView ();
    }
    virtual void SetClearZBuffer (bool flag)
    {
      scfParent->SetClearZBuffer (flag);
    }
    virtual bool GetClearZBuffer () const
    {
      return scfParent->GetClearZBuffer ();
    }
    virtual void SetClearScreen (bool flag)
    {
      scfParent->SetClearScreen (flag);
    }
    virtual bool GetClearScreen () const
    {
      return scfParent->GetClearScreen ();
    }
  } scfiPcCamera;

  // Not an embedded interface to avoid circular reference!!!
  class EventHandler : public iEventHandler
  {
  private:
    celPcCamera* parent;

  public:
    EventHandler (celPcCamera* parent)
    {
      SCF_CONSTRUCT_IBASE (0);
      EventHandler::parent = parent;
    }
    virtual ~EventHandler () { }
    SCF_DECLARE_IBASE;
    virtual bool HandleEvent (iEvent& ev)
    {
      return parent->HandleEvent (ev);
    }
  } *scfiEventHandler;
};

/**
 * This is a region property class.
 */
class celPcRegion : public celPcCommon
{
private:
  bool empty_sector;
  char* worlddir;
  // If empty_sector is true then worldfile is the name of the sector to create.
  char* worldfile;
  char* regionname;
  bool loaded;

  // This property class maintains private child entities
  // which are used for collision detection.
  csRefArray<iCelEntity> entities;

  enum propids
  {
    propid_worlddir = 0,
    propid_worldfile,
    propid_regionname,
    propid_load
  };

  static Property* properties;
  static int propertycount;

  static void UpdateProperties (iObjectRegistry* object_reg);

public:
  celPcRegion (iObjectRegistry* object_reg);
  virtual ~celPcRegion ();

  /// Set the worldfile directory on VFS and filename.
  void SetWorldFile (const char* vfsdir, const char* name);
  /// Get the worldfile directory name.
  const char* GetWorldDir () const { return worlddir; }
  /// Get the worldfile filename.
  const char* GetWorldFile () const { return worldfile; }
  /// Set a name for this region.
  void SetRegionName (const char* name);
  /// Get the name of this region.
  const char* GetRegionName () const { return regionname; }
  /// Create an empty sector.
  void CreateEmptySector (const char* name);
  /**
   * Load an entire region according to worldfile, worlddir,
   * regionname and create entities for all meshes in this region
   * unless there is already an entity for them (an addon may have
   * created them for example).
   */
  bool Load ();
  /**
   * Unload the region by removing all entities from this region
   * and removing it from the engines region list.
   */
  void Unload ();
  /**
   * Get the start sector by searching for the given "name"
   * in all camerapositions within this world, if the position
   * is found it returns the belonging sector, otherwise it's
   * searching for the sector "room".
   */
  iSector* GetStartSector (const char* name);
  /**
   * Get the startposition queried by "name" searching through
   * all camerapositions within this world.
   * Otherwise returns first cameraposition.
   */
  csVector3 GetStartPosition (const char* name);
  /// Points the given camera to the startingposition named "name".
  void PointCamera (iPcCamera* pccamera, const char* name);
  /// Find a sector named "name".
  iSector* FindSector (const char* name);

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcregion"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  virtual bool PerformAction (csStringID, iCelParameterBlock* params);

  struct PcRegion : public iPcRegion
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcRegion);
    virtual void SetWorldFile (const char* vfsdir, const char* name)
    {
      scfParent->SetWorldFile (vfsdir, name);
    }
    virtual const char* GetWorldDir () const
    {
      return scfParent->GetWorldDir ();
    }
    virtual const char* GetWorldFile () const
    {
      return scfParent->GetWorldFile ();
    }
    virtual void CreateEmptySector (const char* name)
    {
      scfParent->CreateEmptySector (name);
    }
    virtual void SetRegionName (const char* name)
    {
      scfParent->SetRegionName (name);
    }
    virtual const char* GetRegionName () const
    {
      return scfParent->GetRegionName ();
    }
    virtual iSector* FindSector(const char* name)
    {
      return scfParent->FindSector(name);
    }
    virtual bool Load ()
    {
      return scfParent->Load ();
    }
    virtual void Unload ()
    {
      scfParent->Unload ();
    }
    virtual iSector* GetStartSector (const char* name = 0)
    {
      return scfParent->GetStartSector (name);
    }
    virtual csVector3 GetStartPosition (const char* name = 0)
    {
      return scfParent->GetStartPosition (name);
    }
    virtual void PointCamera (iPcCamera* pccamera, const char* name = 0)
    {
      scfParent->PointCamera (pccamera, name);
    }
  } scfiPcRegion;
};

#endif // __CEL_PF_ENGFACT__

