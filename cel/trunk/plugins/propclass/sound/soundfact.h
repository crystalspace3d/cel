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
#include "iutil/comp.h"
#include "csutil/scf.h"
#include "csutil/csstring.h"
#include "physicallayer/propclas.h"
#include "physicallayer/propfact.h"
#include "physicallayer/facttmpl.h"
#include "celtool/stdpcimp.h"
#include "celtool/stdparams.h"
#include "propclass/sound.h"
#include "isound/source.h"
#include "isound/listener.h"
#include "isound/renderer.h"
#include "isound/wrapper.h"

struct iCelEntity;
struct iObjectRegistry;

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
  // For PerformAction.
  static csStringID action_setdirection;
  static csStringID id_front;
  static csStringID id_top;

  // For properties.
  enum propids
  {
    propid_front = 0,
    propid_top,
    propid_position,
    propid_velocity,
    propid_distancefactor,
    propid_rollofffactor,
    propid_dopplerfactor,
    propid_headsize,
    propid_environment
  };
  static Property* properties;
  static size_t propertycount;
  static void UpdateProperties (iObjectRegistry* object_reg);

  csRef<iSoundRender> renderer;
  csRef<iSoundListener> listener;

public:
  celPcSoundListener (iObjectRegistry* object_reg);
  virtual ~celPcSoundListener ();

  virtual iSoundListener* GetSoundListener () { return listener; }
  virtual void SetEnvironment (const char* env);
  virtual const char* GetEnvironment ();

  virtual const char* GetName () const { return "pcsoundlistener"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params);

  // Override SetProperty from celPcCommon in order to provide support
  // for the 'max' property.
  virtual bool SetProperty (csStringID, const csVector3&);
  virtual bool GetPropertyVector (csStringID, csVector3&);
  virtual bool SetProperty (csStringID, float);
  virtual float GetPropertyFloat (csStringID);
  virtual bool SetProperty (csStringID, const char*);
  virtual const char* GetPropertyString (csStringID);
};

/**
 * This is a sound source property class.
 */
class celPcSoundSource : public scfImplementationExt1<
	celPcSoundSource, celPcCommon, iPcSoundSource>
{
private:
  // For PerformAction.
  static csStringID action_play;
  static csStringID id_method;
  static csStringID action_stop;

  // For properties.
  enum propids
  {
    propid_soundname = 0,
    propid_volume,
    propid_frequencyfactor,
    propid_mode3d,
    propid_position,
    propid_velocity,
    propid_minimumdistance,
    propid_maximumdistance
  };
  static Property* properties;
  static size_t propertycount;
  static void UpdateProperties (iObjectRegistry* object_reg);

  csRef<iSoundSource> source;
  csString soundname;
  csRef<iSoundWrapper> soundwrap;
  void GetSoundWrap ();
  bool GetSource ();

public:
  celPcSoundSource (iObjectRegistry* object_reg);
  virtual ~celPcSoundSource ();

  virtual iSoundSource* GetSoundSource () { return source; }
  virtual void SetSoundName (const char* name);
  virtual const char* GetSoundName () const { return soundname; }

  virtual const char* GetName () const { return "pcsoundsource"; }
  virtual csPtr<iCelDataBuffer> Save ();
  virtual bool Load (iCelDataBuffer* databuf);
  virtual bool PerformAction (csStringID actionId, iCelParameterBlock* params);

  // Override SetProperty from celPcCommon in order to provide support
  // for the 'max' property.
  virtual bool SetProperty (csStringID, const csVector3&);
  virtual bool GetPropertyVector (csStringID, csVector3&);
  virtual bool SetProperty (csStringID, float);
  virtual float GetPropertyFloat (csStringID);
  virtual bool SetProperty (csStringID, long);
  virtual long GetPropertyLong (csStringID);
  virtual bool SetProperty (csStringID, const char*);
  virtual const char* GetPropertyString (csStringID);
};

#endif // __CEL_PF_SOUNDFACT__

