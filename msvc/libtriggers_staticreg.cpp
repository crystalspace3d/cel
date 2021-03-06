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
static char const metainfo_triggers[] =
"<?xml version=\"1.0\"?>"
"<!-- triggers.csplugin -->"
"<plugin>"
"  <scf>"
"    <classes>"
"      <class>"
"        <name>cel.triggers.entersector</name>"
"        <implementation>celEnterSectorTriggerType</implementation>"
"        <description>CEL Trigger: Triggers when camera enters a sector</description>"
"      </class>"
""
"      <class>"
"        <name>cel.triggers.inventory</name>"
"        <implementation>celInventoryTriggerType</implementation>"
"        <description>CEL Trigger: Triggers when a certain inventory gets some entity</description>"
"      </class>"
""
"      <class>"
"        <name>cel.triggers.meshentersector</name>"
"        <implementation>celMeshEnterSectorTriggerType</implementation>"
"        <description>CEL Trigger: Triggers when a mesh enters a sector</description>"
"      </class>"
""
"      <class>"
"        <name>cel.triggers.meshselect</name>"
"        <implementation>celMeshSelectTriggerType</implementation>"
"        <description>CEL Trigger: Triggers when a mesh is selected</description>"
"      </class>"
""
"      <class>"
"        <name>cel.triggers.message</name>"
"        <implementation>celMessageTriggerType</implementation>"
"        <description>CEL Trigger: Triggers when an entity sends a message</description>"
"      </class>"
""
"      <class>"
"        <name>cel.triggers.operation</name>"
"        <implementation>celOperationTriggerType</implementation>"
"        <description>CEL Trigger: Triggers when a combination of triggers fire</description>"
"      </class>"
""
"      <class>"
"        <name>cel.triggers.propertychange</name>"
"        <implementation>celPropertyChangeTriggerType</implementation>"
"        <description>CEL Trigger: Triggers when a property changes</description>"
"      </class>"
""
"      <class>"
"        <name>cel.triggers.sequencefinish</name>"
"        <implementation>celSequenceFinishTriggerType</implementation>"
"        <description>CEL Trigger: Triggers when a sequence finishes</description>"
"      </class>"
"      "
"      <class>"
"        <name>cel.triggers.timeout</name>"
"        <implementation>celTimeoutTriggerType</implementation>"
"        <description>CEL Trigger: Triggers after time</description>"
"      </class>"
""
"      <class>"
"        <name>cel.triggers.trigger</name>"
"        <implementation>celTriggerTriggerType</implementation>"
"        <description>CEL Trigger: Triggers after a pcTrigger</description>"
"      </class>"
""
"      <class>"
"        <name>cel.triggers.watch</name>"
"        <implementation>celWatchTriggerType</implementation>"
"        <description>CEL Trigger: Triggers when a mesh becomes visible</description>"
"      </class>"
"    </classes>"
"  </scf>"
"</plugin>"
;
  #ifndef celEnterSectorTriggerType_FACTORY_REGISTER_DEFINED 
  #define celEnterSectorTriggerType_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celEnterSectorTriggerType) 
  #endif
  #ifndef celInventoryTriggerType_FACTORY_REGISTER_DEFINED 
  #define celInventoryTriggerType_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celInventoryTriggerType) 
  #endif
  #ifndef celMeshEnterSectorTriggerType_FACTORY_REGISTER_DEFINED 
  #define celMeshEnterSectorTriggerType_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celMeshEnterSectorTriggerType) 
  #endif
  #ifndef celMeshSelectTriggerType_FACTORY_REGISTER_DEFINED 
  #define celMeshSelectTriggerType_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celMeshSelectTriggerType) 
  #endif
  #ifndef celMessageTriggerType_FACTORY_REGISTER_DEFINED 
  #define celMessageTriggerType_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celMessageTriggerType) 
  #endif
  #ifndef celOperationTriggerType_FACTORY_REGISTER_DEFINED 
  #define celOperationTriggerType_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celOperationTriggerType) 
  #endif
  #ifndef celPropertyChangeTriggerType_FACTORY_REGISTER_DEFINED 
  #define celPropertyChangeTriggerType_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celPropertyChangeTriggerType) 
  #endif
  #ifndef celSequenceFinishTriggerType_FACTORY_REGISTER_DEFINED 
  #define celSequenceFinishTriggerType_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celSequenceFinishTriggerType) 
  #endif
  #ifndef celTimeoutTriggerType_FACTORY_REGISTER_DEFINED 
  #define celTimeoutTriggerType_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celTimeoutTriggerType) 
  #endif
  #ifndef celTriggerTriggerType_FACTORY_REGISTER_DEFINED 
  #define celTriggerTriggerType_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celTriggerTriggerType) 
  #endif
  #ifndef celWatchTriggerType_FACTORY_REGISTER_DEFINED 
  #define celWatchTriggerType_FACTORY_REGISTER_DEFINED 
    SCF_DEFINE_FACTORY_FUNC_REGISTRATION(celWatchTriggerType) 
  #endif

class triggers
{
SCF_REGISTER_STATIC_LIBRARY(triggers,metainfo_triggers)
  #ifndef celEnterSectorTriggerType_FACTORY_REGISTERED 
  #define celEnterSectorTriggerType_FACTORY_REGISTERED 
    celEnterSectorTriggerType_StaticInit celEnterSectorTriggerType_static_init__; 
  #endif
  #ifndef celInventoryTriggerType_FACTORY_REGISTERED 
  #define celInventoryTriggerType_FACTORY_REGISTERED 
    celInventoryTriggerType_StaticInit celInventoryTriggerType_static_init__; 
  #endif
  #ifndef celMeshEnterSectorTriggerType_FACTORY_REGISTERED 
  #define celMeshEnterSectorTriggerType_FACTORY_REGISTERED 
    celMeshEnterSectorTriggerType_StaticInit celMeshEnterSectorTriggerType_static_init__; 
  #endif
  #ifndef celMeshSelectTriggerType_FACTORY_REGISTERED 
  #define celMeshSelectTriggerType_FACTORY_REGISTERED 
    celMeshSelectTriggerType_StaticInit celMeshSelectTriggerType_static_init__; 
  #endif
  #ifndef celMessageTriggerType_FACTORY_REGISTERED 
  #define celMessageTriggerType_FACTORY_REGISTERED 
    celMessageTriggerType_StaticInit celMessageTriggerType_static_init__; 
  #endif
  #ifndef celOperationTriggerType_FACTORY_REGISTERED 
  #define celOperationTriggerType_FACTORY_REGISTERED 
    celOperationTriggerType_StaticInit celOperationTriggerType_static_init__; 
  #endif
  #ifndef celPropertyChangeTriggerType_FACTORY_REGISTERED 
  #define celPropertyChangeTriggerType_FACTORY_REGISTERED 
    celPropertyChangeTriggerType_StaticInit celPropertyChangeTriggerType_static_init__; 
  #endif
  #ifndef celSequenceFinishTriggerType_FACTORY_REGISTERED 
  #define celSequenceFinishTriggerType_FACTORY_REGISTERED 
    celSequenceFinishTriggerType_StaticInit celSequenceFinishTriggerType_static_init__; 
  #endif
  #ifndef celTimeoutTriggerType_FACTORY_REGISTERED 
  #define celTimeoutTriggerType_FACTORY_REGISTERED 
    celTimeoutTriggerType_StaticInit celTimeoutTriggerType_static_init__; 
  #endif
  #ifndef celTriggerTriggerType_FACTORY_REGISTERED 
  #define celTriggerTriggerType_FACTORY_REGISTERED 
    celTriggerTriggerType_StaticInit celTriggerTriggerType_static_init__; 
  #endif
  #ifndef celWatchTriggerType_FACTORY_REGISTERED 
  #define celWatchTriggerType_FACTORY_REGISTERED 
    celWatchTriggerType_StaticInit celWatchTriggerType_static_init__; 
  #endif
public:
 triggers();
};
triggers::triggers() {}

}
