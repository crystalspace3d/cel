/*
    Crystal Space Entity Layer
    Copyright (C) 2001 by Jorrit Tyberghein
  
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

#ifndef __CEL_BL_BEHAVE__
#define __CEL_BL_BEHAVE__

#include <stdarg.h>
#include "cstypes.h"
#include "csutil/scf.h"

struct iCelBlLayer;

SCF_VERSION (iCelBehaviour, 0, 0, 1);

/**
 * This is an entity in the CEL layer at the BL (behaviour layer) side.
 */
struct iCelBehaviour : public iBase
{
  /**
   * Get the name of this behaviour (was used in the call to
   * iCelBlLayer::CreateBehaviour()).
   */
  virtual const char* GetName () const = 0;

  /**
   * Get the behaviour layer that created this behaviour.
   */
  virtual iCelBlLayer* GetBehaviourLayer () const = 0;

  /**
   * Send a message to this entity. Returns true if the
   * message was understood and handled by the entity.
   */
  virtual bool SendMessage (const char* msg_id, iBase* msg_info, ...) = 0;

  /**
   * Send a message to this entity. Returns true if the
   * message was understood and handled by the entity.
   */
  virtual bool SendMessageV (const char* msg_id, iBase* msg_info, va_list arg) = 0;
};

#endif // __CEL_BL_BEHAVE__

