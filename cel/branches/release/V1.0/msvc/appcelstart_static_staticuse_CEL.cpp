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
SCF_USE_STATIC_PLUGIN(addon_celentity)
SCF_USE_STATIC_PLUGIN(addon_celentitytpl)
SCF_USE_STATIC_PLUGIN(addon_questdef)
SCF_USE_STATIC_PLUGIN(addon_xmlscripts)
SCF_USE_STATIC_PLUGIN(blxml)
SCF_USE_STATIC_PLUGIN(celconsole)
SCF_USE_STATIC_PLUGIN(mgr_quests)
SCF_USE_STATIC_PLUGIN(mgr_billboard)
SCF_USE_STATIC_PLUGIN(pfactormove)
SCF_USE_STATIC_PLUGIN(pfbillboard)
SCF_USE_STATIC_PLUGIN(pfdamage)
SCF_USE_STATIC_PLUGIN(pfdefcam)
SCF_USE_STATIC_PLUGIN(pfengine)
SCF_USE_STATIC_PLUGIN(pfinput)
SCF_USE_STATIC_PLUGIN(pfinv)
SCF_USE_STATIC_PLUGIN(pflight)
SCF_USE_STATIC_PLUGIN(pfmechanics)
SCF_USE_STATIC_PLUGIN(pfmesh)
SCF_USE_STATIC_PLUGIN(pfmove)
SCF_USE_STATIC_PLUGIN(pfmover)
SCF_USE_STATIC_PLUGIN(pfportal)
SCF_USE_STATIC_PLUGIN(pfquest)
SCF_USE_STATIC_PLUGIN(pfsound)
SCF_USE_STATIC_PLUGIN(pftest)
SCF_USE_STATIC_PLUGIN(pftools)
SCF_USE_STATIC_PLUGIN(pftrigger)
SCF_USE_STATIC_PLUGIN(pfwheeled)
SCF_USE_STATIC_PLUGIN(pfzone)
SCF_USE_STATIC_PLUGIN(pfprojectile)
SCF_USE_STATIC_PLUGIN(stdphyslayer)
SCF_USE_STATIC_PLUGIN(xmlpersist)

