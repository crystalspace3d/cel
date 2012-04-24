/*
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

#ifndef __ACTORSETTINGS_H__
#define __ACTORSETTINGS_H__

#include <stdarg.h>

// CEL Includes
#include "physicallayer/pl.h"
#include "physicallayer/entity.h"
#include "physicallayer/messaging.h"

class ActorSettings;

/**
 * Listen to messages.
 */
class CelTestMessageReceiver : public scfImplementation1<CelTestMessageReceiver,
  	iMessageReceiver>
{
private:
  ActorSettings* as;

public:
  CelTestMessageReceiver (ActorSettings* as) :
    scfImplementationType (this), as (as)
  { }
  virtual ~CelTestMessageReceiver () { }
  virtual bool ReceiveMessage (csStringID msg_id, iMessageSender* sender,
      celData& ret, iCelParameterBlock* params);
};

/**
 * Class to control settings for the actor.
 */
class ActorSettings
{
private:
  csRef<iCelPlLayer> pl;
  csRef<iCelEntity> setting_bar;
  csRef<iCelEntity> entity_cam;

  csRef<CelTestMessageReceiver> receiver;
  csStringID id_toggle_setting_bar;
  csStringID id_next_setting;
  csStringID id_prev_setting;
  csStringID id_decrease_setting;
  csStringID id_increase_setting;
  csStringID id_decrease_setting_slow;
  csStringID id_increase_setting_slow;
  csStringID id_decrease_setting_fast;
  csStringID id_increase_setting_fast;
  csStringID id_next_setting_repeat;
  csStringID id_prev_setting_repeat;
  csStringID id_decrease_setting_repeat;
  csStringID id_increase_setting_repeat;
  csStringID id_decrease_setting_slow_repeat;
  csStringID id_increase_setting_slow_repeat;
  csStringID id_decrease_setting_fast_repeat;
  csStringID id_increase_setting_fast_repeat;
  int current_setting;
  void UpdateSetting ();
  void ChangeSetting (float dir);
 
public:
  ActorSettings () { }
  ~ActorSettings () { }

  void Initialize (iCelPlLayer* pl);
  bool ReceiveMessage (csStringID msg_id, iCelParameterBlock* params);
};

#endif // __CELTEST_H__

