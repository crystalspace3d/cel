/*
    Crystal Space Entity Layer
    Copyright (C) 2011 by Jorrit Tyberghein

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

#include "cssysdef.h"
#include "cstool/initapp.h"
#include "cstool/procmesh.h"
#include "csutil/objreg.h"
#include "csutil/weakref.h"
#include "csutil/event.h"
#include "csutil/cfgacc.h"
#include "csutil/scanstr.h"
#include "csutil/inputdef.h"
#include "csgeom/math3d.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/plugin.h"
#include "iutil/object.h"
#include "iutil/virtclk.h"
#include "iutil/csinput.h"
#include "ivideo/graph3d.h"
#include "ivideo/graph2d.h"
#include "ivaria/reporter.h"
#include "iengine/sector.h"
#include "iengine/camera.h"
#include "iengine/rendermanager.h"
#include "iengine/movable.h"

#include "gridinv.h"
#include "physicallayer/propclas.h"
#include "physicallayer/entity.h"
#include "physicallayer/entitytpl.h"
#include "propclass/mesh.h"

SCF_IMPLEMENT_FACTORY (celUIGridInventory)

//--------------------------------------------------------------------------

class InvListener : public scfImplementation1<InvListener, iPcInventoryListener>
{
private:
  celUIGridInventory* ui;

public:
  InvListener (celUIGridInventory* ui) : scfImplementationType (this), ui (ui) { }
  virtual ~InvListener () { }

  virtual void AddChild (iPcInventory* inventory, iCelEntity* entity)
  {
    ui->Refresh ();
  }
  virtual void RemoveChild (iPcInventory* inventory, iCelEntity* entity)
  {
    ui->Refresh ();
  }
  virtual void AddChildTemplate (iPcInventory* inventory,
      iCelEntityTemplate* tpl, int amount)
  {
    ui->Refresh ();
  }
  virtual void RemoveChildTemplate (iPcInventory* inventory,
      iCelEntityTemplate* tpl, int amount)
  {
    ui->Refresh ();
  }
};

//--------------------------------------------------------------------------

csRef<iString> DefaultInfo::GetName (iCelEntity* ent)
{
  csRef<scfString> str;
  str.AttachNew (new scfString ());
  if (!ent->GetName ())
  {
    csStringID tmpID = ent->GetTemplateNameID ();
    if (tmpID != csInvalidStringID)
      str->Append (pl->FetchString (tmpID));
    else
      str->Append ("?");
  }
  else str->Append (ent->GetName ());
  return str;
}

csRef<iString> DefaultInfo::GetName (iCelEntityTemplate* ent, int count)
{
  csRef<scfString> str;
  str.AttachNew (new scfString ());
  str->Format ("%s (%d)", ent->GetName (), count);
  return str;
}

iMeshFactoryWrapper* DefaultInfo::GetMeshFactory (iCelEntity* entity)
{
  csRef<iPcMesh> pcmesh = celQueryPropertyClassEntity<iPcMesh> (entity);
  if (!pcmesh) return 0;
  if (!pcmesh->GetFactoryName ()) return 0;
  return engine->FindMeshFactory (pcmesh->GetFactoryName ());
}

iMeshFactoryWrapper* DefaultInfo::GetMeshFactory (iCelEntityTemplate* tpl, int count)
{
  // @@@ Not implemented?
  return 0;
}

//--------------------------------------------------------------------------

bool GridLayouter::NextSlot ()
{
  if (verticalscroll)
  {
    ix++;
    if (ix >= horcount)
    {
      ix = 0;
      iy++;
      if (iy >= vercount)
        return false;
    }
  }
  else
  {
    iy++;
    if (iy >= vercount)
    {
      iy = 0;
      ix++;
      if (ix >= horcount)
        return false;
    }
  }
  return true;
}

size_t GridLayouter::FirstSlot ()
{
  ix = 0;
  iy = 0;
  if (verticalscroll)
    return firsty * horcount;
  else
    return firstx * vercount;
}

void GridLayouter::Layout (celUIGridInventory* inv, csArray<GridEntry>& grid)
{
  const InvStyle& style = inv->GetStyle ();
  iGraphics3D* g3d = inv->GetG3D ();
  int w = g3d->GetWidth ();
  int h = g3d->GetHeight ();
  horcount = w / (style.buttonw + style.marginhor);
  vercount = h / (style.buttonh + style.marginver);

  for (size_t i = 0 ; i < grid.GetSize () ; i++)
    grid[i].x = grid[i].y = -1;

  size_t i = FirstSlot ();
  for ( ; i < grid.GetSize () ; i++)
  {
    GridEntry& g = grid[i];
    g.x = style.marginhor + ix * (style.buttonw + style.marginhor);
    g.y = style.marginver + iy * (style.buttonh + style.marginver);
    if (!NextSlot ()) return;
  }
}

void GridLayouter::Scroll (int dx, int dy)
{
  if (verticalscroll) dx = 0;
  else dy = 0;
  firstx += dx;
  firsty += dy;
  if (firstx < 0) firstx = 0;
  if (firsty < 0) firsty = 0;
}

GridEntry* GridLayouter::GetSelected (celUIGridInventory* inv,
    csArray<GridEntry>& grid)
{
  iMouseDriver* mouse = inv->GetMouseDriver ();
  int x = mouse->GetLastX ();
  int y = mouse->GetLastY ();
  const InvStyle& style = inv->GetStyle ();

  for (size_t i = 0 ; i < grid.GetSize () ; i++)
  {
    GridEntry& g = grid[i];
    int hi = (x >= g.x && x < g.x + style.buttonw && y >= g.y && y < g.y + style.buttonh);
    if (hi) return &g;
  }
  return 0;
}

//--------------------------------------------------------------------------

celUIGridInventory::celUIGridInventory (iBase* parent)
  : scfImplementationType (this, parent)
{
  layouter = 0;
}

celUIGridInventory::~celUIGridInventory ()
{
  delete layouter;

  if (scfiRenderHandler)
  {
    csRef<iEventQueue> q (csQueryRegistry<iEventQueue> (object_reg));
    if (q)
      q->RemoveListener (scfiRenderHandler);
    scfiRenderHandler = 0;
  }
  if (scfiLogicHandler)
  {
    csRef<iEventQueue> q (csQueryRegistry<iEventQueue> (object_reg));
    if (q)
      q->RemoveListener (scfiLogicHandler);
    scfiLogicHandler = 0;
  }
  if (scfiInputHandler)
  {
    csRef<iEventQueue> q (csQueryRegistry<iEventQueue> (object_reg));
    if (q)
      q->RemoveListener (scfiInputHandler);
    scfiInputHandler = 0;
  }
}

bool celUIGridInventory::Initialize (iObjectRegistry* object_reg)
{
  celUIGridInventory::object_reg = object_reg;
  pl = csQueryRegistry<iCelPlLayer> (object_reg);
  engine = csQueryRegistry<iEngine> (object_reg);
  vc = csQueryRegistry<iVirtualClock> (object_reg);
  g3d = csQueryRegistry<iGraphics3D> (object_reg);
  info.AttachNew (new DefaultInfo (pl, engine));
  name_reg = csEventNameRegistry::GetRegistry (object_reg);
  mouse = csQueryRegistry<iMouseDriver> (object_reg);

  layouter = new GridLayouter (true);

  return true;
}

bool celUIGridInventory::HandleLogicEvent (iEvent& ev)
{
  if (ev.Name == csevFrame (object_reg))
  {
    if (style.rotateHiMesh)
    {
      GridEntry* g = layouter->GetSelected (this, grid);
      if (g  && g->handle[1])
      {
	g->UpdateEntry (this, 0);
	g->UpdateEntry (this, 1);
      }
    }
    return true;
  }
  return false;
}

static bool KeyEqual (utf32_char key1, utf32_char key2)
{
  return (key1 == key2)
  	|| (CSKEY_IS_MODIFIER (key1) && CSKEY_IS_MODIFIER (key2)
  	&& CSKEY_MODIFIER_COMPARE_CODE (key1, key2));
}

bool Binding::Match (iEventNameRegistry* name_reg, iObjectRegistry* object_reg, iEvent& ev) const
{
  if (CS_IS_KEYBOARD_EVENT (name_reg, ev))
  {
    if (type != csevKeyboardEvent (object_reg))
      return false;
    if (csKeyEventHelper::GetEventType (&ev) != csKeyEventTypeDown)
      return false;
    utf32_char k = csKeyEventHelper::GetRawCode (&ev);
    if (!KeyEqual (key, k))
      return false;
    csKeyModifiers key_modifiers;
    csKeyEventHelper::GetModifiers (&ev, key_modifiers);
    uint32 modifiers = csKeyEventHelper::GetModifiersBits (key_modifiers);
    return ((modifiers && mods) == mods);
  }
  else if (CS_IS_MOUSE_EVENT (name_reg, ev))
  {
    uint dev = csMouseEventHelper::GetNumber (&ev);
    if (dev != device)
      return false;
    int modifiers = csMouseEventHelper::GetModifiers (&ev);
    if ((modifiers && mods) != mods)
      return false;
    if (ev.Name == csevMouseMove (name_reg, dev))
    {
      csEventID mouse_id = csevMouseMove (object_reg, dev);
      return csEventNameRegistry::IsKindOf (name_reg, type, mouse_id);
    }
    else if (ev.Name == csevMouseDown (object_reg, dev))
    {
      int button = csMouseEventHelper::GetButton (&ev);
      if (numeric != button)
	return false;
      csEventID mouse_id = csevMouseButton (object_reg, dev);
      return csEventNameRegistry::IsKindOf (name_reg, type, mouse_id);
    }
  }
  else if (CS_IS_JOYSTICK_EVENT (name_reg, ev))
  {
    uint dev = csJoystickEventHelper::GetNumber (&ev);
    if (dev != device)
      return false;
    int modifiers = csJoystickEventHelper::GetModifiers (&ev);
    if ((modifiers && mods) != mods)
      return false;
    csJoystickEventData data;
    csJoystickEventHelper::GetEventData (&ev, data);
    if ((data.axesChanged & (1 << numeric)) == 0)
      return false;
    if (ev.Name == csevJoystickMove (object_reg, dev))
    {
      csEventID joy_id = csevJoystickMove (object_reg, dev);
      return csEventNameRegistry::IsKindOf (name_reg, type, joy_id);
    }
    else
    {
      int button = csJoystickEventHelper::GetButton (&ev);
      if (numeric != button)
	return false;
      csEventID joy_id = csevJoystickButton (object_reg, dev);
      return csEventNameRegistry::IsKindOf (name_reg, type, joy_id);
    }
  }
  return false;
}

void celUIGridInventory::DoSelect (const char* args, bool close)
{
  GridEntry* g = layouter->GetSelected (this, grid);
  if (g)
  {
    if (g->entity) FireSelectionListeners (g->entity, args);
    else if (g->tpl) FireSelectionListeners (g->tpl, args);
    if (close)
      Close ();
    else
    {
      // Possibly we need to refresh our inventory.
      SetupItems ();
      SetupLayout ();
    }
  }
}

void celUIGridInventory::DoScroll (int dx, int dy)
{
  layouter->Scroll (dx, dy);
  SetupLayout ();
}

bool celUIGridInventory::HandleInputEvent (iEvent& ev)
{
  for (size_t i = 0 ; i < bindings.GetSize () ; i++)
  {
    const Binding& b = bindings[i];
    if (b.Match (name_reg, object_reg, ev))
    {
      switch (b.command)
      {
	case COMMAND_CANCEL: Close (); return true;
	case COMMAND_SELECT: DoSelect (b.args, true); return true;
	case COMMAND_SELECT_KEEPOPEN: DoSelect (b.args, false); return true;
	case COMMAND_SCROLL_LEFT: DoScroll (-1, 0); return true;
	case COMMAND_SCROLL_RIGHT: DoScroll (1, 0); return true;
	case COMMAND_SCROLL_UP: DoScroll (0, -1); return true;
	case COMMAND_SCROLL_DOWN: DoScroll (0, 1); return true;
      }
      return true;
    }
  }
  return true;
}

bool celUIGridInventory::HandleRenderEvent (iEvent& ev)
{
  if (ev.Name == csevFrame (object_reg))
  {
    GridEntry* sel = layouter->GetSelected (this, grid);

    g3d->BeginDraw (CSDRAW_2DGRAPHICS);
    for (size_t i = 0 ; i < grid.GetSize () ; i++)
    {
      GridEntry& g = grid[i];
      if (g.x != -1 && g.y != -1)
      {
        int hi = &g == sel;
        if (g.handle[hi])
        {
          g3d->DrawPixmap (g.handle[hi], g.x, g.y, style.buttonw, style.buttonh,
	      0, 0, style.buttonw, style.buttonh);
        }
      }
    }
  }
  return true;
}

void celUIGridInventory::Activate ()
{
  if (scfiRenderHandler)
    return;

  csRef<iEventQueue> q (csQueryRegistry<iEventQueue> (object_reg));
  CS_ASSERT (q);

  {
    scfiRenderHandler.AttachNew (new RenderEventHandler (this));
    csEventID esub[] = {
	csevFrame (object_reg),
    	CS_EVENTLIST_END
    	};
    q->RegisterListener (scfiRenderHandler, esub);
  }

  {
    scfiLogicHandler.AttachNew (new LogicEventHandler (this));
    csEventID esub[] = {
	csevFrame (object_reg),
    	CS_EVENTLIST_END
    	};
    q->RegisterListener (scfiLogicHandler, esub);
  }

  {
    scfiInputHandler.AttachNew (new InputEventHandler (this));
    csEventID esub[] = {
    	csevKeyboardEvent (object_reg),
    	csevMouseEvent (object_reg),
    	CS_EVENTLIST_END
    	};
    q->RegisterListener (scfiInputHandler, esub);
  }
}

void celUIGridInventory::Deactivate ()
{
  if (!scfiRenderHandler)
    return;

  csRef<iEventQueue> q (csQueryRegistry<iEventQueue> (object_reg));
  CS_ASSERT (q);
  q->RemoveListener (scfiRenderHandler);
  scfiRenderHandler = 0;
  q->RemoveListener (scfiLogicHandler);
  scfiLogicHandler = 0;
  q->RemoveListener (scfiInputHandler);
  scfiInputHandler = 0;
}

// ------------------------------------------------------------------

InvStyle::InvStyle ()
{
  buttonw = 128;
  buttonh = 128;
  marginhor = 16;
  marginver = 16;
  bgred[0] = 50;
  bggreen[0] = 50;
  bgblue[0] = 50;
  bgalpha[0] = 255;
  bgred[1] = 80;
  bggreen[1] = 80;
  bgblue[1] = 80;
  bgalpha[1] = 255;
  fontSize = 10;
  rotateHiMesh = true;
}

bool InvStyle::SetStyleOption (celUIGridInventory* inv,
    const char* name, const char* value)
{
  csString styleName = name;
  if (styleName == "buttonWidth")
  {
    csScanStr (value, "%d", &buttonw);
    return true;
  }
  if (styleName == "buttonHeight")
  {
    csScanStr (value, "%d", &buttonh);
    return true;
  }
  if (styleName == "marginHorizontal")
  {
    csScanStr (value, "%d", &marginhor);
    return true;
  }
  if (styleName == "marginVertical")
  {
    csScanStr (value, "%d", &marginver);
    return true;
  }
  if (styleName == "backgroundImage")
  {
    backgroundImage[0] = value;
    return true;
  }
  if (styleName == "backgroundHilightImage")
  {
    backgroundImage[1] = value;
    return true;
  }
  if (styleName == "backgroundColor")
  {
    int num = csScanStr (value, "%d,%d,%d,%d", &bgred[0], &bggreen[0], &bgblue[0],
	&bgalpha[0]);
    if (num < 4) bgalpha[0] = 255;
    return true;
  }
  if (styleName == "backgroundHilightColor")
  {
    int num = csScanStr (value, "%d,%d,%d,%d", &bgred[1], &bggreen[1], &bgblue[1],
	&bgalpha[1]);
    if (num < 4) bgalpha[1] = 255;
    return true;
  }
  if (styleName == "font")
  {
    fontName = value;
    return true;
  }
  if (styleName == "fontSize")
  {
    csScanStr (value, "%d", &fontSize);
    return true;
  }
  if (styleName == "rotateHilightMesh")
  {
    csScanStr (value, "%b", &rotateHiMesh);
    return true;
  }

  return false;
}

void GridEntry::SetupEntry (celUIGridInventory* inv,
    const char* txt, int amount,
    iMeshFactoryWrapper* factory, int hi)
{
  GridEntry::factory = factory;

  iGraphics3D* g3d = inv->GetG3D ();
  iGraphics2D* g2d = g3d->GetDriver2D ();
  iEngine* engine = inv->GetEngine ();
  const InvStyle& style = inv->GetStyle ();

  iTextureManager* txtmgr = g3d->GetTextureManager ();

  if (!handle[hi])
    handle[hi] = txtmgr->CreateTexture (style.buttonw, style.buttonh, csimg2D, "rgba8",
      CS_TEXTURE_3D);
  g3d->SetRenderTarget (handle[hi]);
  g3d->BeginDraw (CSDRAW_2DGRAPHICS);

  if (style.backgroundTexture[hi])
  {
    int bw, bh;
    style.backgroundTexture[hi]->GetOriginalDimensions (bw, bh);
    // Due to a bug? we have to use the real (scaled) dimensions of the
    // handle texture because otherwise the renderer messes up with the scale.
    int handlew, handleh;
    handle[hi]->GetRendererDimensions (handlew, handleh);
    g3d->DrawPixmap (style.backgroundTexture[hi], 0, 0, handlew, handleh, 0, 0, bw, bh);
  }
  else
  {
    int color = g2d->FindRGB (style.bgred[hi], style.bggreen[hi], style.bgblue[hi], style.bgalpha[hi]);
    g2d->DrawBox (0, 0, style.buttonw, style.buttonh, color);
  }

  if (factory)
  {
    csMeshOnTexture* mt = new csMeshOnTexture (inv->GetObjectRegistry ());
    iSector* sector = engine->FindSector ("___gridinv__");
    if (!sector)
    { 
      sector = engine->CreateSector ("___gridinv__");
      csRef<iLight> light;
      iLightList* ll = sector->GetLights ();
      light = engine->CreateLight (0, csVector3 (-300, 300, -300), 1000, csColor (1, 1, 1));
      ll->Add (light);
      light = engine->CreateLight (0, csVector3 (300, 300, 300), 1000, csColor (1, 1, 1));
      ll->Add (light);
    }
    csString meshName;
    meshName.Format ("__gridmesh__%s__", factory->QueryObject ()->GetName ());
    mesh = engine->FindMeshObject (meshName);
    if (mesh)
      mesh->GetMovable ()->SetSector (sector);
    else
    {
      mesh = engine->CreateMeshWrapper (factory, meshName, sector);
      //mesh->SetZBufMode (CS_ZBUF_NONE);
    }
    mesh->GetMovable ()->SetTransform (trans);
    mesh->GetMovable ()->UpdateMove ();
    iCamera* cam = mt->GetView ()->GetCamera ();
    cam->SetSector (sector);
    mt->ScaleCamera (mesh, style.buttonw, style.buttonh);
    cam->Move (csVector3 (.7, .7, -.5));
    mt->ScaleCamera (mesh, style.buttonw, style.buttonh);
    cam->GetTransform ().LookAt (-cam->GetTransform ().GetOrigin (), csVector3 (0, 1, 0));
    float dist = sqrt (csSquaredDist::PointPoint (cam->GetTransform ().GetOrigin (),
	  csVector3 (0, 0, 0)));
    cam->Move (csVector3 (0, 0, - dist / 10.0f));
    camtrans = cam->GetTransform ();
    mt->Render (mesh, handle[hi], true);
    iRenderManager* rm = engine->GetRenderManager ();
    rm->RenderView (mt->GetView ());
    mesh->GetMovable ()->ClearSectors ();
    delete mt;
  }

  int fg = g2d->FindRGB (0, 0, 255);
  printf ("text=%s\n", txt); fflush (stdout);
  if (amount)
    text.Format ("%s (%d)", txt, amount);
  else
    text = txt;
  g2d->Write (style.font, 10, 10, fg, -1, text);

  g3d->FinishDraw ();
  g3d->SetRenderTarget (0);
}

void GridEntry::UpdateEntry (celUIGridInventory* inv, int hi)
{
  iGraphics3D* g3d = inv->GetG3D ();
  iGraphics2D* g2d = g3d->GetDriver2D ();
  iEngine* engine = inv->GetEngine ();
  const InvStyle& style = inv->GetStyle ();

  g3d->SetRenderTarget (handle[hi]);
  g3d->BeginDraw (CSDRAW_2DGRAPHICS | CSDRAW_CLEARZBUFFER);
  g2d->Clear (g2d->FindRGB (0, 0, 0, 0));

  if (style.backgroundTexture[hi])
  {
    int bw, bh;
    style.backgroundTexture[hi]->GetRendererDimensions (bw, bh);
    // Due to a bug? we have to use the real (scaled) dimensions of the
    // handle texture because otherwise the renderer messes up with the scale.
    int handlew, handleh;
    handle[hi]->GetRendererDimensions (handlew, handleh);
    g3d->DrawPixmap (style.backgroundTexture[hi], 0, 0, handlew, handleh, 0, 0, bw, bh);
  }
  else
  {
    int color = g2d->FindRGB (style.bgred[hi], style.bggreen[hi], style.bgblue[hi], style.bgalpha[hi]);
    g2d->DrawBox (0, 0, style.buttonw, style.buttonh, color);
  }

  if (mesh)
  {
    csMeshOnTexture* mt = new csMeshOnTexture (inv->GetObjectRegistry ());
    iSector* sector = engine->FindSector ("___gridinv__");
    mesh->GetMovable ()->SetSector (sector);
    if (hi && style.rotateHiMesh)
    {
      trans.RotateThis (csVector3 (0, 1, 0), inv->GetVC ()->GetElapsedSeconds ());
    }
    mesh->GetMovable ()->SetTransform (trans);
    mesh->GetMovable ()->UpdateMove ();
    iCamera* cam = mt->GetView ()->GetCamera ();
    cam->SetSector (sector);
    mt->ScaleCamera (mesh, style.buttonw, style.buttonh);
    cam->SetTransform (camtrans);
    mt->Render (mesh, handle[hi], true);
    iRenderManager* rm = engine->GetRenderManager ();
    rm->RenderView (mt->GetView ());
    mesh->GetMovable ()->ClearSectors ();
    delete mt;
  }

  int fg = g2d->FindRGB (255, 255, 255);
  g2d->Write (style.font, 10, 10, fg, -1, text);

  g3d->FinishDraw ();
  g3d->SetRenderTarget (0);
}

// ------------------------------------------------------------------

void celUIGridInventory::SetupItems ()
{
  grid.DeleteAll ();

  if (style.fontName.IsEmpty ())
    style.font = g3d->GetDriver2D ()->GetFontServer ()->LoadFont (CSFONT_COURIER);
  else
    style.font = g3d->GetDriver2D ()->GetFontServer ()->LoadFont (style.fontName, style.fontSize);

  for (int hi = 0 ; hi < 2 ; hi++)
  {
    if (!style.backgroundImage[hi].IsEmpty ())
    {
      iTextureWrapper* txt = engine->CreateTexture (style.backgroundImage[hi],
	  style.backgroundImage[hi], 0, CS_TEXTURE_3D);
      if (!txt)
      {
	printf ("Error loading image '%s'!\n", style.backgroundImage[hi].GetData ());
	return;
      }
      iTextureManager* txtmgr = g3d->GetTextureManager ();
      txt->Register (txtmgr);
      style.backgroundTexture[hi] = txt->GetTextureHandle ();
    }
  }

  for (size_t i = 0 ; i < inventory->GetEntityCount () ; i++)
  {
    iCelEntity* ent = inventory->GetEntity (i);
    csRef<iString> n = info->GetName (ent);
    size_t idx = grid.Push (GridEntry ());
    GridEntry& g = grid[idx];

    g.entity = ent;
    g.SetupEntry (this, n->GetData (), 0, info->GetMeshFactory (ent), 0);
    g.SetupEntry (this, n->GetData (), 0, info->GetMeshFactory (ent), 1);
  }

  for (size_t i = 0 ; i < inventory->GetEntityTemplateCount () ; i++)
  {
    iCelEntityTemplate* ent = inventory->GetEntityTemplate (i);
    int amount = inventory->GetEntityTemplateAmount (i);
    csRef<iString> n = info->GetName (ent, amount);
    size_t idx = grid.Push (GridEntry ());
    GridEntry& g = grid[idx];

    g.tpl = ent;
    g.SetupEntry (this, n->GetData (), amount, info->GetMeshFactory (ent, amount), 0);
    g.SetupEntry (this, n->GetData (), amount, info->GetMeshFactory (ent, amount), 1);
  }
}

void celUIGridInventory::SetupLayout ()
{
  layouter->Layout (this, grid);
}

void celUIGridInventory::Refresh ()
{
  UpdateLists (inventory);
}

void celUIGridInventory::UpdateLists (iPcInventory* inventory)
{
}

void celUIGridInventory::AddSelectionListener (iUIInventorySelectionCallback* cb)
{
  callbacks.Push (cb);
}

void celUIGridInventory::RemoveSelectionListener (iUIInventorySelectionCallback* cb)
{
  callbacks.Delete (cb);
}

void celUIGridInventory::FireSelectionListeners (iCelEntity* entity, const char* command)
{
  for (size_t i = 0 ; i < callbacks.GetSize () ; i++)
    callbacks[i]->SelectEntity (entity, command);
}

void celUIGridInventory::FireSelectionListeners (iCelEntityTemplate* tpl, const char* command)
{
  for (size_t i = 0 ; i < callbacks.GetSize () ; i++)
    callbacks[i]->SelectTemplate (tpl, command);
}

void celUIGridInventory::Open (const char* title, iPcInventory* inventory)
{
  Activate ();

  celUIGridInventory::inventory = inventory;

  SetupItems ();
  SetupLayout ();

  if (!listener)
    listener.AttachNew (new InvListener (this));
  inventory->RemoveInventoryListener (listener);
  inventory->AddInventoryListener (listener);

  UpdateLists (inventory);
}

void celUIGridInventory::Close ()
{
  Deactivate ();

  if (inventory && listener)
    inventory->RemoveInventoryListener (listener);
  inventory = 0;
}

bool celUIGridInventory::Bind (const char* eventname, const char* command, const char* args)
{
  Binding binding;
  csKeyModifiers modifiers;
  csString cmd = command;
  if (cmd == "cancel") binding.command = COMMAND_CANCEL;
  else if (cmd == "select") binding.command = COMMAND_SELECT;
  else if (cmd == "select_keepopen") binding.command = COMMAND_SELECT_KEEPOPEN;
  else if (cmd == "scroll_left") binding.command = COMMAND_SCROLL_LEFT;
  else if (cmd == "scroll_right") binding.command = COMMAND_SCROLL_RIGHT;
  else if (cmd == "scroll_up") binding.command = COMMAND_SCROLL_UP;
  else if (cmd == "scroll_down") binding.command = COMMAND_SCROLL_DOWN;
  else
  {
    printf ("Warning! Grid inventory doesn't understand command '%s'!\n", command);
    return false;
  }
  binding.args = args;

  if (!csInputDefinition::ParseOther (name_reg, eventname, &binding.type, &binding.device,
  	&binding.numeric, &modifiers))
  {
    printf ("Bad input specification '%s'!", eventname);
    return false;
  }
  if (binding.type == csevKeyboardEvent (object_reg))
  {
    csInputDefinition::ParseKey (name_reg, eventname, &binding.key, &binding.cooked, &modifiers);
    binding.mods = csKeyEventHelper::GetModifiersBits (modifiers);
    bindings.Push (binding);
  }
  else
  {
    binding.mods = csKeyEventHelper::GetModifiersBits (modifiers);
    bindings.Push (binding);
  }

  return true;
}


//---------------------------------------------------------------------------

