/*
    Crystal Space Entity Layer
    Copyright (C) 2003 by Jorrit Tyberghein

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
#include "igraphic/image.h"

#include "plugins/managers/billboard/billboard.h"

//---------------------------------------------------------------------------

SCF_IMPLEMENT_IBASE (celBillboard)
  SCF_IMPLEMENTS_INTERFACE (iBillboard)
SCF_IMPLEMENT_IBASE_END

celBillboard::celBillboard ()
{
  SCF_CONSTRUCT_IBASE (0);
  name = 0;
  flags.SetAll (CEL_BILLBOARD_VISIBLE);
  x = y = 0;
  w = h = 10;
}

celBillboard::~celBillboard ()
{
  delete[] name;
}


bool celBillboard::SetImage (iImage* image)
{
  return true;
}

bool celBillboard::SetImage (const char* filename)
{
  return true;
}

void celBillboard::SetSize (int w, int h)
{
}

void celBillboard::SetPosition (int x, int y)
{
}

void celBillboard::Move (int dx, int dy)
{
}

//---------------------------------------------------------------------------

CS_IMPLEMENT_PLUGIN

SCF_IMPLEMENT_FACTORY (celBillboardManager)

SCF_IMPLEMENT_IBASE (celBillboardManager)
  SCF_IMPLEMENTS_INTERFACE (iBillboardManager)
  SCF_IMPLEMENTS_EMBEDDED_INTERFACE (iComponent)
SCF_IMPLEMENT_IBASE_END

SCF_IMPLEMENT_EMBEDDED_IBASE (celBillboardManager::Component)
  SCF_IMPLEMENTS_INTERFACE (iComponent)
SCF_IMPLEMENT_EMBEDDED_IBASE_END

celBillboardManager::celBillboardManager (iBase* parent)
{
  SCF_CONSTRUCT_IBASE (parent);
  SCF_CONSTRUCT_EMBEDDED_IBASE (scfiComponent);
}

celBillboardManager::~celBillboardManager ()
{
}

bool celBillboardManager::Initialize (iObjectRegistry* object_reg)
{
  celBillboardManager::object_reg = object_reg;
  return true;
}

iBillboard* celBillboardManager::CreateBillboard (const char* name)
{
  return 0;
}

iBillboard* celBillboardManager::FindBillboard (const char* name) const
{
  return 0;
}

void celBillboardManager::RemoveBillboard (iBillboard* billboard)
{
}

void celBillboardManager::RemoveAll ()
{
}

void celBillboardManager::SetFlags (uint32 flags, uint32 mask)
{
}

//---------------------------------------------------------------------------

