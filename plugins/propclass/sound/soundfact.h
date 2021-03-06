/*
    Crystal Space Entity Layer
    Copyright (C) 2005 by Jorrit Tyberghein

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

#ifndef __CEL_PF_SOUNDFACT__
#define __CEL_PF_SOUNDFACT__

#include "cstypes.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/sound.h"
#include "propclass/camera.h"
#include "isndsys/ss_source.h"
#include "isndsys/ss_listener.h"
#include "isndsys/ss_renderer.h"
#include "isndsys/ss_manager.h"

struct iCelEntity;
struct iObjectRegistry;
class celSoundSourceMovableListener;
/**
 * Factory for sound.
 */
CEL_DECLARE_FACTORY (SoundListener)
CEL_DECLARE_FACTORY (SoundSource)

/**
 * This is a sound listener property class.
 */
class celPcSoundListener : public scfImplementationExt1<
	celPcSoundListener, celPcCommon, iPcSoundListener>
{
private:
  // For actions.
  static csStringID id_front;
  static csStringID id_top;

  enum actionids
  {
    action_setdirection = 0
  };

  // For properties.
  enum propids
  {
    propid_front = 0,
    propid_top,
    propid_position,
    propid_distancefactor,
    propid_rollofffactor
  };
  static PropertyHolder propinfo;

  csRef<iSndSysRenderer> renderer;
  csRef<iSndSysListener> listener;
  csWeakRef<iPcCamera> pccamera;

public:
  celPcSoundListener (iObjectRegistry* object_reg);
  virtual ~celPcSoundListener ();

  virtual iSndSysListener* GetSoundListener () { return listener; }

  virtual bool PerformActionIndexed (int idx, iCelParameterBlock* params,
      celData& ret);

  virtual bool SetPropertyIndexed (int, const csVector3&);
  virtual bool GetPropertyIndexed (int, csVector3&);
  virtual bool SetPropertyIndexed (int, float);
  virtual bool GetPropertyIndexed (int, float&);
  virtual void PropertyClassesHaveChanged();
  virtual void TickEveryFrame ();
};

/**
 * This is a sound source property class.
 */
class celPcSoundSource : public scfImplementationExt1<
	celPcSoundSource, celPcCommon, iPcSoundSource>
{
private:
  // For actions.
  enum actionids
  {
    action_pause = 0,
    action_unpause,
    action_stop,
    action_play
  };

  // For properties.
  enum propids
  {
    propid_soundname = 0,
    propid_volume,
    propid_directionalradiation,
    propid_position,
    propid_minimumdistance,
    propid_maximumdistance,
    propid_loop,
    propid_follow,
    propid_mode
  };
  static PropertyHolder propinfo;

  csWeakRef<iSndSysSource3D> source3d;
  csWeakRef<iSndSysSource> source;
  csRef<iSndSysStream> stream;
  int mode;
  csString soundname;
  csRef<iSndSysWrapper> soundwrap;
  void GetSoundWrap ();
  bool GetSource ();

  bool follow;
  // Movable listener so we can update the sound source.
  csWeakRef<iMovable> movable_for_listener;
  csRef<celSoundSourceMovableListener> movlistener;
  void UpdateListener ();
  void CheckPropertyClasses ();

public:
  celPcSoundSource (iObjectRegistry* object_reg);
  virtual ~celPcSoundSource ();

  virtual iSndSysSource* GetSoundSource () {
    GetSource(); return source;
  }
  virtual void SetSoundName (const char* name);
  virtual const char* GetSoundName () const { return soundname; }
  virtual void SetMode (const char* modename);
  virtual const char* GetMode () const;
  virtual void Play ();
  virtual void Stop ();
  virtual void Pause ();
  virtual void Unpause ();

  virtual bool PerformActionIndexed (int idx, iCelParameterBlock* params,
      celData& ret);

  virtual bool SetPropertyIndexed (int, const csVector3&);
  virtual bool GetPropertyIndexed (int, csVector3&);
  virtual bool SetPropertyIndexed (int, float);
  virtual bool GetPropertyIndexed (int, float&);
  virtual bool SetPropertyIndexed (int, bool);
  virtual bool GetPropertyIndexed (int, bool&);
  virtual bool SetPropertyIndexed (int, const char*);
  virtual bool GetPropertyIndexed (int, const char*&);

  virtual void TickEveryFrame ();

  virtual void Activate ();
  virtual void Deactivate ();
};

#endif // __CEL_PF_SOUNDFACT__

