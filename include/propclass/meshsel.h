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

#ifndef __CEL_PF_MESHSEL__
#define __CEL_PF_MESHSEL__

#include "cstypes.h"
#include "csutil/scf.h"

struct iPcCamera;
struct iPcMovable;
struct iPcMesh;
struct iPcMeshSelect;
class csVector3;

#define CEL_MOUSE_BUTTON1 1
#define CEL_MOUSE_BUTTON2 2
#define CEL_MOUSE_BUTTON3 4

/**
 * Listen to mesh selection.
 */
struct iPcMeshSelectListener : public virtual iBase
{
  SCF_INTERFACE (iPcMeshSelectListener, 0, 0, 1);

  /**
   * Mouse button goes down on mesh.
   */
  virtual void MouseDown (iPcMeshSelect* meshsel,
  	int x, int y, int button, iCelEntity* entity) = 0;

  /**
   * Mouse button goes up on mesh.
   */
  virtual void MouseUp (iPcMeshSelect* meshsel,
  	int x, int y, int button, iCelEntity* entity) = 0;

  /**
   * Mouse moves while mesh is selected.
   */
  virtual void MouseMove (iPcMeshSelect* meshsel,
  	int x, int y, int button, iCelEntity* entity) = 0;
};

/**
 * This is a property class for selecting meshes.
 *
 * This property class can send out the following messages
 * to the behaviour (add prefix 'cel.parameter.' to get the ID for parameters):
 * - pcmeshsel_down: mesh is selected (x,y,button,entity)
 * - pcmeshsel_up: mesh is unselected (x,y,button,entity)
 * - pcmeshsel_move: mesh is moved (x,y,button,entity)
 *
 * This property class supports the following actions (add prefix
 * 'cel.action.' to get the ID of the action and add prefix 'cel.parameter.'
 * to get the ID of the parameter):
 * - SetCamera: parameters 'entity' (string).
 * - SetMouseButtons: parameters 'buttons' (string or long). In case
 *     of string it should be a string with 'l', 'm', or 'r' (or a combination).
 * - SetDragPlaneNormal: parameters 'normal' (vector3), 'camera' (bool).
 *
 * This property class supports the following properties (add prefix
 * 'cel.property.' to get the ID of the property:
 * - global (bool, read/write): global selection, default false.
 * - follow (bool, read/write): follow mode, default false.
 * - followalways (bool, read/write): follow always mode, default false.
 * - drag (bool, read/write): drag mode, default false.
 * - sendmove (bool, read/write): send move events, default false.
 * - sendup (bool, read/write): send up events, default true.
 * - senddown (bool, read/write): send down events, default true.
 * - maxdistance (float, read/write): maximum selection distance,
 *     default 100000.
 */
struct iPcMeshSelect : public virtual iBase
{
  SCF_INTERFACE (iPcMeshSelect, 0, 0, 2);

  /**
   * Add a listener to listen to mouse selection. These listeners are called
   * right before the behaviour is called.
   */
  virtual void AddMeshSelectListener (iPcMeshSelectListener* listener) = 0;
  /**
   * Remove a mesh selection listener.
   */
  virtual void RemoveMeshSelectListener (iPcMeshSelectListener* listener) = 0;

  /**
   * Set the camera to use for mesh selection.
   */
  virtual void SetCamera (iPcCamera* camera) = 0;

  /**
   * Indicate (with a bit-mask) for which buttons this
   * mesh selector will work. By default this is the left button.
   * Use a combination of CEL_MOUSE_... flags.
   */
  virtual void SetMouseButtons (int buttons) = 0;

  /**
   * Indicate (with a string) for which buttons this
   * mesh selector will work. By default this is the left button.
   * Use a combination of the 'l', 'r', or 'm' characters.
   */
  virtual void SetMouseButtons (const char* buttons) = 0;
  /**
   * Get the bit-mask for the supported buttons.
   */
  virtual int GetMouseButtons () const = 0;

  /**
   * Set global/local selection. With local selection
   * the Behaviour will only get messages when it's
   * own mesh entity is selected (default). With global
   * selection the Behaviour will get messages for selection
   * of all mesh entities.
   */
  virtual void SetGlobalSelection (bool glob) = 0;
  /**
   * Get value of global/local selection.
   */
  virtual bool HasGlobalSelection () const = 0;

  /**
   * If true then follow the mouse so that up/down events
   * come through immediatelly when the mouse leaves the object
   * (even if mouse up has not yet happened). Otherwise mouse
   * up event will arrive only when the mouse is released
   * independent of wether or not the mouse is above the
   * object at release time. Default false.
   */
  virtual void SetFollowMode (bool follow) = 0;
  /**
   * Get the value of follow mode.
   */
  virtual bool HasFollowMode () const = 0;

  /**
   * If true then also send move messages when
   * the entity is not selected but the mouse is positioned
   * on top of the entity. Default false.
   */
  virtual void SetFollowAlwaysMode (bool followalways) = 0;
  /**
   * Get the value of follow always mode.
   */
  virtual bool HasFollowAlwaysMode () const = 0;

  /**
   * If true then it is possible to drag the selected object.
   * Note that drag mode is usually used in combination with
   * follow mode.
   * (default false).
   */
  virtual void SetDragMode (bool drag) = 0;
  /**
   * Get value of drag mode.
   */
  virtual bool HasDragMode () const = 0;

  /**
   * Set the normal of the plane on which the dragged object
   * will move. The actual plane itself will be calculated
   * depending on the current position of the object and that
   * given normal. The normal can be given in camera space
   * or in world space. Default is the 0,0,1 normal in camera
   * space which means that the object will drag according
   * to screen coordinates.
   */
  virtual void SetDragPlaneNormal (const csVector3& drag_normal,
  	bool camera_space) = 0;
  /**
   * Get the drag normal and camera space flag.
   */
  virtual void GetDragPlaneNormal (csVector3& drag_normal,
  	bool& camera_space) const = 0;

  /**
   * If true then also send messages on mouse move (only
   * between mouse-down and mouse-up (message 'selectmesh_move').
   * Default false.
   */
  virtual void SetSendmoveEvent (bool mov) = 0;
  /**
   * Get the value of send move events.
   */
  virtual bool HasSendmoveEvent () const = 0;

  /**
   * If true then send mouse-up events (message 'selectmesh_up').
   * Default true.
   */
  virtual void SetSendupEvent (bool su) = 0;
  /**
   * Get the value of send up events.
   */
  virtual bool HasSendupEvent () const = 0;

  /**
   * If true then send mouse-down events (message 'selectmesh_down').
   * Default true.
   */
  virtual void SetSenddownEvent (bool sd) = 0;
  /**
   * Get the value of send down events.
   */
  virtual bool HasSenddownEvent () const = 0;

  /**
   * Set the maximum distance to use for mesh selection.
   * This is the distance in world units from the camera, into the scene.
   */
  virtual void SetMaxSelectionDistance (float distance) = 0;

  /**
   * Get the maximum distance to use for mesh selection.
   */
  virtual float GetMaxSelectionDistance () const = 0;
};

#endif // __CEL_PF_MESHSEL__

