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

#define CEL_TOKEN_ERROR -1
#define CEL_TOKEN_END 0			// End of string
#define CEL_TOKEN_DEREFVAR 1		// ?expr
#define CEL_TOKEN_STRINGLIT 2		// 'xxx...'
#define CEL_TOKEN_VECTOR 3		// [expr,expr] or [expr,expr,expr]
#define CEL_TOKEN_BOOLTRUE 4		// true
#define CEL_TOKEN_BOOLFALSE 5		// false
#define CEL_TOKEN_COLOR 6		// rgb(expr,expr,expr)
#define CEL_TOKEN_INT32 7		// 348 or -398
#define CEL_TOKEN_UINT32 8		// 348u
#define CEL_TOKEN_FLOAT 9		// 342.33 or -2.33e-33
#define CEL_TOKEN_OPEN 10		// (
#define CEL_TOKEN_CLOSE 11		// )
#define CEL_TOKEN_MINUS 12		// -
#define CEL_TOKEN_ADD 13		// +
#define CEL_TOKEN_MULT 14		// *
#define CEL_TOKEN_DIV 15		// /
#define CEL_TOKEN_COMMA 16		// ,
#define CEL_TOKEN_VECTORCLOSE 17	// ]
#define CEL_TOKEN_ENTITY 18		// entity(expr)
#define CEL_TOKEN_PC 19			// pc(expr,expr)

#define CEL_PRIORITY_ONETERM -1 
#define CEL_PRIORITY_NORMAL 0
#define CEL_PRIORITY_ADD 1
#define CEL_PRIORITY_MULT 2

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

