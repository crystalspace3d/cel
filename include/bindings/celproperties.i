
/* iCelConsoleCommand (include/tools/celconsole.h) */
%cs_attribute(blcelc,iCelConsoleCommand,const char*,Command,GetCommand)
%cs_attribute(blcelc,iCelConsoleCommand,const char*,Description,GetDescription)

/* iCelConsole (include/tools/celconsole.h) */
%cs_attribute(blcelc,iCelConsole,iConsoleInput*,InputConsole,GetInputConsole)
%cs_attribute(blcelc,iCelConsole,iConsoleOutput*,OutputConsole,GetOutputConsole)

/* iQuestTriggerType (include/tools/questmanager.h) */
%cs_attribute(blcelc,iQuestTriggerType,const char*,Name,GetName)

/* iQuestRewardType (include/tools/questmanager.h) */
%cs_attribute(blcelc,iQuestRewardType,const char*,Name,GetName)

/* iQuestSeqOpType (include/tools/questmanager.h) */
%cs_attribute(blcelc,iQuestSeqOpType,const char*,Name,GetName)

/* iQuestSequence (include/tools/questmanager.h) */
%cs_attribute(blcelc,iQuestSequence,const char*,Name,GetName)
%cs_attribute(blcelc,iQuestSequence,bool,Running,IsRunning)

/* iQuest (include/tools/questmanager.h) */
%cs_attribute(blcelc,iQuest,const char*,CurrentState,GetCurrentState)

/* iQuestTriggerResponseFactory (include/tools/questmanager.h) */
%cs_attribute_writeonly(blcelc,iQuestTriggerResponseFactory, iQuestTriggerFactory*, TriggerFactory, SetTriggerFactory)

/* iQuestStateFactory (include/tools/questmanager.h) */
%cs_attribute(blcelc,iQuestStateFactory,const char*,Name,GetName)

/* iQuestSequenceFactory (include/tools/questmanager.h) */
%cs_attribute(blcelc,iQuestSequenceFactory,const char*,Name,GetName)

/* iQuestFactory (include/tools/questmanager.h) */
%cs_attribute(blcelc,iQuestFactory,const char*,Name,GetName)

/* iQuestManager (include/tools/questmanager.h) */
%cs_multi_attr_writeonly(blcelc,iQuestManager,TimeoutTrigger,SetTimeoutTrigger)
%cs_multi_attr_writeonly(blcelc,iQuestManager,EnterSectorTrigger,SetEnterSectorTrigger)
%cs_multi_attr_writeonly(blcelc,iQuestManager,MeshEnterSectorTrigger,SetMeshEnterSectorTrigger)
%cs_multi_attr_writeonly(blcelc,iQuestManager,SequenceFinishTrigger,SetSequenceFinishTrigger)
%cs_multi_attr_writeonly(blcelc,iQuestManager,PropertyChangeTrigger,SetPropertyChangeTrigger)
%cs_multi_attr_writeonly(blcelc,iQuestManager,TriggerTrigger,SetTriggerTrigger)
%cs_multi_attr_writeonly(blcelc,iQuestManager,WatchTrigger,SetWatchTrigger)
%cs_multi_attr_writeonly(blcelc,iQuestManager,OperationTrigger,SetOperationTrigger)

/* iTimeoutQuestTriggerFactory (include/tools/questmanager.h) */
%cs_attribute_writeonly(blcelc,iTimeoutQuestTriggerFactory, const char*, TimeoutParameter, SetTimeoutParameter)

/* iPropertyChangeQuestTriggerFactory (include/tools/questmanager.h) */
%cs_multi_attr_writeonly(blcelc,iPropertyChangeQuestTriggerFactory,EntityParameter,SetEntityParameter)
%cs_attribute_writeonly(blcelc,iPropertyChangeQuestTriggerFactory, const char*, PropertyParameter, SetPropertyParameter)
%cs_attribute_writeonly(blcelc,iPropertyChangeQuestTriggerFactory, const char*, ValueParameter, SetValueParameter)
%cs_attribute_writeonly(blcelc,iPropertyChangeQuestTriggerFactory, const char*, OperationParameter, SetOperationParameter)
%cs_attribute_writeonly(blcelc,iPropertyChangeQuestTriggerFactory, bool, OnChangeOnly, SetOnChangeOnly)

/* iMeshSelectQuestTriggerFactory (include/tools/questmanager.h) */
%cs_multi_attr_writeonly(blcelc,iMeshSelectQuestTriggerFactory,EntityParameter,SetEntityParameter)

/* iInventoryQuestTriggerFactory (include/tools/questmanager.h) */
%cs_multi_attr_writeonly(blcelc,iInventoryQuestTriggerFactory,EntityParameter,SetEntityParameter)
%cs_attribute_writeonly(blcelc,iInventoryQuestTriggerFactory, const char*, ChildEntityParameter, SetChildEntityParameter)

/* iEnterSectorQuestTriggerFactory (include/tools/questmanager.h) */
%cs_multi_attr_writeonly(blcelc,iEnterSectorQuestTriggerFactory,EntityParameter,SetEntityParameter)
%cs_attribute_writeonly(blcelc,iEnterSectorQuestTriggerFactory, const char*, SectorParameter, SetSectorParameter)

/* iOperationQuestTriggerFactory (include/tools/questmanager.h) */
%cs_attribute_writeonly(blcelc,iOperationQuestTriggerFactory, const char*, OperationParameter, SetOperationParameter)

/* iSequenceFinishQuestTriggerFactory (include/tools/questmanager.h) */
%cs_multi_attr_writeonly(blcelc,iSequenceFinishQuestTriggerFactory,EntityParameter,SetEntityParameter)
%cs_attribute_writeonly(blcelc,iSequenceFinishQuestTriggerFactory, const char*, SequenceParameter, SetSequenceParameter)

/* iTriggerQuestTriggerFactory (include/tools/questmanager.h) */
%cs_multi_attr_writeonly(blcelc,iTriggerQuestTriggerFactory,EntityParameter,SetEntityParameter)

/* iMessageQuestTriggerFactory (include/tools/questmanager.h) */
%cs_attribute_writeonly(blcelc,iMessageQuestTriggerFactory, const char*, EntityParameter, SetEntityParameter)
%cs_attribute_writeonly(blcelc,iMessageQuestTriggerFactory, const char*, MaskParameter, SetMaskParameter)

/* iWatchQuestTriggerFactory (include/tools/questmanager.h) */
%cs_multi_attr_writeonly(blcelc,iWatchQuestTriggerFactory,EntityParameter,SetEntityParameter)
%cs_multi_attr_writeonly(blcelc,iWatchQuestTriggerFactory,TargetEntityParameter,SetTargetEntityParameter)
%cs_attribute_writeonly(blcelc,iWatchQuestTriggerFactory, const char*, ChecktimeParameter, SetChecktimeParameter)
%cs_attribute_writeonly(blcelc,iWatchQuestTriggerFactory, const char*, RadiusParameter, SetRadiusParameter)
%cs_multi_attr_writeonly(blcelc,iWatchQuestTriggerFactory,OffsetParameter,SetOffsetParameter)

/* iDebugPrintQuestRewardFactory (include/tools/questmanager.h) */
%cs_attribute_writeonly(blcelc,iDebugPrintQuestRewardFactory, const char*, MessageParameter, SetMessageParameter)

/* iNewStateQuestRewardFactory (include/tools/questmanager.h) */
%cs_attribute_writeonly(blcelc,iNewStateQuestRewardFactory, const char*, StateParameter, SetStateParameter)
%cs_multi_attr_writeonly(blcelc,iNewStateQuestRewardFactory,EntityParameter,SetEntityParameter)
%cs_attribute_writeonly(blcelc,iNewStateQuestRewardFactory, const char*, TagParameter, SetTagParameter)
%cs_attribute_writeonly(blcelc,iNewStateQuestRewardFactory, const char*, ClassParameter, SetClassParameter)

/* iChangePropertyQuestRewardFactory (include/tools/questmanager.h) */
%cs_attribute_writeonly(blcelc,iChangePropertyQuestRewardFactory, const char*, EntityParameter, SetEntityParameter)
%cs_attribute_writeonly(blcelc,iChangePropertyQuestRewardFactory, const char*, ClassParameter, SetClassParameter)
%cs_multi_attr_writeonly(blcelc,iChangePropertyQuestRewardFactory,PCParameter,SetPCParameter)
%cs_attribute_writeonly(blcelc,iChangePropertyQuestRewardFactory, const char*, PropertyParameter, SetPropertyParameter)
%cs_attribute_writeonly(blcelc,iChangePropertyQuestRewardFactory, const char*, StringParameter, SetStringParameter)
%cs_attribute_writeonly(blcelc,iChangePropertyQuestRewardFactory, const char*, LongParameter, SetLongParameter)
%cs_attribute_writeonly(blcelc,iChangePropertyQuestRewardFactory, const char*, FloatParameter, SetFloatParameter)
%cs_attribute_writeonly(blcelc,iChangePropertyQuestRewardFactory, const char*, BoolParameter, SetBoolParameter)
%cs_attribute_writeonly(blcelc,iChangePropertyQuestRewardFactory, const char*, DiffParameter, SetDiffParameter)
%cs_multi_attr_writeonly(blcelc,iChangePropertyQuestRewardFactory,Toggle,SetToggle)

/* iInventoryQuestRewardFactory (include/tools/questmanager.h) */
%cs_multi_attr_writeonly(blcelc,iInventoryQuestRewardFactory,EntityParameter,SetEntityParameter)
%cs_multi_attr_writeonly(blcelc,iInventoryQuestRewardFactory,ChildEntityParameter,SetChildEntityParameter)

/* iCsSequenceQuestRewardFactory (include/tools/questmanager.h) */
%cs_attribute_writeonly(blcelc,iCsSequenceQuestRewardFactory, const char*, SequenceParameter, SetSequenceParameter)
%cs_attribute_writeonly(blcelc,iCsSequenceQuestRewardFactory, const char*, DelayParameter, SetDelayParameter)

/* iSequenceQuestRewardFactory (include/tools/questmanager.h) */
%cs_multi_attr_writeonly(blcelc,iSequenceQuestRewardFactory,EntityParameter,SetEntityParameter)
%cs_attribute_writeonly(blcelc,iSequenceQuestRewardFactory, const char*, TagParameter, SetTagParameter)
%cs_attribute_writeonly(blcelc,iSequenceQuestRewardFactory, const char*, ClassParameter, SetClassParameter)
%cs_attribute_writeonly(blcelc,iSequenceQuestRewardFactory, const char*, SequenceParameter, SetSequenceParameter)
%cs_attribute_writeonly(blcelc,iSequenceQuestRewardFactory, const char*, DelayParameter, SetDelayParameter)

/* iSequenceFinishQuestRewardFactory (include/tools/questmanager.h) */
%cs_multi_attr_writeonly(blcelc,iSequenceFinishQuestRewardFactory,EntityParameter,SetEntityParameter)
%cs_attribute_writeonly(blcelc,iSequenceFinishQuestRewardFactory, const char*, TagParameter, SetTagParameter)
%cs_attribute_writeonly(blcelc,iSequenceFinishQuestRewardFactory, const char*, ClassParameter, SetClassParameter)
%cs_attribute_writeonly(blcelc,iSequenceFinishQuestRewardFactory, const char*, SequenceParameter, SetSequenceParameter)

/* iMessageQuestRewardFactory (include/tools/questmanager.h) */
%cs_attribute_writeonly(blcelc,iMessageQuestRewardFactory, const char*, EntityParameter, SetEntityParameter)
%cs_attribute_writeonly(blcelc,iMessageQuestRewardFactory, const char*, ClassParameter, SetClassParameter)
%cs_attribute_writeonly(blcelc,iMessageQuestRewardFactory, const char*, IDParameter, SetIDParameter)

/* iActionQuestRewardFactory (include/tools/questmanager.h) */
%cs_attribute_writeonly(blcelc,iActionQuestRewardFactory, const char*, EntityParameter, SetEntityParameter)
%cs_attribute_writeonly(blcelc,iActionQuestRewardFactory, const char*, IDParameter, SetIDParameter)
%cs_attribute_writeonly(blcelc,iActionQuestRewardFactory, const char*, PropertyClassParameter, SetPropertyClassParameter)
%cs_attribute_writeonly(blcelc,iActionQuestRewardFactory, const char*, TagParameter, SetTagParameter)

/* iDestroyEntityQuestRewardFactory (include/tools/questmanager.h) */
%cs_attribute_writeonly(blcelc,iDestroyEntityQuestRewardFactory, const char*, EntityParameter, SetEntityParameter)
%cs_attribute_writeonly(blcelc,iDestroyEntityQuestRewardFactory, const char*, ClassParameter, SetClassParameter)

/* iCreateEntityQuestRewardFactory (include/tools/questmanager.h) */
%cs_attribute_writeonly(blcelc,iCreateEntityQuestRewardFactory, const char*, EntityTemplateParameter, SetEntityTemplateParameter)
%cs_attribute_writeonly(blcelc,iCreateEntityQuestRewardFactory, const char*, NameParameter, SetNameParameter)

/* iDebugPrintQuestSeqOpFactory (include/tools/questmanager.h) */
%cs_attribute_writeonly(blcelc,iDebugPrintQuestSeqOpFactory, const char*, MessageParameter, SetMessageParameter)

/* iTransformQuestSeqOpFactory (include/tools/questmanager.h) */
%cs_multi_attr_writeonly(blcelc,iTransformQuestSeqOpFactory,EntityParameter,SetEntityParameter)
%cs_multi_attr_writeonly(blcelc,iTransformQuestSeqOpFactory,VectorParameter,SetVectorParameter)
%cs_multi_attr_writeonly(blcelc,iTransformQuestSeqOpFactory,RotationParameter,SetRotationParameter)

/* iMovePathQuestSeqOpFactory (include/tools/questmanager.h) */
%cs_multi_attr_writeonly(blcelc,iMovePathQuestSeqOpFactory,EntityParameter,SetEntityParameter)

/* iLightQuestSeqOpFactory (include/tools/questmanager.h) */
%cs_multi_attr_writeonly(blcelc,iLightQuestSeqOpFactory,EntityParameter,SetEntityParameter)
%cs_multi_attr_writeonly(blcelc,iLightQuestSeqOpFactory,RelColorParameter,SetRelColorParameter)
%cs_multi_attr_writeonly(blcelc,iLightQuestSeqOpFactory,AbsColorParameter,SetAbsColorParameter)

/* iPropertyQuestSeqOpFactory (include/tools/questmanager.h) */
%cs_attribute_writeonly(blcelc,iPropertyQuestSeqOpFactory, const char*, EntityParameter, SetEntityParameter)
%cs_multi_attr_writeonly(blcelc,iPropertyQuestSeqOpFactory,PCParameter,SetPCParameter)
%cs_attribute_writeonly(blcelc,iPropertyQuestSeqOpFactory, const char*, PropertyParameter, SetPropertyParameter)
%cs_attribute_writeonly(blcelc,iPropertyQuestSeqOpFactory, const char*, FloatParameter, SetFloatParameter)
%cs_attribute_writeonly(blcelc,iPropertyQuestSeqOpFactory, const char*, LongParameter, SetLongParameter)
%cs_multi_attr_writeonly(blcelc,iPropertyQuestSeqOpFactory,Vector2Parameter,SetVector2Parameter)
%cs_multi_attr_writeonly(blcelc,iPropertyQuestSeqOpFactory,Vector3Parameter,SetVector3Parameter)
%cs_attribute_writeonly(blcelc,iPropertyQuestSeqOpFactory, bool, Relative, SetRelative)

/* iCelEdge (include/tools/celgraph.h) */
%cs_attribute(blcelc,iCelEdge,bool,State)
%cs_attribute(blcelc,iCelEdge,iCelNode*,Successor)

/* iCelNode (include/tools/celgraph.h) */
%cs_attribute(blcelc,iCelNode,iMapNode*,MapNode)
%cs_attribute(blcelc,iCelNode,iCelNode*,Parent)
%cs_attribute(blcelc,iCelNode,char*,Name)
%cs_attribute(blcelc,iCelNode,csVector3,Position,GetPosition)
%cs_attribute(blcelc,iCelNode,csArray<iCelNode*  >,Successors,GetSuccessors)
%cs_attribute(blcelc,iCelNode,csArray<iCelNode*  >,AllSuccessors,GetAllSuccessors)
%cs_attribute(blcelc,iCelNode,float,Cost,GetCost)

/* iCelPath (include/tools/celgraph.h) */
%cs_attribute(blcelc,iCelPath,iMapNode*,First,GetFirst)
%cs_attribute(blcelc,iCelPath,iMapNode*,Last,GetLast)

/* iCelGraph (include/tools/celgraph.h) */

/* iBillboardLayer (include/tools/billboard.h) */
%cs_attribute(blcelc,iBillboardLayer,const char*,Name,GetName)

/* iBillboard (include/tools/billboard.h) */
%cs_attribute(blcelc,iBillboard,const char*,Name,GetName)
%cs_attribute(blcelc,iBillboard,csFlags&,Flags,GetFlags)
%cs_attribute(blcelc,iBillboard,const char*,MaterialName)
%cs_attribute_writeonly(blcelc,iBillboard, const char*, MaterialNameFast, SetMaterialNameFast)
%cs_attribute(blcelc,iBillboard,float,Rotation)
%cs_attribute(blcelc,iBillboard,const csVector2&,UVTopLeft)
%cs_attribute(blcelc,iBillboard,const csVector2&,UVBottomRight)
%cs_attribute(blcelc,iBillboard,const csColor&,Color)
%cs_attribute(blcelc,iBillboard,iBillboardLayer*,Layer)
%cs_attribute(blcelc,iBillboard,const char*,Text)
%cs_multi_attr_writeonly(blcelc,iBillboard,TextOffset,SetTextOffset)
%cs_attribute(blcelc,iBillboard,int,TextOffsetDX,GetTextOffsetDX)
%cs_attribute(blcelc,iBillboard,int,TextOffsetDY,GetTextOffsetDY)
%cs_multi_attr(blcelc,iBillboard,TextFont,GetTextFont,SetTextFont)
%cs_attribute(blcelc,iBillboard,float,TextFontSize,GetTextFontSize)
%cs_attribute(blcelc,iBillboard,const csColor&,TextFgColor)
%cs_attribute(blcelc,iBillboard,const csColor&,TextBgColor)
%cs_attribute(blcelc,iBillboard,bool,TextBgTransparent,IsTextBgTransparent)

/* iBillboardManager (include/tools/billboard.h) */
%cs_attribute(blcelc,iBillboardManager,size_t,BillboardCount,GetBillboardCount)
%cs_attribute(blcelc,iBillboardManager,size_t,BillboardLayerCount,GetBillboardLayerCount)
%cs_multi_attr_writeonly(blcelc,iBillboardManager,Flags,SetFlags)
%cs_multi_attr(blcelc,iBillboardManager,DefaultTextFont,GetDefaultTextFont,SetDefaultTextFont)
%cs_attribute(blcelc,iBillboardManager,float,DefaultTextFontSize,GetDefaultTextFontSize)
%cs_attribute(blcelc,iBillboardManager,const csColor&,DefaultTextFgColor)
%cs_attribute(blcelc,iBillboardManager,const csColor&,DefaultTextBgColor)
%cs_multi_attr_writeonly(blcelc,iBillboardManager,DefaultTextBgTransparent,SetDefaultTextBgTransparent)
%cs_attribute(blcelc,iBillboardManager,iSector*,Showroom,GetShowroom)

/* iCelPlLayer (include/physicallayer/pl.h) */
%cs_attribute(blcelc,iCelPlLayer,size_t,EntityTemplateCount,GetEntityTemplateCount)
%cs_attribute(blcelc,iCelPlLayer,size_t,EntityCount,GetEntityCount)
%cs_attribute(blcelc,iCelPlLayer,bool,EntityAddonAllowed,IsEntityAddonAllowed,SetEntityAddonAllowed)
%cs_attribute(blcelc,iCelPlLayer,size_t,PropertyClassFactoryCount,GetPropertyClassFactoryCount)
%cs_attribute(blcelc,iCelPlLayer,size_t,BehaviourLayerCount,GetBehaviourLayerCount)
%cs_attribute(blcelc,iCelPlLayer,iMessageSender*,MessageSender,QueryMessageSender)

/* iCelEntityTracker (include/physicallayer/pl.h) */
%cs_attribute(blcelc,iCelEntityTracker,const char*,Name,GetName)
%cs_attribute(blcelc,iCelEntityTracker,csPtr<iCelEntityIterator>,Iterator,GetIterator)

/* iCelEntityTemplate (include/physicallayer/entitytpl.h) */
%cs_attribute(blcelc,iCelEntityTemplate,iObject*,Object,QueryObject)
%cs_attribute(blcelc,iCelEntityTemplate,const char*,Name)
%cs_multi_attr(blcelc,iCelEntityTemplate,Behaviour,GetBehaviour,SetBehaviour)
%cs_attribute(blcelc,iCelEntityTemplate,const char*,BehaviourLayer,GetBehaviourLayer)
%cs_attribute(blcelc,iCelEntityTemplate,const csSet<csStringID>&,Classes,GetClasses)

/* iCelPropertyClassTemplate (include/physicallayer/entitytpl.h) */
%cs_attribute(blcelc,iCelPropertyClassTemplate,const char*,Name)
%cs_attribute(blcelc,iCelPropertyClassTemplate,const char*,Tag)
%cs_multi_attr_writeonly(blcelc,iCelPropertyClassTemplate,PropertyVariable,SetPropertyVariable)
%cs_multi_attr_writeonly(blcelc,iCelPropertyClassTemplate,Property,SetProperty)

/* iMessageReceiverFilter (include/physicallayer/messaging.h) */

/* iCelPropertyClass (include/physicallayer/propclas.h) */
%cs_attribute(blcelc,iCelPropertyClass,const char*,Name)
%cs_attribute(blcelc,iCelPropertyClass,const char*,Tag)
%cs_attribute(blcelc,iCelPropertyClass,iCelEntity*,Entity)
%cs_multi_attr_writeonly(blcelc,iCelPropertyClass,Property,SetProperty)
%cs_attribute(blcelc,iCelPropertyClass,size_t,PropertyAndActionCount,GetPropertyAndActionCount)

/* iCelPropertyClassList (include/physicallayer/propclas.h) */
%cs_attribute(blcelc,iCelPropertyClassList,size_t,Count,GetCount)

/* iCelPropertyClassFactory (include/physicallayer/propfact.h) */
%cs_attribute(blcelc,iCelPropertyClassFactory,const char*,Name,GetName)

/* iCelEntity (include/physicallayer/entity.h) */
%cs_attribute(blcelc,iCelEntity,iObject*,Object,QueryObject)
%cs_attribute(blcelc,iCelEntity,const char*,Name)
%cs_attribute(blcelc,iCelEntity,uint,ID)
%cs_attribute(blcelc,iCelEntity,iCelPropertyClassList*,PropertyClassList,GetPropertyClassList)
%cs_attribute(blcelc,iCelEntity,iCelBehaviour*,Behaviour)
%cs_attribute(blcelc,iCelEntity,const csSet<csStringID>&,Classes,GetClasses)
%cs_attribute(blcelc,iCelEntity,iMessageChannel*,MessageChannel,QueryMessageChannel)

/* iCelEntityList (include/physicallayer/entity.h) */
%cs_attribute(blcelc,iCelEntityList,size_t,Count,GetCount)
%cs_attribute(blcelc,iCelEntityList,csPtr<iCelEntityIterator>,Iterator,GetIterator)

/* iCelEntityIterator (include/physicallayer/entity.h) */

/* iPcPathFinder (include/propclass/pathfinder.h) */
%cs_attribute_writeonly(blcelc,iPcPathFinder, int, DelayRecheck, SetDelayRecheck)
%cs_attribute_writeonly(blcelc,iPcPathFinder, int, MinDistance, SetMinDistance)
%cs_attribute(blcelc,iPcPathFinder,const csVector3&,Position,GetPosition)
%cs_attribute(blcelc,iPcPathFinder,bool,Active,IsActive)
%cs_attribute_writeonly(blcelc,iPcPathFinder, iCelGraph*, Graph, SetGraph)

/* iPcDamage (include/propclass/damage.h) */
%cs_attribute(blcelc,iPcDamage,float,Damage)
%cs_attribute(blcelc,iPcDamage,const char*,DamageType)
%cs_attribute(blcelc,iPcDamage,const char*,FallOff)
%cs_multi_attr_writeonly(blcelc,iPcDamage,DamageLocation,SetDamageLocation)
%cs_attribute(blcelc,iPcDamage,const char*,DamageSector,GetDamageSector)
%cs_attribute(blcelc,iPcDamage,const csVector3&,DamagePosition,GetDamagePosition)
%cs_attribute(blcelc,iPcDamage,const char*,DamageSource)

/* iPcMeshDeform (include/propclass/meshdeform.h) */
%cs_attribute(blcelc,iPcMeshDeform,iMeshWrapper*,Mesh)
%cs_attribute(blcelc,iPcMeshDeform,float,DeformFactor)
%cs_attribute(blcelc,iPcMeshDeform,float,Noise)
%cs_attribute(blcelc,iPcMeshDeform,float,MaxFrequency)
%cs_attribute(blcelc,iPcMeshDeform,float,MaxDeform)
%cs_attribute(blcelc,iPcMeshDeform,float,Radius)

/* iPcCraftController (include/propclass/craft.h) */
%cs_attribute_writeonly(blcelc,iPcCraftController, float, AccTurn, SetAccTurn)
%cs_attribute_writeonly(blcelc,iPcCraftController, float, AccPitch, SetAccPitch)
%cs_attribute_writeonly(blcelc,iPcCraftController, float, MaxTurn, SetMaxTurn)
%cs_attribute_writeonly(blcelc,iPcCraftController, float, MaxPitch, SetMaxPitch)
%cs_attribute_writeonly(blcelc,iPcCraftController, float, Roll, SetRoll)
%cs_attribute_writeonly(blcelc,iPcCraftController, float, ThrustForce, SetThrustForce)
%cs_attribute_writeonly(blcelc,iPcCraftController, float, TopSpeed, SetTopSpeed)
%cs_attribute_writeonly(blcelc,iPcCraftController, float, RedirectVelocityRatio, SetRedirectVelocityRatio)
%cs_attribute_writeonly(blcelc,iPcCraftController, float, DecelerationRate, SetDecelerationRate)
%cs_attribute_writeonly(blcelc,iPcCraftController, float, BrakingSpeed, SetBrakingSpeed)
%cs_attribute(blcelc,iPcCraftController,bool,ThrusterOn,IsThrusterOn)
%cs_attribute_writeonly(blcelc,iPcCraftController, float, AfterBurnerTopSpeed, SetAfterBurnerTopSpeed)

/* iCelInventorySpaceSlot (include/propclass/inv.h) */
%cs_attribute(blcelc,iCelInventorySpaceSlot,int,Count,GetCount)
%cs_attribute(blcelc,iCelInventorySpaceSlot,bool,Empty,IsEmpty)

/* iCelInventorySpace (include/propclass/inv.h) */
%cs_attribute(blcelc,iCelInventorySpace,int,SpaceTotal,GetSpaceTotal)
%cs_attribute(blcelc,iCelInventorySpace,int,SpaceLeft,GetSpaceLeft)
%cs_attribute(blcelc,iCelInventorySpace,int,SpaceTaken,GetSpaceTaken)

/* iPcInventory (include/propclass/inv.h) */
%cs_attribute(blcelc,iPcInventory,size_t,EntityCount,GetEntityCount)
%cs_attribute(blcelc,iPcInventory,iCelInventorySpace*,Space)

/* iPcTrigger (include/propclass/trigger.h) */
%cs_multi_attr_writeonly(blcelc,iPcTrigger,MonitorDelay,SetMonitorDelay)
%cs_attribute(blcelc,iPcTrigger,bool,Enabled,IsEnabled)
%cs_attribute(blcelc,iPcTrigger,const csWeakRefArray<iCelEntity>&,EntitiesInTrigger,GetEntitiesInTrigger)

/* iPcQuest (include/propclass/quest.h) */
%cs_attribute(blcelc,iPcQuest,iQuest*,Quest,GetQuest)
%cs_attribute(blcelc,iPcQuest,const char*,QuestName,GetQuestName)

/* iPcWheeled (include/propclass/wheeled.h) */
%cs_multi_attr_writeonly(blcelc,iPcWheeled,WheelMesh,SetWheelMesh)
%cs_attribute(blcelc,iPcWheeled,bool,TankMode)
%cs_attribute(blcelc,iPcWheeled,CS_DEPRECATED_METHOD  bool,ABSEnabled)
%cs_attribute(blcelc,iPcWheeled,bool,Differential)
%cs_attribute(blcelc,iPcWheeled,bool,AntiSway)
%cs_attribute(blcelc,iPcWheeled,float,AntiSwayFactor)
%cs_attribute(blcelc,iPcWheeled,float,AntiSwayLimit)
%cs_attribute(blcelc,iPcWheeled,bool,ABS)
%cs_attribute_writeonly(blcelc,iPcWheeled, float, OuterWheelSteerPreset, SetOuterWheelSteerPreset)
%cs_multi_attr_writeonly(blcelc,iPcWheeled,FrontWheelPreset,SetFrontWheelPreset)
%cs_multi_attr_writeonly(blcelc,iPcWheeled,RearWheelPreset,SetRearWheelPreset)
%cs_attribute(blcelc,iPcWheeled,float,AcceleratorAmount,GetAcceleratorAmount)
%cs_attribute(blcelc,iPcWheeled,float,BrakeAmount,GetBrakeAmount)
%cs_attribute(blcelc,iPcWheeled,bool,Handbraking,IsHandbraking,Handbrake)
%cs_attribute(blcelc,iPcWheeled,float,SteerAmount)
%cs_attribute_writeonly(blcelc,iPcWheeled, float, BrakeForce, SetBrakeForce)
%cs_attribute(blcelc,iPcWheeled,float,Speed,GetSpeed)
%cs_attribute_writeonly(blcelc,iPcWheeled, bool, AutoTransmission, SetAutoTransmission)
%cs_multi_attr(blcelc,iPcWheeled,Gear,GetGear,SetGear)
%cs_attribute(blcelc,iPcWheeled,int,TopGear,GetTopGear)
%cs_attribute_writeonly(blcelc,iPcWheeled, bool, AutoReverse, SetAutoReverse)
%cs_attribute(blcelc,iPcWheeled,iBodyGroup*,BodyGroup,GetBodyGroup)
%cs_attribute(blcelc,iPcWheeled,size_t,WheelCount,GetWheelCount)
%cs_attribute(blcelc,iPcWheeled,float,AverageWheelSpin,GetAverageWheelSpin)
%cs_attribute(blcelc,iPcWheeled,bool,CollisionCallbackEnabled,IsCollisionCallbackEnabled,SetCollisionCallbackEnabled)

/* iPcTooltip (include/propclass/tooltip.h) */
%cs_attribute_writeonly(blcelc,iPcTooltip, const char*, Text, SetText)
%cs_attribute(blcelc,iPcTooltip,bool,Visible,IsVisible)
%cs_multi_attr_writeonly(blcelc,iPcTooltip,TextColor,SetTextColor)
%cs_multi_attr_writeonly(blcelc,iPcTooltip,BackgroundColor,SetBackgroundColor)
%cs_attribute_writeonly(blcelc,iPcTooltip, iFont*, Font, SetFont)
%cs_attribute_writeonly(blcelc,iPcTooltip, celTooltipJustify, Justify, SetJustify)

/* iPcSolid (include/propclass/solid.h) */
%cs_attribute(blcelc,iPcSolid,iPcMesh*,Mesh)
%cs_attribute(blcelc,iPcSolid,iCollider*,Collider,GetCollider)

/* iPcMechanicsSystem (include/propclass/mechsys.h) */
%cs_multi_attr(blcelc,iPcMechanicsSystem,DynamicSystem,GetDynamicSystem,SetDynamicSystem)
%cs_attribute(blcelc,iPcMechanicsSystem,float,StepTime)
%cs_attribute(blcelc,iPcMechanicsSystem,float,SimulationSpeed)
%cs_attribute(blcelc,iPcMechanicsSystem,const csVector3,Gravity)

/* iPcMechanicsObject (include/propclass/mechsys.h) */
%cs_attribute(blcelc,iPcMechanicsObject,iPcMesh*,Mesh)
%cs_attribute(blcelc,iPcMechanicsObject,iPcLight*,Light)
%cs_attribute(blcelc,iPcMechanicsObject,iPcCamera*,Camera)
%cs_attribute(blcelc,iPcMechanicsObject,iPcMechanicsSystem*,MechanicsSystem)
%cs_attribute(blcelc,iPcMechanicsObject,iRigidBody*,Body,GetBody)
%cs_attribute(blcelc,iPcMechanicsObject,float,Friction)
%cs_attribute(blcelc,iPcMechanicsObject,float,Mass)
%cs_attribute(blcelc,iPcMechanicsObject,float,Elasticity)
%cs_attribute(blcelc,iPcMechanicsObject,float,Density)
%cs_attribute(blcelc,iPcMechanicsObject,float,Softness)
%cs_attribute(blcelc,iPcMechanicsObject,const csVector3&,Lift)
%cs_attribute(blcelc,iPcMechanicsObject,float,Drag)
%cs_attribute(blcelc,iPcMechanicsObject,const csVector3,LinearVelocity)
%cs_attribute(blcelc,iPcMechanicsObject,const csVector3,AngularVelocity)
%cs_attribute(blcelc,iPcMechanicsObject,bool,Static,IsStatic)
%cs_attribute(blcelc,iPcMechanicsObject,bool,CollisionCallbackEnabled,IsCollisionCallbackEnabled,SetCollisionCallbackEnabled)

/* iPcMechanicsJoint (include/propclass/mechsys.h) */
%cs_attribute(blcelc,iPcMechanicsJoint,iJoint*,Joint,GetJoint)

/* iPcSpawn (include/propclass/spawn.h) */
%cs_attribute(blcelc,iPcSpawn,bool,Enabled,IsEnabled,SetEnabled)
%cs_multi_attr_writeonly(blcelc,iPcSpawn,Timing,SetTiming)
%cs_attribute_writeonly(blcelc,iPcSpawn, bool, EntityNameCounter, SetEntityNameCounter)
%cs_attribute(blcelc,iPcSpawn,bool,EntityNameCounterCounter,IsEntityNameCounterCounter)
%cs_attribute(blcelc,iPcSpawn,bool,SpawnUniqueEnabled,IsSpawnUniqueEnabled)

/* iPcSoundListener (include/propclass/sound.h) */
%cs_attribute(blcelc,iPcSoundListener,iSndSysListener*,SoundListener,GetSoundListener)

/* iPcSoundSource (include/propclass/sound.h) */
%cs_attribute(blcelc,iPcSoundSource,iSndSysSource*,SoundSource,GetSoundSource)
%cs_attribute(blcelc,iPcSoundSource,const char*,SoundName)
%cs_attribute(blcelc,iPcSoundSource,const char*,Mode)

/* iPcCamera (include/propclass/camera.h) */
%cs_multi_attr_writeonly(blcelc,iPcCamera,Region,SetRegion)
%cs_multi_attr_writeonly(blcelc,iPcCamera,ZoneManager,SetZoneManager)
%cs_multi_attr_writeonly(blcelc,iPcCamera,Rectangle,SetRectangle)
%cs_attribute(blcelc,iPcCamera,iCamera*,Camera,GetCamera)
%cs_attribute(blcelc,iPcCamera,iView*,View,GetView)
%cs_attribute(blcelc,iPcCamera,bool,ClearZBuffer)
%cs_attribute(blcelc,iPcCamera,bool,ClearScreen)
%cs_attribute(blcelc,iPcCamera,float,FixedDistance,GetFixedDistance)
%cs_attribute(blcelc,iPcCamera,float,AdaptiveMinFPS,GetAdaptiveMinFPS)
%cs_attribute(blcelc,iPcCamera,float,AdaptiveMaxFPS,GetAdaptiveMaxFPS)
%cs_attribute(blcelc,iPcCamera,float,AdaptiveMinDistance,GetAdaptiveMinDistance)
%cs_attribute_writeonly(blcelc,iPcCamera, bool, AutoDraw, SetAutoDraw)
%cs_attribute(blcelc,iPcCamera,int,DrawFlags,GetDrawFlags)
%cs_multi_attr_writeonly(blcelc,iPcCamera,PerspectiveCenter,SetPerspectiveCenter)

/* iPcSteer (include/propclass/steer.h) */
%cs_attribute(blcelc,iPcSteer,iSector*,Sector,GetSector)
%cs_attribute(blcelc,iPcSteer,const csVector3&,Position,GetPosition)
%cs_attribute_writeonly(blcelc,iPcSteer, int, DelayRecheck, SetDelayRecheck)
%cs_attribute(blcelc,iPcSteer,bool,Moving,IsMoving)

/* iPcActorMove (include/propclass/actormove.h) */
%cs_attribute(blcelc,iPcActorMove,bool,MovingForward,IsMovingForward,Forward)
%cs_attribute(blcelc,iPcActorMove,bool,MovingBackward,IsMovingBackward,Backward)
%cs_attribute(blcelc,iPcActorMove,bool,Moving,IsMoving)
%cs_attribute(blcelc,iPcActorMove,bool,StrafingLeft,IsStrafingLeft,StrafeLeft)
%cs_attribute(blcelc,iPcActorMove,bool,StrafingRight,IsStrafingRight,StrafeRight)
%cs_attribute(blcelc,iPcActorMove,bool,RotatingLeft,IsRotatingLeft,RotateLeft)
%cs_attribute(blcelc,iPcActorMove,bool,RotatingRight,IsRotatingRight,RotateRight)
%cs_attribute(blcelc,iPcActorMove,bool,Running,IsRunning,Run)
%cs_attribute(blcelc,iPcActorMove,bool,AutoRunning,IsAutoRunning,AutoRun)
%cs_attribute(blcelc,iPcActorMove,bool,MouseMoveEnabled,IsMouseMoveEnabled)
%cs_attribute(blcelc,iPcActorMove,bool,MouseMoveAccelerated,IsMouseMoveAccelerated,SetMouseMoveAccelerated)
%cs_attribute(blcelc,iPcActorMove,bool,MouseMoveInverted,IsMouseMoveInverted,SetMouseMoveInverted)
%cs_multi_attr_writeonly(blcelc,iPcActorMove,MouseMoveSpeed,SetMouseMoveSpeed)
%cs_attribute(blcelc,iPcActorMove,float,MouseMoveSpeedX,GetMouseMoveSpeedX)
%cs_attribute(blcelc,iPcActorMove,float,MouseMoveSpeedY,GetMouseMoveSpeedY)
%cs_attribute(blcelc,iPcActorMove,float,MovementSpeed)
%cs_attribute(blcelc,iPcActorMove,float,RunningSpeed)
%cs_attribute(blcelc,iPcActorMove,float,RotationSpeed)
%cs_attribute(blcelc,iPcActorMove,float,JumpingVelocity)
%cs_multi_attr_writeonly(blcelc,iPcActorMove,Animation,SetAnimation)
%cs_multi_attr_writeonly(blcelc,iPcActorMove,AnimationMapping,SetAnimationMapping)

/* iPcMechanicsThruster (include/propclass/mechthruster.h) */
%cs_attribute(blcelc,iPcMechanicsThruster,iPcMechanicsObject*,MechanicsObject)
%cs_attribute(blcelc,iPcMechanicsThruster,const csVector3&,Position)
%cs_attribute(blcelc,iPcMechanicsThruster,const csVector3&,Orientation)
%cs_attribute(blcelc,iPcMechanicsThruster,float,MaxThrust)

/* iPcMechanicsBalancedGroup (include/propclass/mechthruster.h) */
%cs_attribute(blcelc,iPcMechanicsBalancedGroup,celAxisType,Type)

/* iPcMechanicsThrusterController (include/propclass/mechthruster.h) */
%cs_attribute(blcelc,iPcMechanicsThrusterController,iPcMechanicsObject*,MechanicsObject)

/* iPcMover (include/propclass/mover.h) */
%cs_attribute(blcelc,iPcMover,iSector*,Sector,GetSector)
%cs_attribute(blcelc,iPcMover,const csVector3&,Position,GetPosition)
%cs_attribute(blcelc,iPcMover,CS_DEPRECATED_METHOD  const csVector3&,Up,GetUp)
%cs_attribute(blcelc,iPcMover,float,SqRadius,GetSqRadius)
%cs_attribute(blcelc,iPcMover,bool,Moving,IsMoving)

/* iPcDefaultCamera (include/propclass/defcam.h) */
%cs_attribute_writeonly(blcelc,iPcDefaultCamera, iCelEntity*, FollowEntity, SetFollowEntity)
%cs_attribute(blcelc,iPcDefaultCamera,iPcDefaultCamera::CameraMode,Mode)
%cs_attribute(blcelc,iPcDefaultCamera,const char*,ModeName)
%cs_attribute(blcelc,iPcDefaultCamera,iPcDefaultCamera::CameraMode,NextMode,GetNextMode)
%cs_multi_attr_writeonly(blcelc,iPcDefaultCamera,SpringParameters,SetSpringParameters)
%cs_multi_attr_writeonly(blcelc,iPcDefaultCamera,MinMaxCameraDistance,SetMinMaxCameraDistance)
%cs_attribute_writeonly(blcelc,iPcDefaultCamera, float, TurnSpeed, SetTurnSpeed)
%cs_attribute_writeonly(blcelc,iPcDefaultCamera, float, SwingCoef, SetSwingCoef)
%cs_attribute_writeonly(blcelc,iPcDefaultCamera, const csVector3&, FirstPersonOffset, SetFirstPersonOffset)
%cs_attribute_writeonly(blcelc,iPcDefaultCamera, const csVector3&, ThirdPersonOffset, SetThirdPersonOffset)
%cs_attribute(blcelc,iPcDefaultCamera,float,Pitch)
%cs_attribute(blcelc,iPcDefaultCamera,float,PitchVelocity)
%cs_multi_attr(blcelc,iPcDefaultCamera,Yaw,GetYaw,SetYaw)
%cs_attribute(blcelc,iPcDefaultCamera,float,YawVelocity)
%cs_multi_attr(blcelc,iPcDefaultCamera,Distance,GetDistance,SetDistance)
%cs_attribute(blcelc,iPcDefaultCamera,float,DistanceVelocity)

/* iPcActorAnalog (include/propclass/actoranalog.h) */
%cs_multi_attr_writeonly(blcelc,iPcActorAnalog,Axis,SetAxis)
%cs_attribute_writeonly(blcelc,iPcActorAnalog, float, MovementSpeed, SetMovementSpeed)
%cs_attribute_writeonly(blcelc,iPcActorAnalog, float, TurningSpeed, SetTurningSpeed)

/* iPcMesh (include/propclass/mesh.h) */
%cs_attribute_writeonly(blcelc,iPcMesh, const char*, Path, SetPath)
%cs_multi_attr(blcelc,iPcMesh,Mesh,GetMesh,SetMesh)
%cs_attribute(blcelc,iPcMesh,CS_DEPRECATED_METHOD  const char*,Action)
%cs_attribute_writeonly(blcelc,iPcMesh, bool, ReverseAction, SetReverseAction)
%cs_multi_attr_writeonly(blcelc,iPcMesh,Animation,SetAnimation)
%cs_attribute(blcelc,iPcMesh,bool,Visible,IsVisible)

/* iPcMovable (include/propclass/move.h) */
%cs_attribute(blcelc,iPcMovable,iPcMesh*,Mesh)

/* General (include/propclass/newcamera.h) */

/* Tracking (include/propclass/newcamera.h) */

/* iPcNewCamera (include/propclass/newcamera.h) */
%cs_attribute(blcelc,iPcNewCamera,const csVector3&,BaseOrigin,GetBaseOrigin)
%cs_attribute(blcelc,iPcNewCamera,const csVector3&,BaseDir,GetBaseDir)
%cs_attribute(blcelc,iPcNewCamera,const csVector3&,BaseUp,GetBaseUp)
%cs_attribute(blcelc,iPcNewCamera,const csReversibleTransform&,BaseTrans,GetBaseTrans)
%cs_attribute(blcelc,iPcNewCamera,const csVector3&,Origin,GetOrigin)
%cs_attribute(blcelc,iPcNewCamera,const csVector3&,Target,GetTarget)
%cs_attribute(blcelc,iPcNewCamera,const csVector3&,Up,GetUp)
%cs_attribute_writeonly(blcelc,iPcNewCamera, const csVector3&, TargetPositionOffset, SetTargetPositionOffset)
%cs_attribute_writeonly(blcelc,iPcNewCamera, const csVector3&, CameraPositionOffset, SetCameraPositionOffset)
%cs_attribute(blcelc,iPcNewCamera,float,SpringCoefficient)
%cs_attribute(blcelc,iPcNewCamera,float,OriginSpringCoefficient)
%cs_attribute(blcelc,iPcNewCamera,float,TargetSpringCoefficient)
%cs_attribute(blcelc,iPcNewCamera,float,UpSpringCoefficient)
%cs_attribute(blcelc,iPcNewCamera,bool,CollisionDetection)
%cs_attribute(blcelc,iPcNewCamera,float,CollisionSpringCoefficient)
%cs_attribute(blcelc,iPcNewCamera,float,TransitionSpringCoefficient)
%cs_multi_attr_writeonly(blcelc,iPcNewCamera,TransitionCutoffDistance,SetTransitionCutoffDistance)
%cs_attribute(blcelc,iPcNewCamera,float,TransitionCutoffOriginDistance,GetTransitionCutoffOriginDistance)
%cs_attribute(blcelc,iPcNewCamera,float,TransitionCutoffTargetDistance,GetTransitionCutoffTargetDistance)
%cs_attribute(blcelc,iPcNewCamera,size_t,CurrentCameraModeIndex,GetCurrentCameraModeIndex)
%cs_multi_attr_readonly(blcelc,iPcNewCamera,CameraMode,GetCameraMode)

/* iPcProperties (include/propclass/prop.h) */
%cs_multi_attr_writeonly(blcelc,iPcProperties,Property,SetProperty)
%cs_multi_attr(blcelc,iPcProperties,PropertyIndex,GetPropertyIndex,SetPropertyIndex)
%cs_attribute(blcelc,iPcProperties,size_t,PropertyCount,GetPropertyCount)

/* iPcMeshSelect (include/propclass/meshsel.h) */
%cs_attribute_writeonly(blcelc,iPcMeshSelect, iPcCamera*, Camera, SetCamera)
%cs_multi_attr(blcelc,iPcMeshSelect,MouseButtons,GetMouseButtons,SetMouseButtons)
%cs_attribute(blcelc,iPcMeshSelect,bool,GlobalSelection,HasGlobalSelection,SetGlobalSelection)
%cs_attribute(blcelc,iPcMeshSelect,bool,FollowMode,HasFollowMode,SetFollowMode)
%cs_attribute(blcelc,iPcMeshSelect,bool,FollowAlwaysMode,HasFollowAlwaysMode,SetFollowAlwaysMode)
%cs_attribute(blcelc,iPcMeshSelect,bool,DragMode,HasDragMode,SetDragMode)
%cs_attribute(blcelc,iPcMeshSelect,bool,SendmoveEvent,HasSendmoveEvent,SetSendmoveEvent)
%cs_attribute(blcelc,iPcMeshSelect,bool,SendupEvent,HasSendupEvent,SetSendupEvent)
%cs_attribute(blcelc,iPcMeshSelect,bool,SenddownEvent,HasSenddownEvent,SetSenddownEvent)
%cs_attribute(blcelc,iPcMeshSelect,float,MaxSelectionDistance)

/* iPcGravity (include/propclass/gravity.h) */
%cs_attribute(blcelc,iPcGravity,iCollider*,GravityCollider,GetGravityCollider)
%cs_attribute(blcelc,iPcGravity,iPcMovable*,Movable)
%cs_attribute(blcelc,iPcGravity,iPcSolid*,Solid)
%cs_attribute(blcelc,iPcGravity,float,Weight)
%cs_attribute(blcelc,iPcGravity,bool,Resting,IsResting)
%cs_attribute(blcelc,iPcGravity,bool,Active,IsActive,SetActive)

/* iCelMapFile (include/propclass/zone.h) */
%cs_attribute(blcelc,iCelMapFile,const char*,Name)
%cs_attribute(blcelc,iCelMapFile,const char*,Path)
%cs_attribute(blcelc,iCelMapFile,const char*,File)
%cs_attribute(blcelc,iCelMapFile,const char*,SectorName)

/* iCelRegion (include/propclass/zone.h) */
%cs_attribute(blcelc,iCelRegion,const char*,Name,GetName)
%cs_attribute(blcelc,iCelRegion,const char*,CachePath)
%cs_attribute(blcelc,iCelRegion,size_t,MapFileCount,GetMapFileCount)

/* iCelZone (include/propclass/zone.h) */
%cs_attribute(blcelc,iCelZone,const char*,Name,GetName)
%cs_attribute(blcelc,iCelZone,size_t,RegionCount,GetRegionCount)

/* iPcZoneManager (include/propclass/zone.h) */
%cs_attribute(blcelc,iPcZoneManager,bool,ColliderWrappers,IsColliderWrappers)
%cs_attribute(blcelc,iPcZoneManager,int,LoadingMode)
%cs_attribute(blcelc,iPcZoneManager,size_t,ZoneCount,GetZoneCount)
%cs_attribute(blcelc,iPcZoneManager,size_t,RegionCount,GetRegionCount)
%cs_attribute(blcelc,iPcZoneManager,const char *,LastStartRegionName,GetLastStartRegionName)
%cs_attribute(blcelc,iPcZoneManager,const char *,LastStartName,GetLastStartName)

/* iPcHover (include/propclass/hover.h) */
%cs_attribute_writeonly(blcelc,iPcHover, float, HeightBeamCutoff, SetHeightBeamCutoff)
%cs_attribute_writeonly(blcelc,iPcHover, float, AngularBeamOffset, SetAngularBeamOffset)
%cs_attribute_writeonly(blcelc,iPcHover, float, AngularCutoffHeight, SetAngularCutoffHeight)
%cs_attribute_writeonly(blcelc,iPcHover, float, AngularCorrectionStrength, SetAngularCorrectionStrength)
%cs_multi_attr_writeonly(blcelc,iPcHover,Factors,SetFactors)
%cs_attribute_writeonly(blcelc,iPcHover, float, HoverHeight, SetHoverHeight)
%cs_attribute(blcelc,iPcHover,float,Height,GetHeight)

/* iPcSimpleCamera (include/propclass/simpcam.h) */
%cs_attribute_writeonly(blcelc,iPcSimpleCamera, bool, DrawMesh, SetDrawMesh)
%cs_multi_attr_writeonly(blcelc,iPcSimpleCamera,CameraOffset,SetCameraOffset)
%cs_attribute_writeonly(blcelc,iPcSimpleCamera, const csVector3&, LookAtOffset, SetLookAtOffset)
%cs_attribute_writeonly(blcelc,iPcSimpleCamera, iPcMesh*, Mesh, SetMesh)

/* iPcProjectile (include/propclass/projectile.h) */
%cs_attribute(blcelc,iPcProjectile,bool,Moving,IsMoving)

/* iPcLinearMovement (include/propclass/linmove.h) */
%cs_attribute(blcelc,iPcLinearMovement,iPcMesh*,Anchor)
%cs_multi_attr(blcelc,iPcLinearMovement,AngularVelocity,GetAngularVelocity,SetAngularVelocity)
%cs_attribute_writeonly(blcelc,iPcLinearMovement, float, Speed, SetSpeed)
%cs_attribute(blcelc,iPcLinearMovement,const csVector3,Velocity)
%cs_multi_attr_writeonly(blcelc,iPcLinearMovement,SoftDRData,SetSoftDRData)
%cs_multi_attr(blcelc,iPcLinearMovement,FullPosition,GetFullPosition,SetFullPosition)
%cs_multi_attr(blcelc,iPcLinearMovement,Position,GetPosition,SetPosition)
%cs_attribute(blcelc,iPcLinearMovement,float,YRotation,GetYRotation)
%cs_multi_attr(blcelc,iPcLinearMovement,Path,IsPath,SetPath)
%cs_attribute(blcelc,iPcLinearMovement,iSector*,Sector,GetSector)
%cs_attribute_writeonly(blcelc,iPcLinearMovement, float, PathTime, SetPathTime)
%cs_attribute_writeonly(blcelc,iPcLinearMovement, float, PathSpeed, SetPathSpeed)
%cs_multi_attr_writeonly(blcelc,iPcLinearMovement,PathAction,SetPathAction)
%cs_attribute_writeonly(blcelc,iPcLinearMovement, const char *, PathSector, SetPathSector)
%cs_attribute(blcelc,iPcLinearMovement,bool,OnGround,IsOnGround,SetOnGround)
%cs_attribute_writeonly(blcelc,iPcLinearMovement, bool, HugGround, SetHugGround)
%cs_attribute_writeonly(blcelc,iPcLinearMovement, float, DeltaLimit, SetDeltaLimit)
%cs_attribute(blcelc,iPcLinearMovement,float,Gravity)
%cs_attribute(blcelc,iPcLinearMovement,csVector3,PortalDisplacement,GetPortalDisplacement)

/* iPcCommandInput (include/propclass/input.h) */
%cs_attribute_writeonly(blcelc,iPcCommandInput, bool, SendTrigger, SetSendTrigger)
%cs_attribute(blcelc,iPcCommandInput,bool,SendTriggerEnabled,IsSendTriggerEnabled)
%cs_attribute(blcelc,iPcCommandInput,bool,CookedMode)

/* iPcCharacteristics (include/propclass/chars.h) */

/* iPcBillboard (include/propclass/billboard.h) */
%cs_attribute(blcelc,iPcBillboard,const char*,BillboardName)
%cs_attribute(blcelc,iPcBillboard,iBillboard*,Billboard,GetBillboard)

/* iPcRegion (include/propclass/region.h) */
%cs_multi_attr(blcelc,iPcRegion,WorldFile,GetWorldFile,SetWorldFile)
%cs_attribute(blcelc,iPcRegion,const char*,WorldDir,GetWorldDir)
%cs_attribute(blcelc,iPcRegion,const char*,RegionName)
%cs_multi_attr_readonly(blcelc,iPcRegion,StartSector,GetStartSector)
%cs_multi_attr_readonly(blcelc,iPcRegion,StartPosition,GetStartPosition)

/* iCelBlLayer (include/behaviourlayer/bl.h) */
%cs_attribute(blcelc,iCelBlLayer,const char*,Name,GetName)

/* iCelParameterBlock (include/behaviourlayer/behave.h) */
%cs_attribute(blcelc,iCelParameterBlock,size_t,ParameterCount,GetParameterCount)

/* iCelBehaviour (include/behaviourlayer/behave.h) */
%cs_attribute(blcelc,iCelBehaviour,const char*,Name,GetName)
%cs_attribute(blcelc,iCelBehaviour,iCelBlLayer*,BehaviourLayer,GetBehaviourLayer)

/* CEL_CELTOOL_EXPORT (include/celtool/stdpcimp.h) */

/* celVariableParameterBlock (include/celtool/stdparams.h) */
%cs_multi_attr_writeonly(blcelc,celVariableParameterBlock,ParameterDef,SetParameterDef)
%cs_attribute(blcelc,celVariableParameterBlock,size_t,ParameterCount,GetParameterCount)

/* celOneParameterBlock (include/celtool/stdparams.h) */
%cs_multi_attr_writeonly(blcelc,celOneParameterBlock,ParameterDef,SetParameterDef)
%cs_attribute(blcelc,celOneParameterBlock,size_t,ParameterCount,GetParameterCount)
/*
Total attributes: 569
Total possible lists: 15
Total iterators: 2
Total possible purelists: 2
Total conflicting methods: 7
Total unhandled methods: 106
  Attribute name starts with a digit 0
  Setter with too many pars 0
  Setter with no pars 1
  Getter with pars 25
  Returntype with commas 0
  Too many setters 0
  Getter with inputs 80
  Write only multiattr 0
  Too many get and set methods 0
  Too many setter methods 0
  Too many getter methods 0
  No getter or setter 0
  Too many weird methods 0
*/
