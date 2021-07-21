#include <Actor/Player.h>
#include <Core/Minecraft.h>
#include <Container/SimpleContainer.h>
#include <Command/CommandOutput.h>
#include <Level/Level.h>
#include <Item/Item.h>
#include <Net/NetworkIdentifier.h>
#include <Net/ServerNetworkHandler.h>
#include <RakNet/RakPeer.h>

#include <modutils.h>

#include <base/base.h>

#include "loader.h"

template <typename Holder> struct ValueHolder {
  Holder value;
  ~ValueHolder() {}
  operator Holder() const { return value; }
};

#pragma region Player

// Actor::Actor
SynchedActorData &Actor::getEntityData() const { return direct_access<SynchedActorData>(this, 384); }

// Actor::getArmorContainer
SimpleContainer &Actor::getEquipmentContainer() const { return direct_access<SimpleContainer>(this, 1648); }

// Player::Player
class EnderChestContainer * Player::getEnderChestContainer() const {
    return direct_access<std::unique_ptr<class EnderChestContainer>>(this, 4360).get();
}

// Player::getSelectedItem
PlayerInventory &Player::getInventory() const { return *direct_access<PlayerInventory *>(this, 3208); }

// Player::Player
Certificate &Player::getCertificate() const { return *direct_access<class Certificate *>(this, 3016); }

// ServerPlayer::ServerPlayer
NetworkIdentifier const &Player::getNetworkIdentifier() const {
  return direct_access<class NetworkIdentifier const>(this, 2712);
}
// AddPlayerPacket::AddPlayerPacket
std::string &Player::getDeviceId() const { return direct_access<std::string>(this, 8352); }

// ServerNetworkHandler::_createNewPlayer
std::string &Player::getClientPlatformId() const { return direct_access<std::string>(this, 3024); }

// ServerNetworkHandler::_createNewPlayer
std::string &Player::getPlatformOfflineId() const { return direct_access<std::string>(this, 2960); }

// Player::getPlatformOnlineId
std::string &Player::getClientPlatformOnlineId() const { return direct_access<std::string>(this, 3856); }


#pragma endregion

void NetworkIdentifier::kick(std::string const &reason) const {
  LocateService<ServerNetworkHandler>()->disconnectClient(*this, 0, reason, reason.empty());
}

void Player::kick() { LocateService<ServerNetworkHandler>()->forceDisconnectClient(this, true); }

std::string &ServerNetworkHandler::GetMotd() { return direct_access<std::string>(this, 608); }

void CommandOutput::success() { direct_access<bool>(this, 40) = true; }

uint64_t Level::GetServerTick() {
  return CallServerClassMethod<ValueHolder<uint64_t>>("?getCurrentServerTick@Level@@UEBA?BUTick@@XZ", this);
}

// RaidBossComponent::_sendBossEvent
PacketSender &Level::getPacketSender() const { return *direct_access<PacketSender *>(this, 2400); }

// ServerLevel::initialize
LevelDataWrapper &Level::GetLevelDataWrapper() { return direct_access<LevelDataWrapper>(this, 584); }

template <> Minecraft *LocateService<Minecraft>() {
  return *GetServerSymbol<Minecraft *>("?mGame@ServerCommand@@1PEAVMinecraft@@EA");
}

template <> ServerNetworkHandler *LocateService<ServerNetworkHandler>() {
  return LocateService<Minecraft>()->getServerNetworkHandler();
}

template <> NetworkHandler *LocateService<NetworkHandler>() {
    return &LocateService<Minecraft>()->getNetworkHandler();
}

template <> MinecraftCommands *LocateService<MinecraftCommands>() { return &LocateService<Minecraft>()->getCommands(); }

// MinecraftCommands *Minecraft::getCommands() { return direct_access<MinecraftCommands *>(this, 176); }

// Item::allowOffhand
bool Item::getAllowOffhand() const { return direct_access<char>(this, 290) & 0x40; }

RakNet::SystemAddress NetworkIdentifier::getRealAddress() const {
  return LocateService<RakNet::RakPeer>()->GetSystemAddressFromGuid(guid);
}

TClasslessInstanceHook(bool, "?loadLevelData@DBStorage@@UEAA_NAEAVLevelData@@@Z", void *data) {
  auto &path = direct_access<std::string>(this, 160);
  worldHook(std::filesystem::weakly_canonical(path));
  return original(this, data);
}