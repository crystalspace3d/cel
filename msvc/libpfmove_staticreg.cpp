// This file is automatically generated.
#include "cssysdef.h"
#include "csutil/scf.h"

// Put static linking stuff into own section.
// The idea is that this allows the section to be swapped out but not
// swapped in again b/c something else in it was needed.
#if !defined(CS_DEBUG) && defined(CS_COMPILER_MSVC)
#pragma const_seg(".CSmetai")
#pragma comment(linker, "/section:.CSmetai,r")
#pragma code_seg(".CSmeta")
#pragma comment(linker, "/section:.CSmeta,er")
#pragma comment(linker, "/merge:.CSmetai=.CSmeta")
#endif

namespace csStaticPluginInit
{
static char const metainfo_pfmove[] =
"<?xml version=\"1.0\"?>"
"<!-- pfmove.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <!-- @@@ to be deprecated -->"
"      <class>"
"        <name>cel.pcfactory.movable</name>"
"        <implementation>celPfMovable</implementation>"
"        <description>CEL Movable Property Class Factory</description>"
"        <requires>"
"          <class>cel.physicallayer</class>"
"        </requires>"
"      </class>"
"      <!-- @@@ to be deprecated -->"
"      <class>"
"        <name>cel.pcfactory.solid</name>"
"        <implementation>celPfSolid</implementation>"
"        <description>CEL Solid Property Class Factory</description>"
"        <requires>"
"          <class>cel.physicallayer</class>"
"        </requires>"
"      </class>"
"      <!-- @@@ to be deprecated -->"
"      <class>"
"        <name>cel.pcfactory.movableconst_cd</name>"
"        <implementation>celPfMovableConstraintCD</implementation>"
"        <description>CEL MovableConstraintCD Class Factory</description>"
"        <requires>"
"          <class>cel.physicallayer</class>"
"        </requires>"
"      </class>"
"      <!-- @@@ to be deprecated -->"
"      <class>"
"        <name>cel.pcfactory.gravity</name>"
"        <implementation>celPfGravity</implementation>"
"        <description>CEL MovableConstraintCD Class Factory</description>"
"        <requires>"
"          <class>cel.physicallayer</class>"
"        </requires>"
"      </class>"
"      <!-- @@@ to be deprecated -->"
"      <class>"
"        <name>cel.pcfactory.linearmovement</name>"
"        <implementation>celPfLinearMovement</implementation>"
"        <description>CEL Linear Movement Class Factory</description>"
"        <requires>"
"          <class>cel.physicallayer</class>"
"        </requires>"
"      </class>"
"      <!-- @@@ to be deprecated -->"
"      <class>"
"        <name>cel.pcfactory.collisiondetection</name>"
"        <implementation>celPfCollisionDetection</implementation>"
"        <description>CEL Collision Detection Class Factory</description>"
"        <requires>"
"          <class>cel.physicallayer</class>"
"        </requires>"
"      </class>"
""
"      <class>"
"        <name>cel.pcfactory.move.solid</name>"
"        <implementation>celPfSolid</implementation>"
"        <description>CEL Solid Property Class Factory</description>"
"        <requires>"
"          <class>cel.physicallayer</class>"
"        </requires>"
"      </class>"
"      <class>"
"        <name>cel.pcfactory.move.linear</name>"
"        <implementation>celPfLinearMovement</implementation>"
"        <description>CEL Linear Movement Class Factory</description>"
"        <requires>"
"          <class>cel.physicallayer</class>"
"        </requires>"
"      </class>"
"      <class>"
"        <name>cel.pcfactory.object.mesh.collisiondetection</name>"
"        <implementation>celPfCollisionDetection</implementation>"
"        <description>CEL Collision Detection Class Factory</description>"
"        <requires>"
"          <class>cel.physicallayer</class>"
"        </requires>"
"      </class>"
"      <class>"
"        <name>cel.pcfactory.move.movable</name>"
"        <implementation>celPfMovable</implementation>"
"        <description>CEL Movable Property Class Factory</description>"
"        <requires>"
"          <class>cel.physicallayer</class>"
"        </requires>"
"      </class>"
"      <class>"
"        <name>cel.pcfactory.move.movableconst_cd</name>"
"        <implementation>celPfMovableConstraintCD</implementation>"
"        <description>CEL MovableConstraintCD Class Factory</description>"
"        <requires>"
"          <class>cel.physicallayer</class>"
"        </requires>"
"      </class>"
"      <class>"
"        <name>cel.pcfactory.move.gravity</name>"
"        <implementation>celPfGravity</implementation>"
"        <description>CEL MovableConstraintCD Class Factory</description>"
"        <requires>"
"          <class>cel.physicallayer</class>"
"        </requires>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celPfMovable_FACTORY_REGISTER_DEFINED 
  #define celPfMovable_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfMovable) 
  #endif
  #ifndef celPfSolid_FACTORY_REGISTER_DEFINED 
  #define celPfSolid_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfSolid) 
  #endif
  #ifndef celPfMovableConstraintCD_FACTORY_REGISTER_DEFINED 
  #define celPfMovableConstraintCD_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfMovableConstraintCD) 
  #endif
  #ifndef celPfGravity_FACTORY_REGISTER_DEFINED 
  #define celPfGravity_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfGravity) 
  #endif
  #ifndef celPfLinearMovement_FACTORY_REGISTER_DEFINED 
  #define celPfLinearMovement_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfLinearMovement) 
  #endif
  #ifndef celPfCollisionDetection_FACTORY_REGISTER_DEFINED 
  #define celPfCollisionDetection_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfCollisionDetection) 
  #endif
  #ifndef celPfSolid_FACTORY_REGISTER_DEFINED 
  #define celPfSolid_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfSolid) 
  #endif
  #ifndef celPfLinearMovement_FACTORY_REGISTER_DEFINED 
  #define celPfLinearMovement_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfLinearMovement) 
  #endif
  #ifndef celPfCollisionDetection_FACTORY_REGISTER_DEFINED 
  #define celPfCollisionDetection_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfCollisionDetection) 
  #endif
  #ifndef celPfMovable_FACTORY_REGISTER_DEFINED 
  #define celPfMovable_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfMovable) 
  #endif
  #ifndef celPfMovableConstraintCD_FACTORY_REGISTER_DEFINED 
  #define celPfMovableConstraintCD_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfMovableConstraintCD) 
  #endif
  #ifndef celPfGravity_FACTORY_REGISTER_DEFINED 
  #define celPfGravity_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPfGravity) 
  #endif

class pfmove
{
SCF_REGISTER_STATIC_LIBRARY(pfmove,metainfo_pfmove)
  #ifndef celPfMovable_FACTORY_REGISTERED 
  #define celPfMovable_FACTORY_REGISTERED 
    celPfMovable_StaticInit celPfMovable_static_init__; 
  #endif
  #ifndef celPfSolid_FACTORY_REGISTERED 
  #define celPfSolid_FACTORY_REGISTERED 
    celPfSolid_StaticInit celPfSolid_static_init__; 
  #endif
  #ifndef celPfMovableConstraintCD_FACTORY_REGISTERED 
  #define celPfMovableConstraintCD_FACTORY_REGISTERED 
    celPfMovableConstraintCD_StaticInit celPfMovableConstraintCD_static_init__; 
  #endif
  #ifndef celPfGravity_FACTORY_REGISTERED 
  #define celPfGravity_FACTORY_REGISTERED 
    celPfGravity_StaticInit celPfGravity_static_init__; 
  #endif
  #ifndef celPfLinearMovement_FACTORY_REGISTERED 
  #define celPfLinearMovement_FACTORY_REGISTERED 
    celPfLinearMovement_StaticInit celPfLinearMovement_static_init__; 
  #endif
  #ifndef celPfCollisionDetection_FACTORY_REGISTERED 
  #define celPfCollisionDetection_FACTORY_REGISTERED 
    celPfCollisionDetection_StaticInit celPfCollisionDetection_static_init__; 
  #endif
  #ifndef celPfSolid_FACTORY_REGISTERED 
  #define celPfSolid_FACTORY_REGISTERED 
    celPfSolid_StaticInit celPfSolid_static_init__; 
  #endif
  #ifndef celPfLinearMovement_FACTORY_REGISTERED 
  #define celPfLinearMovement_FACTORY_REGISTERED 
    celPfLinearMovement_StaticInit celPfLinearMovement_static_init__; 
  #endif
  #ifndef celPfCollisionDetection_FACTORY_REGISTERED 
  #define celPfCollisionDetection_FACTORY_REGISTERED 
    celPfCollisionDetection_StaticInit celPfCollisionDetection_static_init__; 
  #endif
  #ifndef celPfMovable_FACTORY_REGISTERED 
  #define celPfMovable_FACTORY_REGISTERED 
    celPfMovable_StaticInit celPfMovable_static_init__; 
  #endif
  #ifndef celPfMovableConstraintCD_FACTORY_REGISTERED 
  #define celPfMovableConstraintCD_FACTORY_REGISTERED 
    celPfMovableConstraintCD_StaticInit celPfMovableConstraintCD_static_init__; 
  #endif
  #ifndef celPfGravity_FACTORY_REGISTERED 
  #define celPfGravity_FACTORY_REGISTERED 
    celPfGravity_StaticInit celPfGravity_static_init__; 
  #endif
public:
 pfmove();
};
pfmove::pfmove() {}

}
