/*
    Copyright (C) 2005 by Jorrit Tyberghein

    This file is part of CEL.

    CEL is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    CEL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CEL; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "cssysdef.h"
#include "behave.h"

// CS Includes
#include "csutil/ref.h"

// CEL Includes
#include "propclass/mechsys.h"
#include "physicallayer/propclas.h"

SCF_IMPLEMENT_IBASE (htBehaviourActor)
  SCF_IMPLEMENTS_INTERFACE (iCelBehaviour)
SCF_IMPLEMENT_IBASE_END

htBehaviourActor::htBehaviourActor(iCelBlLayer* bl, iCelEntity* entity) : bl (bl), entity (entity)
{
  SCF_CONSTRUCT_IBASE (0);
}
htBehaviourActor::~htBehaviourActor()
{
  SCF_DESTRUCT_IBASE ();
}

bool htBehaviourActor::SendMessage (const char *msg_id, iCelPropertyClass *pc, celData &ret, iCelParameterBlock *params,...)
{
  va_list arg;
  va_start (arg, params);
  bool rc = SendMessageV (msg_id, pc, ret, params, arg);
  va_end (arg);
  return rc;
}
bool htBehaviourActor::SendMessageV (const char *msg_id, iCelPropertyClass *pc, celData &ret, iCelParameterBlock *params, va_list arg)
{
  // OpenSoundControl message was sent.
  if (strncmp (msg_id, "pcosc_", 6) == 0)
  {
    printf ("\n%s\n", msg_id);
    const celData* d = params->GetParameterByIndex (0);
    if (d->type != CEL_DATA_STRING)
      return false;   // Shouldn't happen!
    printf ("path: <%s>\n", d->value.s->GetData ());
    for (size_t i = 1 ; i < params->GetParameterCount () ; i++)
    {
      d = params->GetParameterByIndex (i);
      printf ("type (%d) ", d->type);
      switch (d->type)
      {
        case (CEL_DATA_LONG):
          printf ("value (%d) (long)", d->value.l);
          break;
        case (CEL_DATA_BOOL):
          printf ("value (%c) (bool)", d->value.bo);
          break;
        case (CEL_DATA_BYTE):
          printf ("value (%c) (byte)", d->value.b);
          break;
        case (CEL_DATA_STRING):
          printf ("value (%s) (string)", d->value.s->GetData ());
          break;
        case (CEL_DATA_FLOAT):
          printf ("value (%f) (float)", d->value.f);
          break;
        case (CEL_DATA_NONE):
        case (CEL_DATA_WORD):
        //case (CEL_DATA_LONG):
        case (CEL_DATA_UBYTE):
        case (CEL_DATA_UWORD):
        case (CEL_DATA_ULONG):
        case (CEL_DATA_VECTOR2):
        case (CEL_DATA_VECTOR3):
        case (CEL_DATA_PCLASS):
        case (CEL_DATA_ENTITY):
        case (CEL_DATA_ACTION):
        case (CEL_DATA_COLOR):
        case (CEL_DATA_IBASE):
        case (CEL_DATA_PARAMETER):
        case (CEL_DATA_LAST):
        default:
          puts ("(unhandled)");
          break;
      }
      puts ("");
    }
  }

  bool pcinput_msg = strncmp (msg_id, "pccommandinput_", 15) == 0;

  if (pcinput_msg)
  {
    if (!strcmp (msg_id+15, "up1"))
    {
      GetCraft ();
      pccraft->StartTurnUp();
    }
    else if (!strcmp (msg_id+15, "up0"))
    {
      GetCraft ();
      pccraft->StopTurnUp();
    }

    else if (!strcmp (msg_id+15, "down1"))
    {
      GetCraft ();
      pccraft->StartTurnDown();
    }
    else if (!strcmp (msg_id+15, "down0"))
    {
      GetCraft ();
      pccraft->StopTurnDown();
    }

    else if (!strcmp (msg_id+15, "left1"))
    {
      GetCraft ();
      pccraft->StartTurnLeft();
    }
    else if (!strcmp (msg_id+15, "left0"))
    {
      GetCraft ();
      pccraft->StopTurnLeft();
    }

    else if (!strcmp (msg_id+15, "right1"))
    {
      GetCraft ();
      pccraft->StartTurnRight();
    }
    else if (!strcmp (msg_id+15, "right0"))
    {
      GetCraft ();
      pccraft->StopTurnRight();
    }

    if (!strcmp (msg_id+15, "thrust1"))
    {
      GetCraft ();
      pccraft->ThrustOn();
    }
    else if (!strcmp (msg_id+15, "thrust0"))
    {
      GetCraft ();
      pccraft->ThrustOff();
    }

    else if (!strcmp (msg_id+15, "jump1"))
    {
      GetMechObject ();
      pcmechobj->AddForceDuration (csVector3 (0, 25.0f, 0), false,
      	csVector3 (0, 0, 0), .2f);
    }

    else if (!strcmp (msg_id+15, "lookup1"))
    {
      GetCamera ();
      pcdefcamera->SetPitchVelocity (1.0f);
    }
    else if (!strcmp (msg_id+15, "lookup0"))
    {
      GetCamera ();
      pcdefcamera->SetPitchVelocity (0.0f);
    }
    else if (!strcmp (msg_id+15, "lookdown1"))
    {
      GetCamera ();
      pcdefcamera->SetPitchVelocity (-1.0f);
    }
    else if (!strcmp (msg_id+15, "lookdown0"))
    {
      GetCamera ();
      pcdefcamera->SetPitchVelocity (0.0f);
    }
    else if (!strcmp (msg_id+15, "center1"))
    {
      GetCamera ();
      pcdefcamera->CenterCamera ();
    }
    return true;
  }

  return false;
}

iPcMechanicsObject* htBehaviourActor::GetMechObject ()
{
  if (!pcmechobj)
  {
    pcmechobj = CEL_QUERY_PROPCLASS_ENT (entity, iPcMechanicsObject);
    CS_ASSERT (pcmechobj != 0);
  }
  return pcmechobj;
}

iPcCraftController* htBehaviourActor::GetCraft ()
{
  if (!pccraft)
  {
    pccraft = CEL_QUERY_PROPCLASS_ENT (entity, iPcCraftController);
    CS_ASSERT (pccraft != 0);
  }
  return pccraft;
}

iPcDefaultCamera* htBehaviourActor::GetCamera ()
{
  if (!pcdefcamera)
  {
    pcdefcamera = CEL_QUERY_PROPCLASS_ENT (entity, iPcDefaultCamera);
    CS_ASSERT (pcdefcamera != 0);
  }
  return pcdefcamera;
}
