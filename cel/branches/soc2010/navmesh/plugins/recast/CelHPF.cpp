/*
    Crystal Space Entity Layer
    Copyright (C) 2009 by Jorrit Tyberghein
  
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


#include "CelHPF.h"

CS_PLUGIN_NAMESPACE_BEGIN(celNavMesh)
{

// TODO already declared in CelNavMeshApi.cpp
//CS_IMPLEMENT_PLUGIN

/*
 * celHPath
 */

celHPath::celHPath () : scfImplementationType (this)
{
}

celHPath::~celHPath ()
{
}
/*
// TODO implement
iObject* celHPath::QueryObject ()
{
  return 0;
}
*/
// TODO implement
void celHPath::AddNode (iMapNode* node)
{
}

// TODO implement
void celHPath::InsertNode (size_t pos, iMapNode* node)
{
}

// TODO implement
iMapNode* celHPath::Next ()
{
  return 0;
}

// TODO implement
iMapNode* celHPath::Previous ()
{
  return 0;
}

// TODO implement
iMapNode* celHPath::Current ()
{
  return 0;
}

// TODO implement
csVector3 celHPath::CurrentPosition ()
{
  return csVector3();
}

// TODO implement
iSector* celHPath::CurrentSector ()
{
  return 0;
}

// TODO implement
bool celHPath::HasNext ()
{
  return false;
}

// TODO implement
bool celHPath::HasPrevious ()
{
  return false;
}

// TODO implement
void celHPath::Restart ()
{
}

// TODO implement
void celHPath::Clear ()
{
}

// TODO implement
iMapNode* celHPath::GetFirst ()
{
  return 0;
}

// TODO implement
iMapNode* celHPath::GetLast ()
{
  return 0;
}

// TODO implement
void celHPath::Invert ()
{
}

// TODO implement
size_t celHPath::GetNodeCount()
{
  return 0;
}



/*
 * celHNavStruct
 */

celHNavStruct::celHNavStruct () : scfImplementationType (this)
{
}

celHNavStruct::~celHNavStruct ()
{
}

// TODO implement
iCelPath* celHNavStruct::ShortestPath (const csVector3& from, iSector* fromSector, const csVector3& goal,
  iSector* goalSector) const
{
  return 0;
}

// TODO implement
iCelPath* celHNavStruct::ShortestPath (iMapNode* from, iMapNode* goal) const
{
  return 0;
}

// TODO implement
bool celHNavStruct::SaveToFile (const csString& file)
{
  return false;
}

// TODO implement
bool celHNavStruct::LoadFromFile (const csString& file)
{
  return false;
}

iCelNavMeshParams* celHNavStruct::GetNavMeshParams ()
{
  return parameters;
}

void celHNavStruct::SetNavMeshParams (iCelNavMeshParams* parameters)
{
  this->parameters.AttachNew(new celNavMeshParams(parameters));
}



/*
 * celHNavMeshBuilder
 */

SCF_IMPLEMENT_FACTORY (celHNavStructBuilder)

celHNavStructBuilder::celHNavStructBuilder (iBase* parent) : scfImplementationType (this, parent)
{
}

celHNavStructBuilder::~celHNavStructBuilder ()
{
}

bool celHNavStructBuilder::Initialize (iObjectRegistry* objectRegistry) 
{
  this->objectRegistry = objectRegistry;
  return true;
}

// TODO implement
void celHNavStructBuilder::SetSectors(csList<iSector*> sectorList)
{
}

// TODO implement
iCelHNavStruct* celHNavStructBuilder::BuildHNavStruct ()
{
  return 0;
}

// TODO implement
bool celHNavStructBuilder::UpdateNavMesh (iCelHNavStruct* hNavStruct, const csBox3& boundingBox, iSector* sector)
{
  if (sector) 
  {

  }
  return false;
}

// TODO implement
bool celHNavStructBuilder::UpdateNavMesh (iCelHNavStruct* hNavStruct, const csOBB& boundingBox, iSector* sector)
{
  if (sector)
  {

  }
  return false;
}

iCelNavMeshParams* celHNavStructBuilder::GetNavMeshParams ()
{
  return parameters;
}

void celHNavStructBuilder::SetNavMeshParams (iCelNavMeshParams* parameters)
{
  this->parameters.AttachNew(new celNavMeshParams(parameters));
}

}
CS_PLUGIN_NAMESPACE_END(celNavMesh)
