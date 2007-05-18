/*
    Crystal Space Entity Layer
    Navigation Tools
    Copyright (C) 2007 by Dariusz Dawidowski

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

#ifndef __CEL_TOOL_NAVIGATION__
#define __CEL_TOOL_NAVIGATION__

#include "cssysdef.h"
#include "csgeom/vector3.h"

struct iSector;
struct iMapNode;
struct iCelEntity;
struct iPcMesh;

/**
 * Navigation result between navigator and target.
 */
struct celNavigationInfo
{
  /// True if navigator and target are found.
  bool success;
  /// True if navigator and target can see eachother.
  bool visible;
  /// Point-point angle between navigator and target.
  csVector3 angle;
  /// Point-Point distance between navigator and target.
  float distance;
};

/**
 * Helplers for navigation.
 */
struct celNavigationTools
{
  /**
   * Returns navigation result between navigator entity and target vector.
   * \param navigator is an entity with pcmesh which looks for a target.
   * \param tag is navigator property class tag or 0.
   * \param vector is a target vector.
   */
  static celNavigationInfo GetNavigationInfo (iCelEntity* navigator,
  	const char* tag, const csVector3& vector);
  /**
   * Returns navigation result between navigator pcmesh and target vector.
   * \param navigator is pcmesh class of the navigator to take
   *        the position from.
   * \param vector is a target vector.
   */
  static celNavigationInfo GetNavigationInfo (iPcMesh* navigator,
  	const csVector3& vector);
  /**
   * Returns navigation result between navigator entity and target entity.
   * \param navigator is an entity with pcmesh which looks for a target.
   * \param navigator_tag is navigator property class tag or 0.
   * \param entity is a target entity with pcmesh.
   * \param entity_tag is target entity property class tag or 0.
   */
  static celNavigationInfo GetNavigationInfo (iCelEntity* navigator,
  	const char* navigator_tag, iCelEntity* entity,
  	const char* entity_tag);
  /**
   * Returns navigation result between navigator pcmesh and target entity.
   * \param navigator is pcmesh class of the navigator to take
   *        the position from.
   * \param entity is a target entity with pcmesh.
   * \param entity_tag is target entity property class tag or 0.
   */
  static celNavigationInfo GetNavigationInfo (iPcMesh* navigator,
  	iCelEntity* entity, const char* entity_tag);
  /**
   * Returns navigation result between navigator entity and target map node.
   * \param navigator is an entity with pcmesh which looks for a target.
   * \param tag is navigator property class tag or 0.
   * \param node is a map node.
   */
  static celNavigationInfo GetNavigationInfo (iCelEntity* navigator,
  	const char* tag, iMapNode* node);
  /**
   * Returns navigation result between navigator pcmesh and target map node.
   * \param navigator is pcmesh class of the navigator to take
   *        the position from.
   * \param node is a map node.
   */
  static celNavigationInfo GetNavigationInfo (iPcMesh* navigator,
  	iMapNode* node);
};

#endif // __CEL_TOOL_NAVIGATION__
