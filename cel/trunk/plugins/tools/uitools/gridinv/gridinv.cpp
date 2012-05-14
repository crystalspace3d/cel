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
#include "csgfx/imagememory.h"
#include "csgfx/imagemanipulate.h"
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
  str->Append (ent->GetName ());
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

GridLayouter::GridLayouter (celUIGridInventory* inv, bool verticalscroll) :
    inv (inv),
    verticalscroll (verticalscroll),
    horcountSet (false), vercountSet (false),
    firstx (0), firsty (0),
    scrollTime (0.0f), maxScrollTime (1.0f),
    scrollDirection (0)
{
}

void GridLayouter::NextSlot ()
{
  if (verticalscroll)
  {
    ix++;
    if (ix >= horcount)
    {
      ix = 0;
      iy++;
    }
  }
  else
  {
    iy++;
    if (iy >= vercount)
    {
      iy = 0;
      ix++;
    }
  }
}

void GridLayouter::FirstSlot ()
{
  ix = 0;
  iy = 0;
}

void GridLayouter::Layout ()
{
  const InvStyle& style = inv->GetStyle ();
  iGraphics3D* g3d = inv->GetG3D ();
  int w = g3d->GetWidth ();
  int h = g3d->GetHeight ();
  if (!horcountSet)
    horcount = w / (style.buttonw + style.marginhor);
  if (!vercountSet)
    vercount = h / (style.buttonh + style.marginver);
  totalwidth = horcount * (style.buttonw + style.marginhor) - style.marginhor;
  totalheight = vercount * (style.buttonh + style.marginver) - style.marginver;
  leftmargin = (w-totalwidth)/2;
  topmargin = (h-totalheight)/2;

  csArray<GridEntry>& grid = inv->GetGrid ();
  FirstSlot ();
  for (size_t i = 0 ; i < grid.GetSize () ; i++)
  {
    GridEntry& g = grid[i];
    g.x = ix * (style.buttonw + style.marginhor);
    g.y = iy * (style.buttonh + style.marginver);
    NextSlot ();
  }
}

void GridLayouter::Scroll (int d, float time)
{
  if (scrollTime > 0.0f) return;	// Ignore, already scrolling.
  const InvStyle& style = inv->GetStyle ();
  if (verticalscroll)
  {
    firsty += d;
    if (firsty < 0) { firsty = 0; return; }
    scrollDirection = (style.buttonh + style.marginver);
  }
  else
  {
    firstx += d;
    if (firstx < 0) { firstx = 0; return; }
    scrollDirection = (style.buttonw + style.marginhor);
  }
  if (d > 0) scrollDirection = -scrollDirection;
  scrollTime = time;
  maxScrollTime = time;
}

void GridLayouter::UpdateScroll (float elapsed)
{
  if (scrollTime <= 0.0f) return;
  scrollTime -= elapsed;
  if (scrollTime <= 0.0f)
  {
    scrollDirection = 0;
    scrollTime = 0.0f;
  }
}

bool GridLayouter::GetRealPosition (size_t i, int& x, int& y)
{
  const InvStyle& style = inv->GetStyle ();

  GridEntry& g = inv->GetGrid ()[i];
  x = leftmargin + g.x;
  y = topmargin + g.y;
  bool toprow = false;
  bool botrow = false;
  float st = scrollTime / maxScrollTime;
  if (verticalscroll)
  {
    y -= firsty * (style.marginver + style.buttonh) + st * scrollDirection;
    if (y + style.buttonh <= 0) return false;
    if (y >= topmargin + totalheight) return false;
    if (y < topmargin) toprow = true;
    if (y+style.buttonh > topmargin + totalheight) botrow = true;
  }
  else
  {
    x -= firstx * (style.marginhor + style.buttonw) + st * scrollDirection;
    if (x + style.buttonw <= 0) return false;
    if (x >= leftmargin + totalwidth) return false;
    if (x < leftmargin) toprow = true;
    if (x+style.buttonw > leftmargin + totalwidth) botrow = true;
  }
  g.alpha = 0;
  if (toprow)
  {
    if (scrollDirection < 0)
      g.alpha = int (255.0 * (1.0 - st));
    else
      g.alpha = int (255.0 * st);
  }
  else if (botrow)
  {
    if (scrollDirection > 0)
      g.alpha = int (255.0 * (1.0 - st));
    else
      g.alpha = int (255.0 * st);
  }
  return true;
}


GridEntry* GridLayouter::GetSelected ()
{
  iMouseDriver* mouse = inv->GetMouseDriver ();
  int x = mouse->GetLastX ();
  int y = mouse->GetLastY ();
  const InvStyle& style = inv->GetStyle ();

  csArray<GridEntry>& grid = inv->GetGrid ();
  for (size_t i = 0 ; i < grid.GetSize () ; i++)
  {
    int rx, ry;
    if (GetRealPosition (i, rx, ry))
    {
      int hi = (x >= rx && x < rx + style.buttonw && y >= ry && y < ry + style.buttonh);
      if (hi) return &grid[i];
    }
  }
  return 0;
}

bool GridLayouter::SetStyleOption (const csString& name, const char* value)
{
  if (name == "grid.horizontal.count")
  {
    csScanStr (value, "%d", &horcount);
    horcountSet = true;
    return true;
  }
  if (name == "grid.vertical.count")
  {
    csScanStr (value, "%d", &vercount);
    vercountSet = true;
    return true;
  }
  if (name == "grid.orientation")
  {
    csString v = value;
    v.Downcase ();
    if (v == "vertical") verticalscroll = true;
    else verticalscroll = false;
    return true;
  }
  return false;
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
  imageio = csQueryRegistry<iImageIO> (object_reg);
  vfs = csQueryRegistry<iVFS> (object_reg);

  layouter = new GridLayouter (this, true);
  style.Setup (g3d->GetDriver2D ());

  return true;
}

bool celUIGridInventory::HandleLogicEvent (iEvent& ev)
{
  if (ev.Name == csevFrame (object_reg))
  {
    layouter->UpdateScroll (vc->GetElapsedSeconds ());

    if (style.rotateHiMesh)
    {
      GridEntry* g = layouter->GetSelected ();
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
  GridEntry* g = layouter->GetSelected ();
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
      layouter->Layout ();
    }
  }
}

void celUIGridInventory::DoScroll (int d)
{
  // @@@ Scroll time config option
  layouter->Scroll (d, 0.5f);
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
	case COMMAND_SCROLL_LEFT: DoScroll (-1); return true;
	case COMMAND_SCROLL_RIGHT: DoScroll (1); return true;
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
    GridEntry* sel = layouter->GetSelected ();

    g3d->BeginDraw (CSDRAW_2DGRAPHICS);
    for (size_t i = 0 ; i < grid.GetSize () ; i++)
    {
      int x, y;
      if (layouter->GetRealPosition (i, x, y))
      {
        GridEntry& g = grid[i];
        int hi = &g == sel;
        if (g.handle[hi])
        {
          g3d->DrawPixmap (g.handle[hi], x, y, style.buttonw, style.buttonh,
	      0, 0, style.buttonw, style.buttonh, g.alpha);
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
}

void InvStyle::Setup (iGraphics2D* g2d)
{
  buttonw = 128;
  buttonh = 128;
  marginhor = 16;
  marginver = 16;
  bg[0] = g2d->FindRGB (50, 50, 50, 255);
  bg[1] = g2d->FindRGB (80, 80, 80, 255);
  rotateHiMesh = true;

  nameStyle.fontSize = 10;
  nameStyle.horPos = -5;
  nameStyle.verPos = 5;
  nameStyle.fg[0] = g2d->FindRGB (255, 255, 255, 255);
  nameStyle.fg[1] = g2d->FindRGB (255, 255, 0, 255);
  nameStyle.bg[0] = -1;
  nameStyle.bg[1] = -1;

  amountStyle.fontSize = 10;
  amountStyle.horPos = 5;
  amountStyle.verPos = -5;
  amountStyle.fg[0] = g2d->FindRGB (255, 255, 255, 255);
  amountStyle.fg[1] = g2d->FindRGB (255, 255, 0, 255);
  amountStyle.bg[0] = -1;
  amountStyle.bg[1] = -1;
}

static int FindRGB (const char* value, iGraphics2D* g2d)
{
  csString v = value;
  v.Downcase ();
  if (v == "transparent") return -1;
  int r, g, b, a;
  int num = csScanStr (value, "%d,%d,%d,%d", &r, &g, &b, &a);
  if (num < 4) a = 255;
  return g2d->FindRGB (r, g, b, a);
}

bool TextStyle::SetStyleOption (const char* prefix, celUIGridInventory* inv,
    const csString& name, const char* value)
{
  if (!name.StartsWith (prefix)) return false;
  csString styleName = name.Slice (strlen (prefix));

  if (styleName == "color.foreground")
  {
    fg[0] = FindRGB (value, inv->GetG2D ());
    return true;
  }
  if (styleName == "color.foreground.hi")
  {
    fg[1] = FindRGB (value, inv->GetG2D ());
    return true;
  }
  if (styleName == "color.background")
  {
    bg[0] = FindRGB (value, inv->GetG2D ());
    return true;
  }
  if (styleName == "color.background.hi")
  {
    bg[1] = FindRGB (value, inv->GetG2D ());
    return true;
  }
  if (styleName == "font")
  {
    fontName = value;
    return true;
  }
  if (styleName == "horizontal")
  {
    csScanStr (value, "%d", horPos);
    return true;
  }
  if (styleName == "vertical")
  {
    csScanStr (value, "%d", verPos);
    return true;
  }
  if (styleName == "font.size")
  {
    csScanStr (value, "%d", &fontSize);
    return true;
  }
  return false;
}

bool TextStyle::GetTextPos (const char* txt, int& x, int& y, int w, int h) const
{
  if (fontSize <= 0) return false;
  
  int tw, th;
  font->GetDimensions (txt, tw, th);
  if (horPos < 0) x = -horPos;
  else if (horPos > 0) x = w - tw - horPos;
  else x = (w - tw) / 2;
  if (verPos < 0) y = -verPos;
  else if (verPos > 0) y = h - th - verPos;
  else y = (h - th) / 2;
  return true;
}

bool InvStyle::SetStyleOption (celUIGridInventory* inv,
    const csString& name, const char* value)
{
  if (nameStyle.SetStyleOption ("name.", inv, name, value)) return true;
  if (amountStyle.SetStyleOption ("amount.", inv, name, value)) return true;

  if (name == "button.width")
  {
    csScanStr (value, "%d", &buttonw);
    return true;
  }
  if (name == "button.height")
  {
    csScanStr (value, "%d", &buttonh);
    return true;
  }
  if (name == "button.hormargin")
  {
    csScanStr (value, "%d", &marginhor);
    return true;
  }
  if (name == "button.vermargin")
  {
    csScanStr (value, "%d", &marginver);
    return true;
  }
  if (name == "background.image")
  {
    backgroundImage[0] = value;
    return true;
  }
  if (name == "background.image.hi")
  {
    backgroundImage[1] = value;
    return true;
  }
  if (name == "background.color")
  {
    bg[0] = FindRGB (value, inv->GetG2D ());
    return true;
  }
  if (name == "background.color.hi")
  {
    bg[1] = FindRGB (value, inv->GetG2D ());
    return true;
  }
  if (name == "hilight.rotate")
  {
    csScanStr (value, "%b", &rotateHiMesh);
    return true;
  }

  return false;
}

void GridEntry::WriteText (celUIGridInventory* inv, int hi)
{
  const InvStyle& style = inv->GetStyle ();
  if (style.nameStyle.fontSize >= 0 || style.amountStyle.fontSize >= 0)
  {
    iGraphics3D* g3d = inv->GetG3D ();
    iGraphics2D* g2d = inv->GetG2D ();

    int handlew, handleh;
    handle[hi]->GetRendererDimensions (handlew, handleh);

    g3d->SetRenderTarget (handle[hi], true);
    g3d->BeginDraw (CSDRAW_2DGRAPHICS);

    int x, y;

    if (amount && style.amountStyle.fontSize >= 0)
    {
      csString t;
      t.Format ("%d", amount);
      if (style.amountStyle.GetTextPos (t, x, y, handlew, handleh))
        g2d->Write (style.amountStyle.font, x, y,
	    style.amountStyle.fg[hi], style.amountStyle.bg[hi], t);
    }

    if (style.nameStyle.GetTextPos (text, x, y, handlew, handleh))
    {
      g2d->Write (style.nameStyle.font, x, y,
	  style.nameStyle.fg[hi], style.nameStyle.bg[hi], text);
    }

    g3d->FinishDraw ();
    g3d->SetRenderTarget (0);
  }
}

iSector* GridEntry::SetupSector (celUIGridInventory* inv)
{
  iEngine* engine = inv->GetEngine ();
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
  return sector;
}

// @@@@@@@@@@@@@@ CLEANUP MESH ON EXIT!

void GridEntry::SetupMesh (celUIGridInventory* inv,
    iSector* sector, iMeshFactoryWrapper* factory)
{
  iEngine* engine = inv->GetEngine ();
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
  trans.Identity ();
  mesh->GetMovable ()->SetTransform (trans);
  mesh->GetMovable ()->UpdateMove ();
}

void GridEntry::SetupCamera (celUIGridInventory* inv, iSector* sector,
    csMeshOnTexture* mt)
{
  const InvStyle& style = inv->GetStyle ();
  iCamera* cam = mt->GetView ()->GetCamera ();
  cam->SetSector (sector);
  mt->ScaleCamera (mesh, style.buttonw, style.buttonh);
  const csBox3 mesh_box = mesh->GetWorldBoundingBox ();
  const csVector3 mesh_center = mesh_box.GetCenter ();
  float dist = sqrt (csSquaredDist::PointPoint (cam->GetTransform ().GetOrigin (),
	mesh_center));
  cam->Move (csVector3 (dist/1.6, dist/1.6, 0));
  cam->GetTransform ().LookAt (mesh_center-cam->GetTransform ().GetOrigin (), csVector3 (0, 1, 0));
  cam->Move (csVector3 (0, 0, dist/10.0));
  camtrans = cam->GetTransform ();
}

void GridEntry::SetupBackgroundTexture (celUIGridInventory* inv, int hi)
{
  const InvStyle& style = inv->GetStyle ();
  iGraphics3D* g3d = inv->GetG3D ();
  iGraphics2D* g2d = g3d->GetDriver2D ();
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
    int color = style.bg[hi];
    g2d->DrawBox (0, 0, style.buttonw, style.buttonh, color);
  }
}

void GridEntry::CacheButton (celUIGridInventory* inv, int hi)
{
  csString cacheFilename;
  cacheFilename.Format ("%s/%s_%d.png",
      inv->GetCacheDir (),
      factory->QueryObject ()->GetName (), hi);
  printf ("Caching '%s'!\n", cacheFilename.GetData ());
  CS::StructuredTextureFormat format = CS::TextureFormatStrings::ConvertStructured (
      "abgr8");
  csRef<iDataBuffer> buf = handle[hi]->Readback (format);
  int handlew, handleh;
  handle[hi]->GetRendererDimensions (handlew, handleh);
  csRef<csImageMemory> image;
  image.AttachNew (new csImageMemory (handlew, handleh,
	(const void*)(buf->GetData ()), CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA));
  csRef<iDataBuffer> filebuf = inv->GetImageIO ()->Save (image, "image/png", "progressive");
  if (!inv->GetVFS ()->WriteFile (cacheFilename, (const char*)filebuf->GetData (),
	filebuf->GetSize ()))
  {
    // @@@ Proper error reporting.
    printf ("Error writing to file '%s'!\n", cacheFilename.GetData ());
  }
}

csRef<iImage> GridEntry::ReadCachedButton (celUIGridInventory* inv, int hi)
{
  csRef<iImage> cachedImage;
  if (factory && inv->GetCacheDir ())
  {
    csString cacheFilename;
    cacheFilename.Format ("%s/%s_%d.png",
	inv->GetCacheDir (),
	factory->QueryObject ()->GetName (), hi);

    csRef<iDataBuffer> filebuf = inv->GetVFS ()->ReadFile (cacheFilename);
    if (filebuf)
    {
      cachedImage = inv->GetImageIO ()->Load (filebuf, CS_IMGFMT_TRUECOLOR | CS_IMGFMT_ALPHA);
    }
  }
  return cachedImage;
}

bool GridEntry::SetupTexture (celUIGridInventory* inv, int hi)
{
  iGraphics3D* g3d = inv->GetG3D ();
  iTextureManager* txtmgr = g3d->GetTextureManager ();
  const InvStyle& style = inv->GetStyle ();
  csRef<iImage> cachedImage = ReadCachedButton (inv, hi);
  if (cachedImage)
  {
    if (style.buttonw != cachedImage->GetWidth () ||
	style.buttonh != cachedImage->GetHeight ())
      cachedImage = csImageManipulate::Rescale (cachedImage, style.buttonw,
	  style.buttonh);
    handle[hi] = txtmgr->RegisterTexture (cachedImage, CS_TEXTURE_3D | CS_TEXTURE_NOMIPMAPS);
  }
  else
  {
    handle[hi] = txtmgr->CreateTexture (style.buttonw, style.buttonh, csimg2D, "rgba8",
      CS_TEXTURE_3D | CS_TEXTURE_NOMIPMAPS);
  }
  handle[hi]->SetTextureClass ("nocompress");

  return cachedImage != 0;
}

void GridEntry::SetupEntry (celUIGridInventory* inv,
    const char* txt, int amount,
    iMeshFactoryWrapper* factory, int hi)
{
  GridEntry::factory = factory;
  text = txt;
  GridEntry::amount = amount;

  iGraphics3D* g3d = inv->GetG3D ();
  iEngine* engine = inv->GetEngine ();

  bool fromcache = SetupTexture (inv, hi);

  csMeshOnTexture* mt = 0;
  if (factory)
  {
    mt = new csMeshOnTexture (inv->GetObjectRegistry ());
    iSector* sector = SetupSector (inv);
    SetupMesh (inv, sector, factory);
    SetupCamera (inv, sector, mt);
  }

  if (!fromcache)
  {
    g3d->SetRenderTarget (handle[hi]);
    g3d->BeginDraw (CSDRAW_2DGRAPHICS);

    SetupBackgroundTexture (inv, hi);

    if (factory)
    {
      mt->Render (mesh, handle[hi], true);
      iRenderManager* rm = engine->GetRenderManager ();
      rm->RenderView (mt->GetView ());

      mesh->GetMovable ()->ClearSectors ();
    }

    g3d->FinishDraw ();
    g3d->SetRenderTarget (0);

    if (factory && inv->GetCacheDir ())
      CacheButton (inv, hi);
  }

  delete mt;

  WriteText (inv, hi);
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

  SetupBackgroundTexture (inv, hi);

  if (mesh)
  {
    csMeshOnTexture* mt = new csMeshOnTexture (inv->GetObjectRegistry ());
    iSector* sector = SetupSector (inv);
    mesh->GetMovable ()->SetSector (sector);
    if (hi && style.rotateHiMesh)
    {
      trans.RotateThis (csVector3 (0, 1, 0), inv->GetVC ()->GetElapsedSeconds ());
    }
    mesh->GetMovable ()->SetTransform (trans);
    mesh->GetMovable ()->UpdateMove ();
    iCamera* cam = mt->GetView ()->GetCamera ();
    cam->SetSector (sector);
    cam->SetTransform (camtrans);
    mt->Render (mesh, handle[hi], true);
    iRenderManager* rm = engine->GetRenderManager ();
    rm->RenderView (mt->GetView ());
    mesh->GetMovable ()->ClearSectors ();
    delete mt;
  }

  g3d->FinishDraw ();
  g3d->SetRenderTarget (0);

  WriteText (inv, hi);
}

// ------------------------------------------------------------------

void celUIGridInventory::SetupItems ()
{
  grid.DeleteAll ();

  iGraphics2D* g2d = g3d->GetDriver2D ();
  if (style.nameStyle.fontSize >= 0)
  {
    if (style.nameStyle.fontName.IsEmpty ())
      style.nameStyle.font = g2d->GetFontServer ()->LoadFont (CSFONT_COURIER);
    else
      style.nameStyle.font = g2d->GetFontServer ()->LoadFont (style.nameStyle.fontName, style.nameStyle.fontSize);
  }
  if (style.amountStyle.fontSize >= 0)
  {
    if (style.amountStyle.fontName.IsEmpty ())
      style.amountStyle.font = g2d->GetFontServer ()->LoadFont (CSFONT_COURIER);
    else
      style.amountStyle.font = g2d->GetFontServer ()->LoadFont (style.amountStyle.fontName, style.amountStyle.fontSize);
  }

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
  layouter->Layout ();

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

bool celUIGridInventory::SetStyleOption (const char* name, const char* value)
{
  csString n = name;
  if (n == "grid.cachedir") { cacheDir = value; return true; }
  else if (style.SetStyleOption (this, n, value)) return true;
  return layouter->SetStyleOption (n, value);
}

//---------------------------------------------------------------------------

