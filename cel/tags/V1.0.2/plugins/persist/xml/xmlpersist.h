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

#ifndef __CEL_PERSIST_XML__
#define __CEL_PERSIST_XML__

#include "csutil/hash.h"
#include "csutil/refarr.h"
#include "csutil/set.h"
#include "csutil/weakref.h"
#include "iutil/comp.h"
#include "physicallayer/persist.h"
#include "physicallayer/datatype.h"

class celPersistXML;
struct iFile;
struct iVFS;
struct iDocumentNode;
struct iObjectRegistry;
struct iCelPropertyClass;
struct iCelEntity;
struct iCelDataBuffer;
struct iCelPlLayer;
struct iCelBlLayer;
struct celData;

/**
 * This is the XML persistence layer.
 */
class celPersistXML : public iCelPersistence
{
private:
  bool Write (iDocumentNode* node, iCelDataBuffer* db);
  bool Write (iDocumentNode* node, celData* data);
  bool Write (iDocumentNode* pcnode, iCelPropertyClass* pc, bool savelocal);
  bool Write (iDocumentNode* entnode, iCelEntity* entity, bool savelocal);
  bool WriteFirstPass (iDocumentNode* entnode, iCelEntity* entity);

  bool Read (iDocumentNode* node, celData* cd);
  bool Read (iDocumentNode* node, csRef<iCelDataBuffer>& db);
  bool Read (iDocumentNode* pcnode, iCelEntity* entity, iCelPropertyClass*& pc);
  bool Read (iDocumentNode* entnode, iCelEntity*& entity);
  bool ReadFirstPass (iDocumentNode* entnode, iCelEntity* entity);

  bool Report (const char* msg, ...);

public:
  SCF_DECLARE_IBASE;
  csRef<iVFS> vfs;
  iCelLocalEntitySet* set;
  csWeakRef<iCelPlLayer> pl;
  csHash<size_t, csPtrKey<iCelEntity> > entities_map;
  
  celPersistXML (iBase* parent);
  virtual ~celPersistXML ();
  bool Initialize (iObjectRegistry* object_reg);

  virtual bool Load (iCelLocalEntitySet* set, iFile* file);
  virtual bool Load (iCelLocalEntitySet* set, const char* name);
  virtual bool Save (iCelLocalEntitySet* set, iFile* file);
  virtual bool Save (iCelLocalEntitySet* set, const char* name);

  struct Component : public iComponent
  {
    SCF_DECLARE_EMBEDDED_IBASE (celPersistXML);
    virtual bool Initialize (iObjectRegistry* p)
    { return scfParent->Initialize (p); }
  } scfiComponent;
protected:
  iObjectRegistry* object_reg;
};

#endif // __CEL_PERSIST_XML__

