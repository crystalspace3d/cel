%module blcel
%include cs.i
%{
#include "pl/pl.h"
#include "pl/propfact.h"
#include "pl/propclas.h"
#include "pl/entity.h"
#include "pl/persist.h"
#include "bl/bl.h"
#include "bl/behave.h"
%}

struct iCelPlLayer : public iBase
{
  iCelEntity* CreateEntity ();
};

struct iCelEntity : public iBase
{
  const char* GetName () const;
  void SetName (const char* n);
};

