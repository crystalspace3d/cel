#include "cssysdef.h"
#include "physicallayer/datatype.h"
#include "physicallayer/propclas.h"
#include "physicallayer/entity.h"

csString celData::GetDebugInfo ()
{
  char buffer[128];
  switch (type)
  {
  case CEL_DATA_NONE:
    sprintf (buffer, "\tData not typed!");
    break;
  case CEL_DATA_BOOL:
    sprintf (buffer, "\tBool: %d", value.bo);
    break;
  case CEL_DATA_BYTE:
    sprintf (buffer, "\tByte: %d", value.b);
    break;
  case CEL_DATA_WORD:
    sprintf (buffer, "\tWord: %d", value.w);
    break;
  case CEL_DATA_LONG:
    sprintf (buffer, "\tLong: %d", value.l);
    break;
  case CEL_DATA_UBYTE:
    sprintf (buffer, "\tuByte: %d", value.ub);
    break;
  case CEL_DATA_UWORD:
    sprintf (buffer, "\tuWord: %d", value.uw);
    break;
  case CEL_DATA_ULONG:
    sprintf (buffer, "\tuLong: %d", value.ul);
    break;
  case CEL_DATA_FLOAT:
    sprintf (buffer, "\tFloat: %f", value.f);
    break;
  case CEL_DATA_VECTOR2:
    sprintf (buffer, "\tVector2: %f %f", value.v.x, value.v.y);
    break;
  case CEL_DATA_VECTOR3:
    sprintf (buffer, "\tVector3: %f %f %f", value.v.x, value.v.y, value.v.z);
    break;
  case CEL_DATA_COLOR:
    sprintf (buffer, "\tColor: %f %f %f", value.col.red, value.col.green, value.col.blue);
    break;
  case CEL_DATA_STRING:
    sprintf (buffer, "\tString: %s", value.s->GetData ());
    break;
  case CEL_DATA_PCLASS:
    sprintf (buffer, "\tProperty Class: %s %s", value.pc->GetName (), value.pc->GetTag ());
    break;
  case CEL_DATA_ENTITY:
    sprintf (buffer, "\tEntity: id %d", value.ent->GetID ());
    break;
  case CEL_DATA_ACTION:
    sprintf (buffer, "\tAction: %s", value.s->GetData ());
    break;
  case CEL_DATA_IBASE:
    sprintf (buffer, "\tiBase");
    break;
  case CEL_DATA_PARAMETER:
    sprintf (buffer, "\tParameter: name %s type %d", value.par.parname->GetData (), value.par.partype);
    break;
  default:
    sprintf (buffer, "\tError: not a type!");
    break;
  }
  return buffer;
}
