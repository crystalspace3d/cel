/*
    Crystal Space Entity Layer
    Copyright (C) 2008 by Jorrit Tyberghein

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
#include "csutil/objreg.h"
#include "csutil/event.h"
#include "csutil/cfgacc.h"
#include "csutil/inputdef.h"
#include "csutil/array.h"
#include "csutil/priorityqueue.h"
#include "cstool/mapnode.h"
#include "ivaria/mapnode.h"
#include "csgeom/math3d.h"
#include "iutil/evdefs.h"
#include "iutil/event.h"
#include "iutil/plugin.h"
#include "iutil/objreg.h"
#include "ivideo/graph3d.h"
#include "ivaria/reporter.h"
#include "ivaria/conin.h"
#include "ivaria/script.h"
#include "csutil/randomgen.h"
#include "csutil/hash.h"
#include "csgeom/*"

//WIP - Not Functional. Not Feature Complete. Just an idea of the API.
//Need to study the code that is already implemented in CS, in particular Geometry algorithms in utilities.

#include "tools/expression.h"
#include "plugins/tools/celnavmesh/celnavmesh.h"

CS_IMPLEMENT_PLUGIN

AINavigation::celNavMesh::celNavMesh()
{
	//STUB
}

AINavigation::celNavMesh::~celNavMesh()
{
	//STUB
}

bool AINavigation::celNavMesh::loadGeometry( csArray<float> triangleList )
{
	//STUB
}

bool AINavigation::celNavMesh::loadGeometry( csArray<float> triangleList, csArray<float> normalVectorList )
{
	//STUB
}

void AINavigation::celNavMesh::findNormal()
{
	//STUB
}

void AINavigation::celNavMesh::findConvexity()
{
	//STUB
}

void AINavigation::celNavMesh::merge()
{
	//STUB
}

void AINavigation::celNavMesh::cull( float minSurfaceArea )
{
	//STUB
}

csArray<float> AINavigation::celNavMesh::toTriangles()
{
	//STUB
}

bool AINavigation::celNavMesh::connectCenter()
{
//STUB
}

bool AINavigation::celNavMesh::connectEdge()
{
//STUB
}

bool AINavigation::celNavMesh::subtract( csArray<float> obstacles )
{
//STUB
}

void AINavigation::celNavMesh::subdivide()
{
//STUB
}

float AINavigation::celNavMesh::getMinPoligonSize()
{
//STUB
}

void AINavigation::celNavMesh::setMinPolygonSize()
{
//STUB
}

csRefArray<iCelNode> AINavigation::celNavMesh::generateMapNode()
{
//STUB
}









