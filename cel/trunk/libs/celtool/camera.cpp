#include "cssysdef.h"
#include "celtool/camera.h"
#include "propclass/zone.h"
#include "propclass/region.h"
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/persist.h"
#include "physicallayer/datatype.h"
#include "behaviourlayer/behave.h"
#include "iutil/objreg.h"
#include "iutil/object.h"
#include "iutil/eventq.h"
#include "iutil/event.h"
#include "iutil/evdefs.h"
#include "iutil/virtclk.h"
#include "iengine/engine.h"
#include "iengine/camera.h"
#include "iengine/region.h"
#include "iengine/campos.h"
#include "iengine/sector.h"
#include "cstool/csview.h"
#include "ivaria/view.h"
#include "ivideo/graph3d.h"

//---------------------------------------------------------------------------

celPcCameraCommon::celPcCameraCommon (iObjectRegistry* object_reg)
  : celPcCommon (object_reg)
{
  engine = CS_QUERY_REGISTRY (object_reg, iEngine);
  g3d = CS_QUERY_REGISTRY (object_reg, iGraphics3D);
  view = csPtr<iView> (new csView (engine, g3d));

  rect_set = false;
  vc = CS_QUERY_REGISTRY (object_reg, iVirtualClock);
  CS_ASSERT (vc != 0);

  clear_zbuf = false;
  clear_screen = false;

  DisableDistanceClipping ();

  pl->CallbackEveryFrame ((iCelTimerListener*)this, cscmdProcess);
}

celPcCameraCommon::~celPcCameraCommon ()
{
}

bool celPcCameraCommon::SetRegion (iPcRegion* newregion, bool point,
	const char *name)
{
  region = newregion;
  zonemgr = 0;

  if (point)
  {
    csRef<iPcCamera> camera = SCF_QUERY_INTERFACE (this, iPcCamera);

    if (region)
      region->PointCamera (camera, name);
    else
    {
      // camera->GetCamera ()->SetSector (0);
      camera->GetCamera ()->GetTransform ().SetOrigin (csVector3(0,0,0));
    }
  }

  return true;
}

bool celPcCameraCommon::SetZoneManager (iPcZoneManager* newzonemgr,
	bool point, const char* regionname, const char *name)
{
  region = 0;
  zonemgr = newzonemgr;

  if (point)
  {
    csRef<iPcCamera> camera = SCF_QUERY_INTERFACE (this, iPcCamera);

    if (zonemgr)
      zonemgr->PointCamera (entity->GetName (), regionname, name);
    else
    {
      // camera->GetCamera ()->SetSector (0);
      camera->GetCamera ()->GetTransform ().SetOrigin (csVector3(0,0,0));
    }
  }

  return true;
}

void celPcCameraCommon::SetRectangle (int x, int y, int w, int h)
{
  rect_x = x;
  rect_y = y;
  rect_w = w;
  rect_h = h;
  view->SetRectangle (x, y, w, h);
  rect_set = true;
}

iCamera* celPcCameraCommon::GetCamera () const
{
  return view->GetCamera ();
}

void celPcCameraCommon::DisableDistanceClipping ()
{
  fp.use_farplane = false;
  if (view && view->GetCamera ())
    view->GetCamera ()->SetFarPlane (0);
}

void celPcCameraCommon::EnableFixedDistanceClipping (float dist)
{
  fp.use_farplane = true;
  fp.fixed_distance = dist;
  SetDistanceClipping (dist);
}

void celPcCameraCommon::EnableAdaptiveDistanceClipping (float min_fps,
	float max_fps, float min_dist)
{
  fp.use_farplane = true;
  fp.fixed_distance = -1.0f;
  fp.min_fps = min_fps;
  fp.max_fps = max_fps;
  fp.min_dist = min_dist;
  fp.fps_valid = false;
  fp.smooth_fps = 30.0f;
  fp.accumulated_elapsed = 0;
  fp.current_distance = -1.0f;
}

void celPcCameraCommon::SetDistanceClipping (float dist)
{
  csVector3 v1 (0, 0, dist), v2 (0, 1, dist), v3 (1, 0, dist);
  csPlane3 p (v1, v2, v3);
  view->GetCamera ()->SetFarPlane (&p);
  fp.current_distance = dist;
}

// 1. qsqrt vs sqrt
// 2. don't use static fields, may conflict with other camera instances
// 3. use virtual clock and not csGetTicks(), movie recorder, speed, elapsed
// 4. GetDistanceClipping() is expensive, better remember current distance.
// 5. Adapts very slow to lower clipping plane.

void celPcCameraCommon::AdaptDistanceClipping (csTicks elapsed_time)
{
  if (!fp.use_farplane) return;
  if (fp.fixed_distance > 0.0f) return;

  float curr_fps;		// FPS calculated from the last frame.

  // When we are called for the first time, we just initialize some variables
  // and exit.
  if (!fp.fps_valid)
  {
    fp.fps_valid = true;
    return;
  }

  if (!elapsed_time)
    curr_fps = 1000.0;
  else
    curr_fps = 1000.0 / float (elapsed_time);
  fp.smooth_fps = 0.5*curr_fps + 0.5*fp.smooth_fps;

  fp.accumulated_elapsed += elapsed_time;
  if (fp.accumulated_elapsed > 500)
  {
    fp.accumulated_elapsed = 0;
    float change;

    float curr_dist = fp.current_distance;
    if (curr_dist < 0.0f)
      curr_dist = FP_INIT_DIST;

    if (fp.smooth_fps < fp.min_fps)
    {
      change = (fp.min_fps - fp.smooth_fps);
      curr_dist -= change;
      if (curr_dist < fp.min_dist) curr_dist = fp.min_dist;
      SetDistanceClipping (curr_dist);
    }
    else if (fp.smooth_fps > fp.max_fps)
    {
      change = (fp.smooth_fps - fp.max_fps);
      curr_dist += change;
      if (curr_dist > FP_MAX_DIST) curr_dist = FP_MAX_DIST;
      SetDistanceClipping (curr_dist);
    }
  }
}

void celPcCameraCommon::SetAutoDraw(bool auto_draw)
{
  if (auto_draw)
    pl->CallbackEveryFrame ((iCelTimerListener*)this, cscmdProcess);
  else
    pl->RemoveCallbackEveryFrame ((iCelTimerListener*)this, cscmdProcess);
}

void celPcCameraCommon::Draw ()
{
  // First get elapsed time from the virtual clock.
  csTicks elapsed_time = vc->GetElapsedTicks ();

  AdaptDistanceClipping (elapsed_time);

  // Tell 3D driver we're going to display 3D things.
  if (g3d->BeginDraw (engine->GetBeginDrawFlags () | CSDRAW_3DGRAPHICS
    	| (clear_zbuf ? CSDRAW_CLEARZBUFFER : 0)
	| (clear_screen ? CSDRAW_CLEARSCREEN : 0)))
    view->Draw ();
}

void celPcCameraCommon::TickEveryFrame ()
{
  Draw();
}

void celPcCameraCommon::SaveCommon (iCelDataBuffer* databuf)
{
  csRef<iCelPropertyClass> pc;
  if (region) pc = SCF_QUERY_INTERFACE (region, iCelPropertyClass);
  databuf->Add (pc);
  if (zonemgr) pc = SCF_QUERY_INTERFACE (zonemgr, iCelPropertyClass);
  databuf->Add (pc);
  databuf->Add (view->GetCamera()->GetSector()->QueryObject()->GetName());
  const csTransform& tr = view->GetCamera ()->GetTransform ();
  databuf->Add (tr.GetO2TTranslation ());

  databuf->Add (tr.GetO2T ().m11);
  databuf->Add (tr.GetO2T ().m12);
  databuf->Add (tr.GetO2T ().m13);
  databuf->Add (tr.GetO2T ().m21);
  databuf->Add (tr.GetO2T ().m22);
  databuf->Add (tr.GetO2T ().m23);
  databuf->Add (tr.GetO2T ().m31);
  databuf->Add (tr.GetO2T ().m32);
  databuf->Add (tr.GetO2T ().m33);

  databuf->Add (rect_set);
  databuf->Add ((uint16)rect_x);
  databuf->Add ((uint16)rect_y);
  databuf->Add ((uint16)rect_w);
  databuf->Add ((uint16)rect_h);

  databuf->Add (clear_zbuf);
  databuf->Add (clear_screen);
}

bool celPcCameraCommon::LoadCommon (iCelDataBuffer* databuf)
{
  csMatrix3 m_o2t;
  csVector3 v_o2t;

  iCelPropertyClass* pc = databuf->GetPC ();
  if (pc)
  {
    region = SCF_QUERY_INTERFACE (pc, iPcRegion);
    if (region)
      SetRegion (region, false, 0);
  }

  pc = databuf->GetPC ();
  if (pc)
  {
    zonemgr = SCF_QUERY_INTERFACE (pc, iPcZoneManager);
    if (zonemgr)
      SetZoneManager (zonemgr, false, 0, 0);
  }

  const char* sectname = databuf->GetString ()->GetData ();
  iSector* sector;
  if (region) sector = region->FindSector (sectname);
  else sector = engine->FindSector (sectname);
  if (!sector)
  {
    Report (object_reg,"Illegal sector '%s' specified.  Cannot load.",
    	sectname);
    return false;
  }
  databuf->GetVector3 (v_o2t);

  m_o2t.m11 = databuf->GetFloat ();
  m_o2t.m12 = databuf->GetFloat ();
  m_o2t.m13 = databuf->GetFloat ();
  m_o2t.m21 = databuf->GetFloat ();
  m_o2t.m22 = databuf->GetFloat ();
  m_o2t.m23 = databuf->GetFloat ();
  m_o2t.m31 = databuf->GetFloat ();
  m_o2t.m32 = databuf->GetFloat ();
  m_o2t.m33 = databuf->GetFloat ();

  view->GetCamera ()->SetSector (sector);
  csOrthoTransform tr (m_o2t, v_o2t);
  view->GetCamera ()->SetTransform (tr);

  rect_set = databuf->GetBool ();
  rect_x = databuf->GetUInt16 ();
  rect_y = databuf->GetUInt16 ();
  rect_w = databuf->GetUInt16 ();
  rect_h = databuf->GetUInt16 ();

  clear_zbuf = databuf->GetBool ();
  clear_screen = databuf->GetBool ();

  if (rect_set)
    view->SetRectangle (rect_x, rect_y, rect_w, rect_h);

  return true;
}

#define CAMERA_SERIAL 1

csPtr<iCelDataBuffer> celPcCameraCommon::Save ()
{
  csRef<iCelDataBuffer> databuf = pl->CreateDataBuffer (CAMERA_SERIAL);
  SaveCommon (databuf);
  return csPtr<iCelDataBuffer> (databuf);
}

bool celPcCameraCommon::Load (iCelDataBuffer* databuf)
{
  int serialnr = databuf->GetSerialNumber ();
  if (serialnr != CAMERA_SERIAL)
  {
    Report (object_reg, "serialnr != CAMERA_SERIAL.  Cannot load.");
    return false;
  }
  return LoadCommon (databuf);
}

//---------------------------------------------------------------------------

