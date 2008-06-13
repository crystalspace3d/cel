/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Christian Van Brussel

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

#ifndef __NETWORK_TYPES_H__
#define __NETWORK_TYPES_H__

// client events
enum
{
  CLIENT_EVENT_SIMPLE_TEST = 1,
  CLIENT_EVENT_COMPLEX_TEST,
  CLIENT_EVENT_FORWARD_TEST,
  CLIENT_EVENT_KICKING_TEST,  
  CLIENT_EVENT_KICKING_ALL_TEST  
};

// server events
enum
{
  SERVER_EVENT_FORWARDED_TEST = 1,
  SERVER_EVENT_MESSAGE
};

// network links
enum
{
  NETWORK_LINK_ACTOR = 1
};

#endif // __NETWORK_TYPES_H__
