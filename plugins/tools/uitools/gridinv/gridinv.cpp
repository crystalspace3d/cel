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


celUIGridInventory::celUIGridInventory (iBase* parent)
  : scfImplementationType (this, parent)
{
  scfiEventHandler = 0;
  scfiPreEventHandler = 0;
}

celUIGridInventory::~celUIGridInventory ()
{
  if (scfiEventHandler)
  {
    csRef<iEventQueue> q (csQueryRegistry<iEventQueue> (object_reg));
    if (q)
      q->RemoveListener (scfiEventHandler);
    scfiEventHandler->DecRef ();
  }
  if (scfiPreEventHandler)
  {
    csRef<iEventQueue> q (csQueryRegistry<iEventQueue> (object_reg));
    if (q)
      q->RemoveListener (scfiPreEventHandler);
    scfiPreEventHandler->DecRef ();
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

  return true;
}

bool celUIGridInventory::HandlePreEvent (iEvent& ev)
{
  if (ev.Name == csevFrame (object_reg))
  {
    if (style.rotateHiMesh)
    {
      int x, y;
      x = mouse->GetLastX ();
      y = mouse->GetLastY ();

      for (size_t i = 0 ; i < grid.GetSize () ; i++)
      {
        GridEntry& g = grid[i];
        int hi = (x >= g.x && x < g.x + style.buttonw && y >= g.y && y < g.y + style.buttonh);
        if (hi && g.handle[hi])
        {
	  g.UpdateEntry (this, 0);
	  g.UpdateEntry (this, 1);
        }
      }
    }
    return true;
  }
  return false;
}

bool celUIGridInventory::HandleEvent (iEvent& ev)
{
  if (ev.Name == csevFrame (object_reg))
  {
    int x, y;
    x = mouse->GetLastX ();
    y = mouse->GetLastY ();

    g3d->BeginDraw (CSDRAW_2DGRAPHICS);
    for (size_t i = 0 ; i < grid.GetSize () ; i++)
    {
      GridEntry& g = grid[i];
      int hi = (x >= g.x && x < g.x + style.buttonw && y >= g.y && y < g.y + style.buttonh);
      if (g.handle[hi])
      {
        g3d->DrawPixmap (g.handle[hi], g.x, g.y, style.buttonw, style.buttonh,
	    0, 0, style.buttonw, style.buttonh);
      }
    }
    return true;
  }
  else if (CS_IS_KEYBOARD_EVENT (name_reg, ev))
  {
    utf32_char key = csKeyEventHelper::GetRawCode (&ev);
    csKeyModifiers key_modifiers;
    csKeyEventHelper::GetModifiers (&ev, key_modifiers);
    //uint32 modifiers = csKeyEventHelper::GetModifiersBits (key_modifiers);
    uint32 type = csKeyEventHelper::GetEventType (&ev);
    if (type == csKeyEventTypeDown)
    {
      if (style.stopKey == key)
      {
	Close ();
      }
    }
    return true;
  }
  else if (ev.Name == csevMouseUp (name_reg, 0))
  {
    int x, y;
    x = mouse->GetLastX ();
    y = mouse->GetLastY ();

    for (size_t i = 0 ; i < grid.GetSize () ; i++)
    {
      GridEntry& g = grid[i];
      int hi = (x >= g.x && x < g.x + style.buttonw && y >= g.y && y < g.y + style.buttonh);
      if (hi)
      {
	if (g.entity) FireSelectionListeners (g.entity);
	else if (g.tpl) FireSelectionListeners (g.tpl);
	Close ();
      }
    }
    return true;
  }
  else if (ev.Name == csevMouseDown (name_reg, 0))
  {
    return true;
  }
  return false;
}

void celUIGridInventory::Activate ()
{
  if (scfiEventHandler)
    return;

  csRef<iEventQueue> q (csQueryRegistry<iEventQueue> (object_reg));
  CS_ASSERT (q);
  scfiEventHandler = new EventHandler (this);
  csEventID esub[] = {
    	csevKeyboardEvent (object_reg),
	csevFrame (object_reg),
    	csevMouseEvent (object_reg),
    	CS_EVENTLIST_END
    	};
  q->RegisterListener (scfiEventHandler, esub);
  scfiPreEventHandler = new PreEventHandler (this);
  csEventID esubPre[] = {
	csevFrame (object_reg),
    	CS_EVENTLIST_END
    	};
  q->RegisterListener (scfiPreEventHandler, esubPre);
}

void celUIGridInventory::Deactivate ()
{
  if (!scfiEventHandler)
    return;

  csRef<iEventQueue> q (csQueryRegistry<iEventQueue> (object_reg));
  CS_ASSERT (q);
  q->RemoveListener (scfiEventHandler);
  scfiEventHandler->DecRef ();
  scfiEventHandler = 0;
  q->RemoveListener (scfiPreEventHandler);
  scfiPreEventHandler->DecRef ();
  scfiPreEventHandler = 0;
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
  stopKey = CSKEY_ESC;
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
  if (styleName == "stopKey")
  {
    utf32_char cooked;
    csKeyModifiers modifiers;
    csInputDefinition::ParseKey (inv->GetNameReg (), value, &stopKey, &cooked, &modifiers);
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
    int x, int y,
    const char* txt, int amount,
    iMeshFactoryWrapper* factory, int hi)
{
  GridEntry::x = x;
  GridEntry::y = y;
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
    g3d->DrawPixmap (style.backgroundTexture[hi], 0, 0, style.buttonw, style.buttonh,
	    0, 0, style.buttonw, style.buttonh);
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
      mesh = engine->CreateMeshWrapper (factory, meshName, sector);
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

  int fg = g2d->FindRGB (255, 255, 255);
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
  g3d->BeginDraw (CSDRAW_2DGRAPHICS | CSDRAW_CLEARSCREEN | CSDRAW_CLEARZBUFFER);

  if (style.backgroundTexture[hi])
  {
    g3d->DrawPixmap (style.backgroundTexture[hi], 0, 0, style.buttonw, style.buttonh,
	    0, 0, style.buttonw, style.buttonh);
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

void celUIGridInventory::Setup ()
{
  int w = g3d->GetWidth ();
  int h = g3d->GetHeight ();
  int horcount = w / (style.buttonw + style.marginhor);
  int vercount = h / (style.buttonh + style.marginver);
  grid.DeleteAll ();
  grid.SetSize (horcount * vercount);

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

  int ix = 0, iy = 0;

  for (size_t i = 0 ; i < inventory->GetEntityCount () ; i++)
  {
    iCelEntity* ent = inventory->GetEntity (i);
    csRef<iString> n = info->GetName (ent);
    GridEntry& g = grid[iy*horcount + ix];

    g.entity = ent;
    g.SetupEntry (this,
	style.marginhor + ix * (style.buttonw + style.marginhor),
	style.marginver + iy * (style.buttonh + style.marginver),
	n->GetData (), 0, info->GetMeshFactory (ent), 0);
    g.SetupEntry (this,
	style.marginhor + ix * (style.buttonw + style.marginhor),
	style.marginver + iy * (style.buttonh + style.marginver),
	n->GetData (), 0, info->GetMeshFactory (ent), 1);
    ix++;
    if (ix >= horcount)
    {
      ix = 0;
      iy++;
    }
  }

  for (size_t i = 0 ; i < inventory->GetEntityTemplateCount () ; i++)
  {
    iCelEntityTemplate* ent = inventory->GetEntityTemplate (i);
    int amount = inventory->GetEntityTemplateAmount (i);
    csRef<iString> n = info->GetName (ent, amount);
    GridEntry& g = grid[iy*horcount + ix];

    g.tpl = ent;
    g.SetupEntry (this,
	style.marginhor + ix * (style.buttonw + style.marginhor),
	style.marginver + iy * (style.buttonh + style.marginver),
	n->GetData (), amount, info->GetMeshFactory (ent, amount), 0);
    g.SetupEntry (this,
	style.marginhor + ix * (style.buttonw + style.marginhor),
	style.marginver + iy * (style.buttonh + style.marginver),
	n->GetData (), amount, info->GetMeshFactory (ent, amount), 1);
    ix++;
    if (ix >= horcount)
    {
      ix = 0;
      iy++;
    }
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

void celUIGridInventory::FireSelectionListeners (iCelEntity* entity)
{
  for (size_t i = 0 ; i < callbacks.GetSize () ; i++)
    callbacks[i]->SelectEntity (entity);
}

void celUIGridInventory::FireSelectionListeners (iCelEntityTemplate* tpl)
{
  for (size_t i = 0 ; i < callbacks.GetSize () ; i++)
    callbacks[i]->SelectTemplate (tpl);
}

void celUIGridInventory::Open (const char* title, iPcInventory* inventory)
{
  Activate ();

  celUIGridInventory::inventory = inventory;

  Setup ();

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

//---------------------------------------------------------------------------

