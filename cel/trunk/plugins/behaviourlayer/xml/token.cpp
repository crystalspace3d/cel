/*
    Crystal Space Entity Layer
    Copyright (C) 2003 by Jorrit Tyberghein

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

#include "cssysdef.h"
#include "csutil/objreg.h"
#include "plugins/behaviourlayer/xml/token.h"

const char* celXmlSkipWhiteSpace (const char* input)
{
  while (isspace (*input)) input++;
  return input;
}

const char* celXmlParseTokenNumeric (const char* input, int& token)
{
  token = CEL_TOKEN_INT32;
  input = celXmlSkipWhiteSpace (input);
  while (isdigit (*input)) input++;
  if (*input == '.')
  {
    token = CEL_TOKEN_FLOAT;
    input++;
    while (isdigit (*input)) input++;
  }
  if (*input == 'e' || *input == 'E')
  {
    token = CEL_TOKEN_FLOAT;
    input++;
    if (*input == '-' || *input == '+') input++;
    while (isdigit (*input)) input++;
  }
  if (token == CEL_TOKEN_INT32 && *input == 'u')
  {
    token = CEL_TOKEN_UINT32;
    input++;
  }
  return input;
}

const char* celXmlParseToken (const char* input, int& token)
{
  input = celXmlSkipWhiteSpace (input);
  switch (*input)
  {
    case 0: token = CEL_TOKEN_END; return input;
    case '?': token = CEL_TOKEN_DEREFVAR; return input+1;
    case '\'': token = CEL_TOKEN_STRINGLIT; return input+1;
    case '[': token = CEL_TOKEN_VECTOR; return input+1;
    case ']': token = CEL_TOKEN_VECTORCLOSE; return input+1;
    case '(': token = CEL_TOKEN_OPEN; return input+1;
    case ')': token = CEL_TOKEN_CLOSE; return input+1;
    case ',': token = CEL_TOKEN_COMMA; return input+1;
    case '*': token = CEL_TOKEN_MULT; return input+1;
    case '/': token = CEL_TOKEN_DIV; return input+1;
    case '-': token = CEL_TOKEN_MINUS; return input+1;
    case '+': token = CEL_TOKEN_ADD; return input+1;
    default:
      if (!strncmp ("false", input, 5))
      {
        token = CEL_TOKEN_BOOLFALSE;
	return input+5;
      }
      if (!strncmp ("true", input, 4))
      {
        token = CEL_TOKEN_BOOLTRUE;
	return input+4;
      }
      if (!strncmp ("rgb(", input, 4))
      {
        token = CEL_TOKEN_COLOR;
	return input+4;
      }
      if (!strncmp ("pc(", input, 3))
      {
        token = CEL_TOKEN_PC;
	return input+3;
      }
      if (!strncmp ("entity(", input, 7))
      {
        token = CEL_TOKEN_ENTITY;
	return input+7;
      }
      if (isdigit (*input) || *input == '.')
        return celXmlParseTokenNumeric (input, token);
      token = CEL_TOKEN_ERROR;
      return input;
  }
}

const char* celXmlParseString (const char* input, char*& str)
{
  const char* i = input;
  int len = 0;
  while (*i)
  {
    if (*i == '\'' && *(i+1) == '\'') { len++; i += 2; }
    else if (*i == '\'') break;
    else { len++; i++; }
  }
  if (*i != '\'') { str = 0; return i; }
  str = new char[len+1];
  char* s = str;
  while (*input)
  {
    if (*input == '\'' && *(input+1) == '\'') { *s++ = '\''; input += 2; }
    else if (*input == '\'') { input++; break; }
    else { *s++ = *input++; }
  }
  *s = 0;
  return input;
}


//---------------------------------------------------------------------------

