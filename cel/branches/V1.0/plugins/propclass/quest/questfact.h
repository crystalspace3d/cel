/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein
  
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

#ifndef __CEL_PF_QUESTFACT__
#define __CEL_PF_QUESTFACT__

#include "cstypes.h"
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "csutil/csstring.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/quest.h"
#include "tools/questmanager.h"

struct iCelEntity;
struct iObjectRegistry;

/**
 * Factory for quest.
 */
CEL_DECLARE_FACTORY (Quest)

/**
 * This is a quest property class.
 */
class celPcQuest : public celPcCommon
{
private:
  // For SendMessage parameters.
  //static csStringID id_message;
  //celOneParameterBlock* params;

  // For actions.
  // id_message is shared.
  static csStringID id_name;

  enum actionids
  {
    action_newquest = 0,
    action_stopquest
  };

  // For properties.
  enum propids
  {
    propid_name = 0,
    propid_state
  };
  static PropertyHolder propinfo;

  // Other fields.
  csRef<iQuestManager> quest_mgr;
  void GetQuestManager ();

  celQuestParams quest_params;
  csString questname;
  csRef<iQuest> quest;

public:
  celPcQuest (iObjectRegistry* object_reg);
  virtual ~celPcQuest ();

  bool NewQuest (const char* name, celQuestParams& params);
  void StopQuest ();
  iQuest* GetQuest () const { return quest; }
  const char* GetQuestName () const { return questname; }

  SCF_DECLARE_IBASE_EXT (celPcCommon);

  virtual const char* GetName () const { return "pcquest"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformActionIndexed (int idx, iCelParameterBlock* params,
      celData& ret);

  virtual bool SetPropertyIndexed (int, const char*);
  virtual bool GetPropertyIndexed (int, const char*&);

  struct PcQuest : public iPcQuest
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPcQuest);
    virtual bool NewQuest (const char* name, celQuestParams& params)
    {
      return scfParent->NewQuest (name, params);
    }
    virtual void StopQuest ()
    {
      scfParent->StopQuest ();
    }
    virtual iQuest* GetQuest () const
    {
      return scfParent->GetQuest ();
    }
    virtual const char* GetQuestName () const
    {
      return scfParent->GetQuestName ();
    }
  } scfiPcQuest;
};

#endif // __CEL_PF_QUESTFACT__

