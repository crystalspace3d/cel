/*    Crystal Space Entity Layer
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

#ifndef __CEL_TOOLS_CELNAVMESH__
#define __CEL_TOOLS_CELNAVMESH__

#include "csutil/csobject.h"
#include "csutil/util.h"
#include "csutil/hash.h"
#include "csutil/redblacktree.h"
#include "csutil/weakrefarr.h"
#include "csutil/stringarray.h"
#include "csutil/eventhandlers.h"
#include "iutil/comp.h"
#include "iutil/eventh.h"
#include "iutil/eventq.h"
#include "ivaria/conin.h"
#include "ivaria/conout.h"

#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/entitytpl.h"
#include "physicallayer/propclas.h"
#include "behaviourlayer/behave.h"
#include "behaviourlayer/bl.h"
#include "tools/celgraph.h"
#include "propclass/prop.h"
#include "propclass/quest.h"



#include <stdio.h>
#include <string.h>

// This is preliminary work, with the purpose of giving the GSoC Mentors an idea of the API.

namespace AINavigation{
	/*
	* CLASS: celNavCell - Used to store cell (convex polygon) data for use in advanced features.
	* TODO: This is just a draft, need to do more research and come up with a good design.

	class celNavCell
	{
	private:
	csRef<iCelNode> geometry;               //Contains a node that represents the geometry.
	csRefArray<iCelNavCell> adjacentlinks;  //List of all the adjacent cells.
	float deltaHeight;                      //used to define vertical jump or crawl links.
	float deltaLenght;                      //used to define horizontal jumps.
	csRef navCellID;                        //cell ID.

	public:
	cell();
	~cell();	
	};

	*/

	/*
	* CLASS: celNavMesh - Used to generate the navigation mesh.
	* TODO: Test a few data structures to obtain the best performance/memory ratio.
	* use a hash table to create links between cells/adjacent polygons.
	* some functionality is still not defined.
	*/
	class celNavMesh
	{
	private:
		csArray<float> navMeshGeometry;   //Saves the usable geometry, upon processing.
		csArray<float> normalVectorList;  //Saves the normal vectors of the usable polygon geometry.
		csArray<float> surfaceAreaList;   //Saves the surface area of each convex polygon. For culling and metrics.
		csArray<celNavCell> *listofCells; //contains the cell (convex polygons) list.
		float minSubDivisionPolygonSize;  //defines the minimal polygonal size for subdivision.
		//csRef navMeshID;                //navigation mesh ID. 

	public:
		celNavMesh();
		~celNavMesh();
		virtual bool loadGeometry(csArray<float> triangleList);
		//import a list of triangles with no normal information.
		virtual bool loadGeometry(csArray<float> triangleList, csArray<float> normalVectorList); 
		//import a list of triangles with normal information.
		virtual void findNormal(); 
		//if the world geometry does not have normal information, this function can be used to calculate the normal of the triangles.
		virtual void selectWalkable(float deltaNormal); 
		//will only select triangle that have a normal in the [90-deltaNormal, 90+deltaNormal] range.
		virtual void findConvexity();
		//Uses the Hertel-Mehlhorn algorithm. Results in no more than 4 times min number of polygons.
		virtual void merge();
		//Try to minimize the number of polygons necessary to represent the mesh. Uses 3->, 2->1 polygon merge/removal algorithms.
		virtual void cull(float minSurfaceArea);
		//Culls any minor polygons that result of heavy subdivisions or particularities of the geometry.
		virtual csArray<float> toTriangles();
		//converts the navmesh geometry into a triangle list. May be useful for rendering.
		virtual bool connectCenter();
		//connects the cells using the center of the polygons.
		virtual bool connectEdge();
		//connects the cells using the center of the adjacent edges.
		//Both of these methods set the node location in the navigation graph.
		virtual bool subtract(csArray<float> obstacles);
		virtual void subdivide();
		//remove any obstacles that may be overlayed over our geometry. recursively subdivide a polygon until
		//the obstacle is removed or we reach the minimal polygons subdivision. Select free polygons, delete 
		//secluded or partially secluded polygons.
		virtual float getMinPoligonSize(); 
		//Returns the minimal polygon size for subdivision.
		virtual void setMinPolygonSize();
		//Sets the minimal polygon size for subdivision.
		virtual csRefArray<iCelNode> generateMapNode(); 
		//Generates a set of nodes ready to be used by the celGraph tool.
	};


}

#endif //__CEL_TOOLS_CELNAVMESH__ 
