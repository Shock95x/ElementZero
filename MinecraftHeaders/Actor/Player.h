#pragma once

#include <vector>

#include <hook.h>

#include "Actor.h"
#include "Mob.h"
#include "../Core/ExtendedCertificate.h"
#include "../Container/PlayerInventory.h"
#include "../Container/EnderChestContainer.h"
#include "../Math/Vec3.h"
#include "../Math/BlockPos.h"
#include "../Core/AutomaticID.h"
#include "../Core/VanillaDimensions.h"
#include "../Command/CommandPermissionLevel.h"
#include "../dll.h"

class Packet;
class ServerPlayer;
class NetworkIdentifier;
class Dimension;
enum class StructureFeatureType;
enum class AbilitiesIndex;
enum class BedSleepingResult;
enum class GameType;
enum class CooldownType;
enum class ClientPlayMode;
enum class MovementEventType;
enum class PlayerUISlot;

#ifndef BASEAPI
#  define BASEAPI __declspec(dllimport)
#endif

class Player : public Mob {
public:
    class PlayerSpawnPoint {
    public:
      inline static BlockPos InvalidSpawnPos = BlockPos::MIN;
      BlockPos spawn_block = InvalidSpawnPos, spawn_pos = InvalidSpawnPos;
      AutomaticID<class Dimension, int> dim = VanillaDimensions::Undefined;

      inline PlayerSpawnPoint()        = default;
      inline PlayerSpawnPoint &operator=(PlayerSpawnPoint &&) = default;
      inline bool hasSpawnPoint() const { return spawn_block != BlockPos::MIN && dim != VanillaDimensions::Undefined; }
      inline void invalidate() { *this = {}; }
      inline bool isValid() const { return dim != VanillaDimensions::Undefined; }
      inline void setSpawnPoint(BlockPos spawn_block, AutomaticID<class Dimension, int> dim, BlockPos spawn_pos) {
        this->spawn_block = spawn_block;
        this->spawn_pos   = spawn_pos;
        this->dim         = dim;
      }
      inline void setSpawnPoint(BlockPos spawn_block, AutomaticID<class Dimension, int> dim) {
        setSpawnPoint(spawn_block, dim, spawn_block);
      }
    };

    inline ServerPlayer *asServerPlayer() const noexcept {
      return const_cast<ServerPlayer *>(reinterpret_cast<ServerPlayer const *>(this));
    }

    MCAPI virtual void prepareRegion(class ChunkSource &);
    MCAPI virtual void destroyRegion(void);
    MCAPI virtual void suspendRegion(void);
    MCAPI virtual void _fireWillChangeDimension(void);
    MCAPI virtual void _fireDimensionChanged(void);
    MCAPI virtual void changeDimensionWithCredits(class AutomaticID<class Dimension, int>);
    MCAPI virtual int tickWorld(struct Tick const &);
    MCAPI virtual void frameUpdate(class FrameUpdateContextBase &);
    MCAPI virtual std::vector<class ChunkPos> const &getTickingOffsets(void) const;
    MCAPI virtual void moveView(void);
    MCAPI virtual void setName(std::string const &);
    MCAPI virtual void checkMovementStats(class Vec3 const &);
    MCAPI virtual class StructureFeature *getCurrentStructureFeature() const;
    MCAPI virtual bool isAutoJumpEnabled() const;
    MCAPI virtual void respawn(void);
    MCAPI virtual void resetRot();
    MCAPI virtual void resetPos(bool);
    MCAPI virtual bool isInTrialMode();
    MCAPI virtual bool hasResource(int);
    MCAPI virtual void completeUsingItem(void);
    MCAPI virtual void setPermissions(enum CommandPermissionLevel);
    MCAPI virtual void startDestroying(void);
    MCAPI virtual void stopDestroying(void);
    MCAPI virtual void openPortfolio();
    MCAPI virtual void openBook(int, bool, int, BlockActor *);
    MCAPI virtual void openTrading(struct ActorUniqueID const &, bool);
    MCAPI virtual bool canOpenContainerScreen(void);
    MCAPI virtual void openChalkboard(class ChalkboardBlockActor *, bool);
    MCAPI virtual void openNpcInteractScreen(Actor &);
    MCAPI virtual void openInventory(void);
    MCAPI virtual void displayChatMessage(std::string const &, std::string const &);
    MCAPI virtual void displayClientMessage(std::string const &);
    MCAPI virtual void displayTextObjectMessage(class TextObjectRoot const &);
    MCAPI virtual void displayTextObjectWhisperMessage(class TextObjectRoot const &, std::string const &, std::string const &);
    MCAPI virtual void displayWhisperMessage(std::string const &, std::string const &, std::string const &, std::string const &);
    MCAPI virtual enum BedSleepingResult startSleepInBed(class BlockPos const &);
    MCAPI virtual void stopSleepInBed(bool, bool);
    MCAPI virtual bool canStartSleepInBed(void);
    MCAPI virtual int getSleepTimer(void) const;
    MCAPI virtual int getPreviousTickSleepTimer(void) const;
    MCAPI virtual void openSign(BlockPos const &);
    MCAPI virtual void playEmote(std::string const &);
    MCAPI virtual bool isLocalPlayer() const;
    MCAPI virtual bool isHostingPlayer(void) const;
    MCAPI virtual bool isLoading(void) const;
    MCAPI virtual bool isPlayerInitialized(void) const;
    MCAPI virtual void stopLoading();
    MCAPI virtual void registerTrackedBoss(struct ActorUniqueID);
    MCAPI virtual void unRegisterTrackedBoss(struct ActorUniqueID);
    MCAPI virtual void setPlayerGameType(enum GameType);
    MCAPI virtual void _crit(class Actor &);
    MCAPI virtual class IMinecraftEventing *getEventing(void) const;
    MCAPI virtual unsigned char getUserId();
    MCAPI virtual void sendEventPacket(class EventPacket &) const;
    MCAPI virtual void addExperience(int);
    MCAPI virtual void addLevels(int);
    MCAPI virtual void setContainerData(class IContainerManager &, int, int);
    MCAPI virtual void slotChanged(
    MCAPI     class IContainerManager &, class Container &, int, class ItemStack const &, class ItemStack const &, bool);
    MCAPI virtual void inventoryChanged(class Container &, int, class ItemStack const &, class ItemStack const &, bool);
    MCAPI virtual void refreshContainer(class IContainerManager &);
    MCAPI virtual void deleteContainerManager(void);
    MCAPI virtual void setFieldOfViewModifier(float);
    MCAPI virtual bool isPositionRelevant(class AutomaticID<class Dimension, int>, class BlockPos const &);
    MCAPI virtual bool isEntityRelevant(class Actor const &);
    MCAPI virtual bool isTeacher(void) const;
    MCAPI virtual void onSuspension(void);
    MCAPI virtual void onLinkedSlotsChanged(void);
    MCAPI virtual void startCooldown(class Item const *);
    MCAPI virtual int getItemCooldownLeft(enum CooldownType) const;
    MCAPI virtual bool isItemInCooldown(enum CooldownType) const;
    MCAPI virtual void sendInventoryTransaction(class InventoryTransaction const &) const;
    MCAPI virtual void sendComplexInventoryTransaction(std::unique_ptr<class ComplexInventoryTransaction>) const;
    MCAPI virtual class PlayerEventCoordinator &getPlayerEventCoordinator(void);
    MCAPI virtual class MoveInputHandler *getMoveInputHandler();
    MCAPI virtual enum InputMode getInputMode(void) const;
    MCAPI virtual enum ClientPlayMode getPlayMode(void) const;
    MCAPI virtual void reportMovementTelemetry(enum MovementEventType);
    MCAPI virtual void onMovePlayerPacketNormal(class Vec3 const &, class Vec2 const &, float);

    MCAPI bool isInRaid(void) const;
    MCAPI bool isSurvival(void) const;
    MCAPI bool isInCreativeMode(void) const;
    MCAPI bool isHiddenFrom(class Mob &) const;

    MCAPI bool canBeSeenOnMap(void) const;
    MCAPI bool canUseOperatorBlocks(void) const;
    MCAPI bool canDestroy(class Block const &) const;
    MCAPI bool canUseAbility(enum AbilitiesIndex) const;

    MCAPI class Agent *getAgent(void) const;
    MCAPI int getDirection(void) const;
    MCAPI int getXpNeededForNextLevel(void) const;
    MCAPI float getDestroySpeed(class Block const &) const;
    MCAPI float getDestroyProgress(class Block const &) const;
    MCAPI class ItemStack const &getSelectedItem(void) const;
    MCAPI enum GameType getPlayerGameType(void) const;
    MCAPI class ItemStack const &getCurrentActiveShield(void) const;
    MCAPI class SerializedSkin & getSkin(void);
    MCAPI class SerializedSkin const & getSkin(void) const;
    MCAPI class PlayerInventory &getSupplies(void);
    MCAPI class BlockPos const &getSpawnPosition(void) const;
    MCAPI unsigned char getClientSubId(void) const;

    MCAPI void updateTeleportDestPos(void);
    MCAPI void updateInventoryTransactions(void);
    MCAPI void updateSkin(class SerializedSkin const &, int);

    MCAPI void setAgent(class Agent *);
    MCAPI void setRespawnPosition(class BlockPos const &, bool);
    MCAPI void setBedRespawnPosition(class BlockPos const &);
    MCAPI void setPlayerUIItem(enum PlayerUISlot, class ItemStack const &);
    MCAPI void setContainerManager(class std::shared_ptr<class IContainerManager>);

    MCAPI void eat(int, float);
    MCAPI void startUsingItem(class ItemStack const &, int);
    MCAPI void stopUsingItem(void);
    MCAPI void releaseUsingItem(void);
    MCAPI void stopGliding(void);
    MCAPI void resetPlayerLevel(void);
    MCAPI void handleJumpEffects(void);
    MCAPI bool take(class Actor &, int, int);
    MCAPI void updateTrackedBosses(void);
    MCAPI void causeFoodExhaustion(float);
    MCAPI bool checkNeedAutoJump(float, float);
    MCAPI void clearRespawnPosition(void);
    MCAPI void recheckSpawnPosition(void);

    MCAPI NetworkIdentifier const & getClientId(void) const;

    inline void sendNetworkPacket(class Packet &pkt) const {
        (*(void (__fastcall **) (const Player*, Packet&)) (*(__int64 *) this + 3472))(this, pkt);
    }

  BUILD_ACCESS_COMPAT(PlayerInventory &, Inventory);
  BUILD_ACCESS_COMPAT(class EnderChestContainer *, EnderChestContainer);
  BUILD_ACCESS_COMPAT(class Certificate &, Certificate);
  BUILD_ACCESS_COMPAT(class NetworkIdentifier const &, NetworkIdentifier);
  BUILD_ACCESS_COMPAT(std::string &, DeviceId);
  BUILD_ACCESS_COMPAT(std::string &, ClientPlatformId);
  BUILD_ACCESS_COMPAT(std::string &, PlatformOfflineId);
  BUILD_ACCESS_COMPAT(std::string &, ClientPlatformOnlineId);
  BASEAPI void kick();
};