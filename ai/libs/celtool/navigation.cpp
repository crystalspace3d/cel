/*
    Crystal Space Entity Layer
    Navigation Tools
    Copyright (C) 2007 by Dariusz Dawidowski

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


#include "csqsqrt.h"
#include "csgeom/math3d.h"
#include "csgeom/quaternion.h"
#include "iengine/mesh.h"
#include "iengine/movable.h"
#include "ivaria/mapnode.h"

#include "celtool/navigation.h"
#include "physicallayer/propclas.h"
#include "propclass/mesh.h"


celNavigationInfo celNavigationTools::GetNavigationInfo (
	iCelEntity* navigator, const char* tag,
	const csVector3& vector)
{
  celNavigationInfo info;
  info.success = false;
  csRef<iPcMesh> pcmesh;
  if (tag)
    pcmesh = CEL_QUERY_PROPCLASS_TAG_ENT (navigator, iPcMesh, tag);
  else
    pcmesh = CEL_QUERY_PROPCLASS_ENT (navigator, iPcMesh);
  if (!pcmesh) return info;
  iMeshWrapper* mesh = pcmesh->GetMesh ();
  if (!mesh) return info;
  iMovable* movable = mesh->GetMovable ();
  if (!movable) return info;
  csVector3 navi_pos = movable->GetPosition ();
  info.angle.x = -atan2 (navi_pos.z - vector.z, navi_pos.y - vector.y);
  info.angle.y = -atan2 (navi_pos.x - vector.x, navi_pos.z - vector.z);
  info.angle.z = -atan2 (navi_pos.y - vector.y, navi_pos.x - vector.x);
  info.distance = csQsqrt (csSquaredDist::PointPoint (navi_pos, vector));
  info.visible = mesh->HitBeam (navi_pos, vector).hit;
  info.success = true;
  return info;
}

celNavigationInfo celNavigationTools::GetNavigationInfo (
	iPcMesh* navigator,
	const csVector3& vector)
{
  celNavigationInfo info;
  info.success = false;
  iMeshWrapper* mesh = navigator->GetMesh ();
  if (!mesh) return info;
  iMovable* movable = mesh->GetMovable ();
  if (!movable) return info;
  csVector3 navi_pos = movable->GetPosition ();
  info.angle.x = -atan2 (navi_pos.z - vector.z, navi_pos.y - vector.y);
  info.angle.y = -atan2 (navi_pos.x - vector.x, navi_pos.z - vector.z);
  info.angle.z = -atan2 (navi_pos.y - vector.y, navi_pos.x - vector.x);
  info.distance = csQsqrt (csSquaredDist::PointPoint (navi_pos, vector));
  info.visible = mesh->HitBeam (navi_pos, vector).hit;
  info.success = true;
  return info;
}

celNavigationInfo celNavigationTools::GetNavigationInfo (
	iCelEntity* navigator, const char* tag,
	iCelEntity* entity, const char* entity_tag)
{
  celNavigationInfo info;
  info.success = false;
  csRef<iPcMesh> navi_pcmesh;
  csRef<iPcMesh> targ_pcmesh;
  if (tag)
    navi_pcmesh = CEL_QUERY_PROPCLASS_TAG_ENT (navigator, iPcMesh, tag);
  else
    navi_pcmesh = CEL_QUERY_PROPCLASS_ENT (navigator, iPcMesh);
  if (entity_tag)
    targ_pcmesh = CEL_QUERY_PROPCLASS_TAG_ENT (entity, iPcMesh, entity_tag);
  else
    targ_pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
  if (!navi_pcmesh || !targ_pcmesh) return info;
  iMeshWrapper* navi_mesh = navi_pcmesh->GetMesh ();
  iMeshWrapper* targ_mesh = targ_pcmesh->GetMesh ();
  if (!navi_mesh || !targ_mesh) return info;
  iMovable* navi_movable = navi_mesh->GetMovable ();
  iMovable* targ_movable = targ_mesh->GetMovable ();
  if (!navi_movable || !targ_movable) return info;
  csVector3 navi_pos = navi_movable->GetPosition ();
  csVector3 targ_pos = targ_movable->GetPosition ();
  info.angle.x = -atan2 (navi_pos.z - targ_pos.z, navi_pos.y - targ_pos.y);
  info.angle.y = -atan2 (navi_pos.x - targ_pos.x, navi_pos.z - targ_pos.z);
  info.angle.z = -atan2 (navi_pos.y - targ_pos.y, navi_pos.x - targ_pos.x);
  info.distance = csQsqrt (csSquaredDist::PointPoint (navi_pos, targ_pos));
  info.visible = navi_mesh->HitBeam (navi_pos, targ_pos).hit;
  info.success = true;
  return info;
}

celNavigationInfo celNavigationTools::GetNavigationInfo (
	iPcMesh* navigator,
	iCelEntity* entity, const char* entity_tag)
{
  celNavigationInfo info;
  info.success = false;
  csRef<iPcMesh> targ_pcmesh;
  if (entity_tag)
    targ_pcmesh = CEL_QUERY_PROPCLASS_TAG_ENT (entity, iPcMesh, entity_tag);
  else
    targ_pcmesh = CEL_QUERY_PROPCLASS_ENT (entity, iPcMesh);
  if (!targ_pcmesh) return info;
  iMeshWrapper* navi_mesh = navigator->GetMesh ();
  iMeshWrapper* targ_mesh = targ_pcmesh->GetMesh ();
  if (!navi_mesh || !targ_mesh) return info;
  iMovable* navi_movable = navi_mesh->GetMovable ();
  iMovable* targ_movable = targ_mesh->GetMovable ();
  if (!navi_movable || !targ_movable) return info;
  csVector3 navi_pos = navi_movable->GetPosition ();
  csVector3 targ_pos = targ_movable->GetPosition ();
  info.angle.x = -atan2 (navi_pos.z - targ_pos.z, navi_pos.y - targ_pos.y);
  info.angle.y = -atan2 (navi_pos.x - targ_pos.x, navi_pos.z - targ_pos.z);
  info.angle.z = -atan2 (navi_pos.y - targ_pos.y, navi_pos.x - targ_pos.x);
  info.distance = csQsqrt (csSquaredDist::PointPoint (navi_pos, targ_pos));
  info.visible = navi_mesh->HitBeam (navi_pos, targ_pos).hit;
  info.success = true;
  return info;
}

celNavigationInfo celNavigationTools::GetNavigationInfo (
	iCelEntity* navigator, const char* tag,
	iMapNode* node)
{
  celNavigationInfo info;
  info.success = false;
  csRef<iPcMesh> pcmesh;
  if (tag)
    pcmesh = CEL_QUERY_PROPCLASS_TAG_ENT (navigator, iPcMesh, tag);
  else
    pcmesh = CEL_QUERY_PROPCLASS_ENT (navigator, iPcMesh);
  if (!pcmesh) return info;
  iMeshWrapper* mesh = pcmesh->GetMesh ();
  if (!mesh) return info;
  iMovable* movable = mesh->GetMovable ();
  if (!movable) return info;
  csVector3 navi_pos = movable->GetPosition ();
  csVector3 targ_pos = node->GetPosition ();
  info.angle.x = -atan2 (navi_pos.z - targ_pos.z, navi_pos.y - targ_pos.y);
  info.angle.y = -atan2 (navi_pos.x - targ_pos.x, navi_pos.z - targ_pos.z);
  info.angle.z = -atan2 (navi_pos.y - targ_pos.y, navi_pos.x - targ_pos.x);
  info.distance = csQsqrt (csSquaredDist::PointPoint (navi_pos, targ_pos));
  info.visible = mesh->HitBeam (navi_pos, targ_pos).hit;
  info.success = true;
  return info;
}

celNavigationInfo celNavigationTools::GetNavigationInfo (
	iPcMesh* navigator,
	iMapNode* node)
{
  celNavigationInfo info;
  info.success = false;
  iMeshWrapper* mesh = navigator->GetMesh ();
  if (!mesh) return info;
  iMovable* movable = mesh->GetMovable ();
  if (!movable) return info;
  csVector3 navi_pos = movable->GetPosition ();
  csVector3 targ_pos = node->GetPosition ();
  info.angle.x = -atan2 (navi_pos.z - targ_pos.z, navi_pos.y - targ_pos.y);
  info.angle.y = -atan2 (navi_pos.x - targ_pos.x, navi_pos.z - targ_pos.z);
  info.angle.z = -atan2 (navi_pos.y - targ_pos.y, navi_pos.x - targ_pos.x);
  info.distance = csQsqrt (csSquaredDist::PointPoint (navi_pos, targ_pos));
  info.visible = mesh->HitBeam (navi_pos, targ_pos).hit;
  info.success = true;
  return info;
}
