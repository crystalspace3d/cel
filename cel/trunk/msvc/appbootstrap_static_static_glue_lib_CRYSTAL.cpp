// This file is automatically generated.
#include "cssysdef.h"

#if defined(CS_GLUE_IS_SHARED)
  CS_DECLARE_DEFAULT_STATIC_VARIABLE_REGISTRATION
  // Needed to work around some order of initialization issues
  static void csStaticVarCleanup_local (void (*p)())
  { csStaticVarCleanup_csutil (p); }
  CS_DEFINE_STATIC_VARIABLE_REGISTRATION (csStaticVarCleanup_local);
  #define BIND_EXPORT CS_EXPORT_SYM
#else
  #define BIND_EXPORT
#endif
struct BIND_EXPORT _Bind_bootstrap_static_plugins_CRYSTAL
{
  _Bind_bootstrap_static_plugins_CRYSTAL ();
};
struct _static_use_CRYSTAL { _static_use_CRYSTAL (); };
_Bind_bootstrap_static_plugins_CRYSTAL::_Bind_bootstrap_static_plugins_CRYSTAL () {}
// Needed to pull in _cs_static_use object file
namespace { _static_use_CRYSTAL _static_use_CRYSTAL_bind; }

