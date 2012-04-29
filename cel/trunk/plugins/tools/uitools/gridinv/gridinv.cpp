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
#include "csgeom/math3d.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/plugin.h"
#include "iutil/object.h"
#include "iutil/virtclk.h"
#include "ivideo/graph3d.h"
#include "ivideo/graph2d.h"
#include "ivaria/reporter.h"
#include "iengine/sector.h"
#include "iengine/camera.h"
#include "iengine/rendermanager.h"

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
}

bool celUIGridInventory::Initialize (iObjectRegistry* object_reg)
{
  celUIGridInventory::object_reg = object_reg;
  pl = csQueryRegistry<iCelPlLayer> (object_reg);
  engine = csQueryRegistry<iEngine> (object_reg);
  g3d = csQueryRegistry<iGraphics3D> (object_reg);
  info.AttachNew (new DefaultInfo (pl, engine));
  name_reg = csEventNameRegistry::GetRegistry (object_reg);

  return true;
}

bool celUIGridInventory::HandleEvent (iEvent& ev)
{
  if (ev.Name == csevFrame (object_reg))
  {
    g3d->BeginDraw (CSDRAW_2DGRAPHICS);
    for (size_t i = 0 ; i < grid.GetSize () ; i++)
    {
      const GridEntry& g = grid[i];
      if (g.handle)
      {
        g3d->DrawPixmap (g.handle, g.x, g.y, style.buttonw, style.buttonh,
	    0, 0, style.buttonw, style.buttonh);
      }
    }
    return true;
  }
  else if (ev.Name == csevMouseUp (name_reg, 0))
  {
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
}

// ------------------------------------------------------------------

InvStyle::InvStyle ()
{
  buttonw = 128;
  buttonh = 128;
  marginhor = 16;
  marginver = 16;
  bgred = 50;
  bggreen = 50;
  bgblue = 50;
  bgalpha = 255;
}

bool InvStyle::SetStyleOption (const char* name, const char* value)
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
    backgroundImage = value;
  }
  if (styleName == "backgroundColor")
  {
    int num = csScanStr (value, "%d,%d,%d,%d", &bgred, &bggreen, &bgblue,
	&bgalpha);
    if (num < 4) bgalpha = 255;
  }
  return false;
}

void GridEntry::SetupEntry (const InvStyle& style, iObjectRegistry* object_reg,
    iEngine* engine, iGraphics3D* g3d, iFont* font,
    int x, int y,
    const char* text, int amount,
    iMeshFactoryWrapper* factory)
{
  GridEntry::x = x;
  GridEntry::y = y;

  iTextureManager* txtmgr = g3d->GetTextureManager ();
  handle = txtmgr->CreateTexture (style.buttonw, style.buttonh, csimg2D, "rgba8",
      CS_TEXTURE_3D);
  g3d->SetRenderTarget (handle);
  g3d->BeginDraw (CSDRAW_2DGRAPHICS);

  iGraphics2D* g2d = g3d->GetDriver2D ();
  int color = g2d->FindRGB (style.bgred, style.bggreen, style.bgblue, style.bgalpha);
  g2d->DrawBox (0, 0, style.buttonw, style.buttonh, color);
  if (style.backgroundTexture)
  {
    g3d->DrawPixmap (style.backgroundTexture, 0, 0, style.buttonw, style.buttonh,
	    0, 0, style.buttonw, style.buttonh);
  }

  if (factory)
  {
    csMeshOnTexture* mt = new csMeshOnTexture (object_reg);
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
    csRef<iMeshWrapper> mesh = engine->CreateMeshWrapper (factory, "__gridmesh__", sector);
    iCamera* cam = mt->GetView ()->GetCamera ();
    cam->SetSector (sector);
    mt->ScaleCamera (mesh, style.buttonw, style.buttonh);
    cam->Move (csVector3 (.7, .7, -.5));
    mt->ScaleCamera (mesh, style.buttonw, style.buttonh);
    cam->GetTransform ().LookAt (-cam->GetTransform ().GetOrigin (), csVector3 (0, 1, 0));
    float dist = sqrt (csSquaredDist::PointPoint (cam->GetTransform ().GetOrigin (),
	  csVector3 (0, 0, 0)));
    cam->Move (csVector3 (0, 0, - dist / 10.0f));
    mt->Render (mesh, handle, true);
    iRenderManager* rm = engine->GetRenderManager ();
    rm->RenderView (mt->GetView ());
  }


  int fg = g2d->FindRGB (255, 255, 255);
  printf ("text=%s\n", text); fflush (stdout);
  if (amount)
  {
    csString t;
    t.Format ("%s (%d)", text, amount);
    g2d->Write (font, 10, 10, fg, color, t);
  }
  else
    g2d->Write (font, 10, 10, fg, color, text);

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
  font = g3d->GetDriver2D ()->GetFontServer ()->LoadFont (CSFONT_COURIER);

  if (!style.backgroundImage.IsEmpty ())
  {
    iTextureWrapper* txt = engine->CreateTexture (style.backgroundImage,
	style.backgroundImage, 0, CS_TEXTURE_3D);
    if (!txt)
    {
      printf ("Error loading image '%s'!\n", style.backgroundImage.GetData ());
      return;
    }
    iTextureManager* txtmgr = g3d->GetTextureManager ();
    txt->Register (txtmgr);
    style.backgroundTexture = txt->GetTextureHandle ();
  }

  int ix = 0, iy = 0;

  for (size_t i = 0 ; i < inventory->GetEntityCount () ; i++)
  {
    iCelEntity* ent = inventory->GetEntity (i);
    csRef<iString> n = info->GetName (ent);
    grid[iy*horcount + ix].SetupEntry (style, object_reg, engine, g3d, font,
	style.marginhor + ix * (style.buttonw + style.marginhor),
	style.marginver + iy * (style.buttonh + style.marginver),
	n->GetData (), 0, info->GetMeshFactory (ent));
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
    grid[iy*horcount + ix].SetupEntry (style, object_reg, engine, g3d, font,
	style.marginhor + ix * (style.buttonw + style.marginhor),
	style.marginver + iy * (style.buttonh + style.marginver),
	n->GetData (), amount, info->GetMeshFactory (ent, amount));
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

bool celUIGridInventory::OkButton ()
{
  Deactivate ();
  //window->hide();
  if (inventory && listener)
    inventory->RemoveInventoryListener (listener);
  inventory = 0;
  return true;
}

bool celUIGridInventory::CancelButton ()
{
  Deactivate ();
  //window->hide();
  if (inventory && listener)
    inventory->RemoveInventoryListener (listener);
  inventory = 0;
  return true;
}

bool celUIGridInventory::Select ()
{
  size_t itemIdx = 0;
  if (itemIdx < inventory->GetEntityCount ())
  {
    FireSelectionListeners (inventory->GetEntity (itemIdx));
    return true;
  }
  itemIdx -= inventory->GetEntityCount ();
  FireSelectionListeners (inventory->GetEntityTemplate (itemIdx));

  return true;
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
  //window->show ();
}

void celUIGridInventory::Close ()
{
  Setup ();
  //window->hide();
  if (inventory && listener)
    inventory->RemoveInventoryListener (listener);
  inventory = 0;
}

//---------------------------------------------------------------------------

