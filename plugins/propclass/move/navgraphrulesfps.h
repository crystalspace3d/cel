/*
    Crystal Space Entity Layer
    Copyright (C) 2003 by Steve Cook

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

#ifndef __CEL_PF_NAVGRAPHRULESFPS__
#define __CEL_PF_NAVGRAPHRULESFPS__

// CS Stuff
#include "cstypes.h"
#include "csutil/scf.h"

// PcCommon defs
#include "plugins/propclass/common/stdpcimp.h"
#include "propclass/region.h"

// Template
#include "physicallayer/facttmpl.h"

// Nav Graph Rules FPS Implementation
//===================================

struct iPcNavGraphRulesFPS;

SCF_VERSION (iPcNavGraphRulesFPS, 0, 0, 1);

struct iPcNavGraphRulesFPS : public iPcNavGraphRules
{
};

CEL_DECLARE_FACTORY (NavGraphRulesFPS)

class celPcNavGraphRulesFPS : public celPcNavGraphRules
{
    /* A default NavGraph implementation for a first person shooter
     *
     */
private:
    csRef<iPcMovableConstraint> moveconst;
    iCelEntity* previousent;

public:
  virtual int TraverseLink( celPcNavGraph* graph, iSector* sector, iPcNavLink* plink, iCelEntity* ent );
  virtual void OptimiseGraph(celPcNavGraph* graph);
  virtual int FindShortestPath( celPcNavGraph* graph, int iNodeStart, int iNodeEnd, int* &ipath );
  virtual int FindNearestNode( celPcNavGraph* graph, csVector3* point, iSector* sector, iCelEntity* ent );

  // Implement PCCommon methods
  celPcNavGraphRulesFPS (iObjectRegistry* object_reg);
  virtual ~celPcNavGraphRulesFPS ();
  virtual const char* GetName () const { return "pcgraphrulesfps"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);

  SCF_DECLARE_IBASE_EXT (celPcNavGraphRules);

  struct PcNavGraphRulesFPS : public iPcNavGraphRulesFPS
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcNavGraphRules);

      // Various virtual methods that can be used to change the graph behaviour
      virtual int TraverseLink( iPcNavGraph* graph, iSector* sector, iPcNavLink* plink, iCelEntity* ent )
      {
          return scfParent->TraverseLink( (celPcNavGraph*)(graph->GetPrivateObject ()), sector, plink, ent );
      }

      virtual void OptimiseGraph(iPcNavGraph* graph)
      {
        scfParent->OptimiseGraph( (celPcNavGraph*)(graph->GetPrivateObject ()) );    
      }

      virtual int FindShortestPath( iPcNavGraph* graph, int iNodeStart, int iNodeEnd, int* &ipath )
      {
        return scfParent->FindShortestPath( (celPcNavGraph*)(graph->GetPrivateObject ()), iNodeStart, iNodeEnd, ipath );    
      }

      virtual int FindNearestNode( iPcNavGraph* graph, csVector3* point, iSector* sector, iCelEntity* ent )
      {
        return scfParent->FindNearestNode((celPcNavGraph*)(graph->GetPrivateObject ()), point, sector, ent);    
      }

  } scfiPcNavGraphRulesFPS;

};


#endif
