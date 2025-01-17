enum class MinecraftPacketIds {
   Login = 0x1,
   PlayStatus = 0x2,
   ServerToClientHandshake = 0x3,
   ClientToServerHandshake = 0x4,
   Disconnect = 0x5,
   ResourcePacksInfo = 0x6,
   ResourcePackStack = 0x7,
	SetTime = 0xa,
	StartGame = 0xb,
	AddPlayer = 0xc,
	AddActor = 0xd,
	RemoveActor = 0xe,
	TakeItemActor = 0x11,
	MoveActorAbsolute = 0x12,
	MovePlayer = 0x13,
	UpdateBlock = 0x15,
	AddPainting = 0x16,
	TickSync = 0x17,
	BlockEvent = 0x1a,
	ActorEvent = 0x1b,
	MobEffect = 0x1c,
	UpdateAttributes = 0x1d,
	InventoryTransaction = 0x1e,
	MobEquipment = 0x1f,
	MobArmorEquipment = 0x20,
	Interact = 0x21,
	BlockPickRequest = 0x22,
	ActorPickRequest = 0x23,
	PlayerAction = 0x24,
	HurtArmor = 0x26,
	SetActorData = 0x27,
	SetActorLink = 0x29,
	SetHealth = 0x2a,
	SetSpawnPosition = 0x2b,
	Animate = 0x2c,
	Respawn = 0x2d,
	ContainerOpen = 0x2e,
	ContainerClose = 0x2f,
	PlayerHotbar = 0x30,
	InventoryContent = 0x31,
	InventorySlot = 0x32,
	ContainerSetData = 0x33,
	CraftingData = 0x34,
	CraftingEvent = 0x35,
	GuiDataPickItem = 0x36,
	AdventureSettings = 0x37,
	BlockActorData = 0x38,
	PlayerInput = 0x39,
	LevelChunk = 0x3a,
	SetCommandsEnabled = 0x3b,
	SetDifficulty = 0x3c,
	ChangeDimension = 0x3d,
	SetPlayerGameType = 0x3e,
	PlayerList = 0x3f,
	Event = 0x41,
	SpawnExperienceOrb = 0x42,
	ClientboundMapItemData = 0x43,
	MapInfoRequest = 0x44,
	ItemFrameDropItem = 0x47,
	GameRulesChanged = 0x48,
	Camera = 0x49,
	BossEvent = 0x4a,
	ShowCredits = 0x4b,
	AvailableCommands = 0x4c,
	CommandRequest = 0x4d,
	CommandBlockUpdate = 0x4e,
	CommandOutput = 0x4f,
	UpdateTrade = 0x50,
	UpdateEquip = 0x51,
	ResourcePackDataInfo = 0x52,
	ResourcePackChunkData = 0x53,
	ResourcePackChunkRequest = 0x54,
	Transfer = 0x55,
	PlaySound = 0x56,
	StopSound = 0x57,
	SetTitle = 0x58,
	AddBehaviorTree = 0x59,
	StructureBlockUpdate = 0x5a,
	ShowStoreOffer = 0x5b,
	PurchaseReceipt = 0x5c,
	PlayerSkin = 0x5d,
	SubClientLogin = 0x5e,
	AutomationClientConnect = 0x5f,
	SetLastHurtBy = 0x60,
	BookEdit = 0x61,
	NpcRequest = 0x62,
	PhotoTransfer = 0x63,
	ModalFormRequest = 0x64,
	ModalFormResponse = 0x65,
	ServerSettingsRequest = 0x66,
	ServerSettingsResponse = 0x67,
	ShowProfile = 0x68,
	SetDefaultGameType = 0x69,
	RemoveObjective = 0x6a,
	SetDisplayObjective = 0x6b,
	SetScore = 0x6c,
	LabTable = 0x6d,
	UpdateBlockSynced = 0x6e,
	MoveActorDelta = 0x6f,
	SetScoreboardIdentity = 0x70,
	SetLocalPlayerAsInitialized = 0x71,
	UpdateSoftEnum = 0x72,
	NetworkStackLatency = 0x73,
	ScriptCustomEvent = 0x75,
	SpawnParticleEffect = 0x76,
	AvailableActorIdentifiers = 0x77,
	NetworkChunkPublisherUpdate = 0x79,
	BiomeDefinitionList = 0x7a,
	LevelSoundEvent = 0x7b,
	LevelEventGeneric = 0x7c,
	LecternUpdate = 0x7d,
	AddEntity = 0x7f,
	ClientCacheStatus = 0x81,
	OnScreenTextureAnimation = 0x82,
	MapCreateLockedCopy = 0x83,
	StructureTemplateDataRequest = 0x84,
	StructureTemplateDataResponse = 0x85,
	ClientCacheBlobStatus = 0x87,
	ClientCacheMissResponse = 0x88,
	EducationSettings = 0x89,
	Emote = 0x8a,
	MultiplayerSettings = 0x8b,
	SettingsCommand = 0x8c,
	AnvilDamage = 0x8d,
	CompletedUsingItem = 0x8e,
	NetworkSettings = 0x8f,
	PlayerAuthInput = 0x90,
	CreativeContent = 0x91,
	PlayerEnchantOptions = 0x92,
	ItemStackRequest = 0x93,
	ItemStackResponse = 0x94,
	PlayerArmorDamage = 0x95,
	CodeBuilder = 0x96,
	UpdatePlayerGameType = 0x97,
	EmoteList = 0x98,
	PositionTrackingDBServerBroadcast = 0x99,
	PositionTrackingDBClientRequest = 0x9a,
	DebugInfo = 0x9b,
	PacketViolationWarning = 0x9c,
	MotionPredictionHints = 0x9d,
	AnimateEntity = 0x9e,
	CameraShake = 0x9f,
	PlayerFog = 0xa0,
	CorrectPlayerMovePrediction = 0xa1,
	ItemComponent = 0xa2,
	FilterText = 0xa3,
	ClientboundDebugRenderer = 0xa4,
	SyncActorProperty = 0xa5,
	AddVolumeEntity = 0xa6,
	RemoveVolumeEntity = 0xa7
}