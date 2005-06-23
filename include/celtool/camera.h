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

#ifndef __CEL_PF_CAMFACT__
#define __CEL_PF_CAMFACT__

#include "cstypes.h"
#include "csutil/scf.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "ivaria/reporter.h"
#include "propclass/camera.h"

struct iCelEntity;
struct iObjectRegistry;
struct iGraphics3D;
struct iEngine;
struct iVirtualClock;
struct iCamera;
struct iView;
struct iPcRegion;
struct iPcZoneManager;
class csView;

//---------------------------------------------------------------------------

static void Report (iObjectRegistry* object_reg, const char* msg, ...)
{
  va_list arg;
  va_start (arg, msg);

  csRef<iReporter> rep (CS_QUERY_REGISTRY (object_reg, iReporter));
  if (rep)
    rep->ReportV (CS_REPORTER_SEVERITY_ERROR, "cel.persistence",
    	msg, arg);
  else
  {
    csPrintfV (msg, arg);
    csPrintf ("\n");
    fflush (stdout);
  }

  va_end (arg);
}

//---------------------------------------------------------------------------

/**
 * The representation of a camera.
 */
struct celPcCameraCommon: public celPcCommon
{
protected:
  csRef<iGraphics3D> g3d;
  csRef<iEngine> engine;
  csRef<iVirtualClock> vc;
  csRef<iView> view;
  bool rect_set;
  int rect_x, rect_y, rect_w, rect_h;

  // Fields for the far plane.
  struct
  {
#define FP_MAX_DIST 10000.0	// Maximum visibile distance.
#define FP_INIT_DIST 200.0	// We start from this value.
    bool use_farplane;		// If true then we use a farplane.
    float fixed_distance;	// If < 0 then we use adaptive system.
    // All fields below are for adaptive only.
    float min_fps, max_fps;	// Acceptable FPS interval.
    float min_dist;		// Minimum acceptiable distance.
    bool fps_valid;		// First frame we don't know fps yet.
    float smooth_fps;		// To have less chaotic FPS changes.
    float current_distance;	// Remember current distance.
    csTicks accumulated_elapsed;// Accumulated elapsed time.
  } fp;

  csWeakRef<iPcRegion> region;
  csWeakRef<iPcZoneManager> zonemgr;

  bool clear_zbuf, clear_screen;

public:
  celPcCameraCommon (iObjectRegistry* object_reg);
  virtual ~celPcCameraCommon ();

  bool SetRegion (iPcRegion* region, bool point, const char* name);
  bool SetZoneManager (iPcZoneManager* zonemgr, bool point,
  	const char* regionname, const char* name);
  bool SetZoneManager (const char* entityname, bool point,
  	const char* regionname, const char* name);
  void SetRectangle (int x, int y, int w, int h);

  iCamera* GetCamera () const;
  iView* GetView () const { return view; }

  void SetClearZBuffer (bool flag) { clear_zbuf = flag; }
  bool GetClearZBuffer () const { return clear_zbuf; }
  void SetClearScreen (bool flag) { clear_screen = flag; }
  bool GetClearScreen () const { return clear_screen; }

  // For distance clipping.
  void SetDistanceClipping (float dist);
  void AdaptDistanceClipping (csTicks elapsed_time);

  void DisableDistanceClipping ();
  void EnableFixedDistanceClipping (float dist);
  void EnableAdaptiveDistanceClipping (float min_fps,
	float max_fps, float min_dist);
  bool UseDistanceClipping () const { return fp.use_farplane; }
  bool UseFixedDistanceClipping () const
  {
    return fp.use_farplane && fp.fixed_distance >= 0.0f;
  }
  float GetFixedDistance () const { return fp.fixed_distance; }
  float GetAdaptiveMinFPS () const { return fp.min_fps; }
  float GetAdaptiveMaxFPS () const { return fp.max_fps; }
  float GetAdaptiveMinDistance () const { return fp.min_dist; }

  void SetAutoDraw (bool auto_draw);

  /**
   * In the Save() implementation of the subclass you can call this
   * function to save the common data members.
   */
  void SaveCommon (iCelDataBuffer* databuf);
  /**
   * In the Load() implementation of the subclass you can call this
   * function to load the common data members. Returns false on failure.
   */
  bool LoadCommon (iCelDataBuffer* databuf);

  virtual void Draw ();

  virtual const char* GetName () const { return "pccamera"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual void TickEveryFrame ();
};

#endif // __CEL_PF_CAMFACT__

