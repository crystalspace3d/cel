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

#ifndef __CEL_BLXML_TOKEN__
#define __CEL_BLXML_TOKEN__

#include "cstypes.h"

enum
{
  CEL_TOKEN_ERROR = -1,
  CEL_TOKEN_END,	// End of string
  CEL_TOKEN_DEREFVAR,	// ?expr
  CEL_TOKEN_STRINGLIT,	// 'xxx...'
  CEL_TOKEN_VECTOR,	// [expr,expr] or [expr,expr,expr]
  CEL_TOKEN_BOOLTRUE,	// true
  CEL_TOKEN_BOOLFALSE,	// false
  CEL_TOKEN_INT32,	// 348 or -398
  CEL_TOKEN_UINT32,	// 348u
  CEL_TOKEN_FLOAT,	// 342.33 or -2.33e-33
  CEL_TOKEN_OPEN,	// (
  CEL_TOKEN_CLOSE,	// )
  CEL_TOKEN_MINUS,	// -
  CEL_TOKEN_ADD,	// +
  CEL_TOKEN_MULT,	// *
  CEL_TOKEN_DIV,	// /
  CEL_TOKEN_EQ,		// ==
  CEL_TOKEN_NE,		// !=
  CEL_TOKEN_LE,		// <=
  CEL_TOKEN_GE,		// >=
  CEL_TOKEN_LT,		// <
  CEL_TOKEN_GT,		// >
  CEL_TOKEN_LOGAND,	// &&
  CEL_TOKEN_LOGOR,	// ||
  CEL_TOKEN_COMMA,	// ,
  CEL_TOKEN_DOT,	// .
  CEL_TOKEN_VECTORCLOSE,// ]
  CEL_TOKEN_IDENTIFIER,	// identifier
  CEL_TOKEN_FUNCTION,	// <identifier>(

  CEL_TOKEN_LAST
};

#define CEL_PRIORITY_ONETERM -1 
#define CEL_PRIORITY_NORMAL 0
#define CEL_PRIORITY_LOGOR 1
#define CEL_PRIORITY_LOGAND 2
#define CEL_PRIORITY_BITOR 3
#define CEL_PRIORITY_BITXOR 4
#define CEL_PRIORITY_BITAND 5
#define CEL_PRIORITY_EQUAL 6
#define CEL_PRIORITY_LTGT 7
#define CEL_PRIORITY_BITSHIFT 8
#define CEL_PRIORITY_ADDSUB 9
#define CEL_PRIORITY_MULTDIV 10

/**
 * Skip white space.
 */
const char* celXmlSkipWhiteSpace (const char* input);

/**
 * Parse a token. Returns the position after the token.
 * 'token' will be set to the token constant (one of CEL_TOKEN_...).
 */
const char* celXmlParseToken (const char* input, int& token);

/**
 * Parse a token as a number (INT32, UINT32, or FLOAT).
 */
const char* celXmlParseTokenNumeric (const char* input, int& token);

/**
 * Parse a string. 'input' should point right after the ' token.
 * Returns position right after ending ' token. Allocates a string
 * in 'str' (delete with delete[] when ready) or 0 in case of error.
 * 
 */
const char* celXmlParseString (const char* input, char*& str);

#endif // __CEL_BLXML_TOKEN__

