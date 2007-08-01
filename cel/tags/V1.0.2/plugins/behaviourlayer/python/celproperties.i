
/* iCelConsoleCommand (include/tools/celconsole.h) */
%cs_attribute(blcelc,iCelConsoleCommand,const char*,Command,GetCommand)
%cs_attribute(blcelc,iCelConsoleCommand,const char*,Description,GetDescription)

/* iCelConsole (include/tools/celconsole.h) */
%cs_attribute(blcelc,iCelConsole,iConsoleInput*,InputConsole,GetInputConsole)
%cs_attribute(blcelc,iCelConsole,iConsoleOutput*,OutputConsole,GetOutputConsole)

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

/* iCelPropertyClass (include/physicallayer/propclas.h) */
%cs_attribute(blcelc,iCelPropertyClass,const char*,Name,GetName)
%cs_attribute(blcelc,iCelPropertyClass,const char*,Tag)
%cs_attribute(blcelc,iCelPropertyClass,iCelEntity*,Entity)
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

/* iCelEntityList (include/physicallayer/entity.h) */
%cs_attribute(blcelc,iCelEntityList,size_t,Count,GetCount)
%cs_attribute(blcelc,iCelEntityList,csPtr<iCelEntityIterator>,Iterator,GetIterator)

/* iCelEntityIterator (include/physicallayer/entity.h) */
%cs_attribute(blcelc,iCelEntityIterator,bool,Next,HasNext)

/* iPcCraftController (include/propclass/craft.h) */
%cs_attribute_writeonly(blcelc,iPcCraftController, float, AccTurn, SetAccTurn)
%cs_attribute_writeonly(blcelc,iPcCraftController, float, AccPitch, SetAccPitch)
%cs_attribute_writeonly(blcelc,iPcCraftController, float, MaxTurn, SetMaxTurn)
%cs_attribute_writeonly(blcelc,iPcCraftController, float, MaxPitch, SetMaxPitch)
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

/* iPcSoundListener (include/propclass/sound.h) */
%cs_attribute(blcelc,iPcSoundListener,iSndSysListener*,SoundListener,GetSoundListener)

/* iPcSoundSource (include/propclass/sound.h) */
%cs_attribute(blcelc,iPcSoundSource,iSndSysSource*,SoundSource,GetSoundSource)
%cs_attribute(blcelc,iPcSoundSource,const char*,SoundName)

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

/* iPcActorMove (include/propclass/actormove.h) */
%cs_attribute(blcelc,iPcActorMove,bool,MovingForward,IsMovingForward,Forward)
%cs_attribute(blcelc,iPcActorMove,bool,MovingBackward,IsMovingBackward,Backward)
%cs_attribute(blcelc,iPcActorMove,bool,StrafingLeft,IsStrafingLeft,StrafeLeft)
%cs_attribute(blcelc,iPcActorMove,bool,StrafingRight,IsStrafingRight,StrafeRight)
%cs_attribute(blcelc,iPcActorMove,bool,RotatingLeft,IsRotatingLeft,RotateLeft)
%cs_attribute(blcelc,iPcActorMove,bool,RotatingRight,IsRotatingRight,RotateRight)
%cs_attribute(blcelc,iPcActorMove,bool,Running,IsRunning,Run)
%cs_attribute(blcelc,iPcActorMove,bool,AutoRunning,IsAutoRunning,AutoRun)
%cs_attribute(blcelc,iPcActorMove,bool,MouseMoveEnabled,IsMouseMoveEnabled)
%cs_attribute(blcelc,iPcActorMove,bool,MouseMoveInverted,IsMouseMoveInverted,SetMouseMoveInverted)
%cs_multi_attr_writeonly(blcelc,iPcActorMove,MouseMoveSpeed,SetMouseMoveSpeed)
%cs_attribute(blcelc,iPcActorMove,float,MouseMoveSpeedX,GetMouseMoveSpeedX)
%cs_attribute(blcelc,iPcActorMove,float,MouseMoveSpeedY,GetMouseMoveSpeedY)
%cs_attribute(blcelc,iPcActorMove,float,MovementSpeed)
%cs_attribute(blcelc,iPcActorMove,float,RunningSpeed)
%cs_attribute(blcelc,iPcActorMove,float,RotationSpeed)
%cs_attribute(blcelc,iPcActorMove,float,JumpingVelocity)
%cs_multi_attr_writeonly(blcelc,iPcActorMove,Animation,SetAnimation)

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
%cs_attribute(blcelc,iPcMover,const csVector3&,Up,GetUp)
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
%cs_attribute(blcelc,iPcDefaultCamera,float,YawVelocity)
%cs_attribute(blcelc,iPcDefaultCamera,float,DistanceVelocity)

/* iPcMesh (include/propclass/mesh.h) */
%cs_attribute_writeonly(blcelc,iPcMesh, const char*, Path, SetPath)
%cs_multi_attr(blcelc,iPcMesh,Mesh,GetMesh,SetMesh)
%cs_attribute(blcelc,iPcMesh,CS_DEPRECATED_METHOD  const char*,Action)
%cs_attribute_writeonly(blcelc,iPcMesh, bool, ReverseAction, SetReverseAction)
%cs_multi_attr_writeonly(blcelc,iPcMesh,Animation,SetAnimation)
%cs_attribute(blcelc,iPcMesh,bool,Visible,IsVisible)

/* iPcMovable (include/propclass/move.h) */
%cs_attribute(blcelc,iPcMovable,iPcMesh*,Mesh)

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
%cs_attribute(blcelc,iCelMapFile,const char*,Path)
%cs_attribute(blcelc,iCelMapFile,const char*,File)
%cs_attribute(blcelc,iCelMapFile,const char*,SectorName)

/* iCelRegion (include/propclass/zone.h) */
%cs_attribute(blcelc,iCelRegion,const char*,Name,GetName)
%cs_attribute(blcelc,iCelRegion,const char*,CsRegionName,GetCsRegionName)
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

/* iPcLinearMovement (include/propclass/linmove.h) */
%cs_attribute(blcelc,iPcLinearMovement,iPcMesh*,Anchor)
%cs_multi_attr(blcelc,iPcLinearMovement,AngularVelocity,GetAngularVelocity,SetAngularVelocity)
%cs_attribute_writeonly(blcelc,iPcLinearMovement, float, Speed, SetSpeed)
%cs_multi_attr_writeonly(blcelc,iPcLinearMovement,SoftDRData,SetSoftDRData)
%cs_multi_attr_writeonly(blcelc,iPcLinearMovement,FullPosition,SetFullPosition)
%cs_multi_attr_writeonly(blcelc,iPcLinearMovement,Position,SetPosition)
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
%cs_attribute(blcelc,iPcRegion,iRegion*,Region,GetRegion)

/* iPcProjectile (include/propclass/projectile.h) */
%cs_attribute(blcelc,iPcProjectile,bool,Moving,IsMoving)

/* iPcWheeled (include/propclass/wheeled.h) */
%cs_multi_attr_writeonly(blcelc,iPcWheeled,WheelMesh,SetWheelMesh)
%cs_attribute(blcelc,iPcWheeled,bool,TankMode)
%cs_attribute_writeonly(blcelc,iPcWheeled, float, OuterWheelSteerPreset, SetOuterWheelSteerPreset)
%cs_multi_attr_writeonly(blcelc,iPcWheeled,FrontWheelPreset,SetFrontWheelPreset)
%cs_multi_attr_writeonly(blcelc,iPcWheeled,RearWheelPreset,SetRearWheelPreset)
%cs_attribute(blcelc,iPcWheeled,bool,Accelerating,IsAccelerating,Accelerate)
%cs_attribute(blcelc,iPcWheeled,bool,Braking,IsBraking,Brake)
%cs_attribute(blcelc,iPcWheeled,bool,Handbraking,IsHandbraking,Handbrake)
%cs_attribute(blcelc,iPcWheeled,float,SteerAmount)
%cs_attribute_writeonly(blcelc,iPcWheeled, float, BrakeForce, SetBrakeForce)
%cs_attribute_writeonly(blcelc,iPcWheeled, bool, AutoTransmission, SetAutoTransmission)
%cs_multi_attr(blcelc,iPcWheeled,Gear,GetGear,SetGear)
%cs_attribute(blcelc,iPcWheeled,int,TopGear,GetTopGear)
%cs_attribute_writeonly(blcelc,iPcWheeled, bool, AutoReverse, SetAutoReverse)
%cs_attribute(blcelc,iPcWheeled,iBodyGroup*,BodyGroup,GetBodyGroup)
%cs_attribute(blcelc,iPcWheeled,int,WheelCount,GetWheelCount)

/* iPcDamage (include/propclass/damage.h) */
%cs_attribute(blcelc,iPcDamage,float,Damage)
%cs_attribute(blcelc,iPcDamage,const char*,DamageType)
%cs_attribute(blcelc,iPcDamage,const char*,FallOff)
%cs_multi_attr_writeonly(blcelc,iPcDamage,DamageLocation,SetDamageLocation)
%cs_attribute(blcelc,iPcDamage,const char*,DamageSector,GetDamageSector)
%cs_attribute(blcelc,iPcDamage,const csVector3&,DamagePosition,GetDamagePosition)
%cs_attribute(blcelc,iPcDamage,const char*,DamageSource)

/* iCelBlLayer (include/behaviourlayer/bl.h) */
%cs_attribute(blcelc,iCelBlLayer,const char*,Name,GetName)

/* iCelParameterBlock (include/behaviourlayer/behave.h) */
%cs_attribute(blcelc,iCelParameterBlock,size_t,ParameterCount,GetParameterCount)

/* iCelBehaviour (include/behaviourlayer/behave.h) */
%cs_attribute(blcelc,iCelBehaviour,const char*,Name,GetName)
%cs_attribute(blcelc,iCelBehaviour,iCelBlLayer*,BehaviourLayer,GetBehaviourLayer)

/* celGenericParameterBlock (include/celtool/stdparams.h) */
%cs_multi_attr_writeonly(blcelc,celGenericParameterBlock,ParameterDef,SetParameterDef)
%cs_attribute(blcelc,celGenericParameterBlock,size_t,ParameterCount,GetParameterCount)

/* celVariableParameterBlock (include/celtool/stdparams.h) */
%cs_multi_attr_writeonly(blcelc,celVariableParameterBlock,ParameterDef,SetParameterDef)
%cs_attribute(blcelc,celVariableParameterBlock,size_t,ParameterCount,GetParameterCount)

/* celOneParameterBlock (include/celtool/stdparams.h) */
%cs_multi_attr_writeonly(blcelc,celOneParameterBlock,ParameterDef,SetParameterDef)
%cs_attribute(blcelc,celOneParameterBlock,size_t,ParameterCount,GetParameterCount)
/*
Total attributes: 378
Total possible lists: 15
Total possible purelists: 2
Total unhandled methods: 96
  Attribute name starts with a digit 0
  Setter with too many pars 0
  Setter with no pars 1
  Getter with pars 25
  Returntype with commas 0
  Too many setters 0
  Getter with inputs 70
  Write only multiattr 0
  Too many get and set methods 0
  Too many setter methods 0
  Too many getter methods 0
  No getter or setter 0
  Too many weird methods 0
*/
