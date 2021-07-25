#include <algorithm>

#include <Block/BlockSource.h>
#include <Block/Block.h>
#include <Block/BlockLegacy.h>
#include <Actor/Player.h>
#include <Math/BlockPos.h>
#include <Net/NetworkIdentifier.h>
#include <Packet/PlayerActionPacket.h>
#include <Packet/InventoryTransactionPacket.h>
#include <Packet/TextPacket.h>

#include <dllentry.h>
#include <hook.h>
#include <command.h>
#include <audit.h>

#include "settings.h"

std::unique_ptr<SQLite::Database> landDB;
Action action;
Vector3 pointA;
Vector3 pointB;

DEF_LOGGER("LandManager");
DEFAULT_SETTINGS(settings);

void dllenter() {}
void dllexit() {}

void checkAction(Mod::PlayerEntry const &, Mod::PlayerAction const &, Mod::CallbackToken<std::string> &);
void checkInventoryTransaction(
    Mod::PlayerEntry const &, ComplexInventoryTransaction const &, Mod::CallbackToken<std::string> &);

void PreInit() {
  LOGV("[LM] PreInit");

  LandManager::InitDatabase();

  Mod::CommandSupport::GetInstance().AddListener(SIG("loaded"), initCommand);
  Mod::AuditSystem::GetInstance().AddListener(SIG("action"), {Mod::RecursiveEventHandlerAdaptor(checkAction)});
  Mod::AuditSystem::GetInstance().AddListener(
      SIG("inventory_transaction"), {Mod::RecursiveEventHandlerAdaptor(checkInventoryTransaction)});
}

void PostInit() {}

void checkAction(
    Mod::PlayerEntry const &player, Mod::PlayerAction const &pAction, Mod::CallbackToken<std::string> &token) {
  std::ostringstream p;

  switch (pAction.type) {
  case PlayerActionType::START_BREAK:
  case PlayerActionType::CONTINUE_BREAK:
  case PlayerActionType::INTERACT_BLOCK: {
    Vector3 point = Vector3(pAction.pos.x, pAction.pos.y, pAction.pos.z);

    if (action == Action::Create) {
      if (!pointA.init) {
        pointA = point;

        auto packet = TextPacket::createTextPacket<TextPacketType::SystemMessage>(
            player.name,
            "[Zonas] Punto inicial seleccionado, continua seleccionando el punto final o sal con '/land exit'", "");
        player.player->sendNetworkPacket(packet);
      } else {
        pointB = point;

        int vol = pointA.Volume(pointB);

        std::ostringstream text;

        text << "[Zonas] Volumen: " << vol << "U - Precio: " << vol * settings.blockPrice
             << "M. Compra esta zona con '/land buy' o sal con '/land exit'";

        auto packet = TextPacket::createTextPacket<TextPacketType::SystemMessage>(player.name, text.str(), "");
        player.player->sendNetworkPacket(packet);
      }
    }

    bool hasPerm = LandManager::HasPerm(player, point);
    if (!hasPerm) token("Blocked by SpawnProtection");

    break;
  }
  default: break;
  }
}

void checkInventoryTransaction(
    Mod::PlayerEntry const &entry, ComplexInventoryTransaction const &transaction,
    Mod::CallbackToken<std::string> &token) {

  switch (transaction.type) {
  case ComplexInventoryTransaction::Type::ITEM_USE: {
    auto &data = (ItemUseInventoryTransaction const &) transaction;

    switch (data.actionType) {
    case ItemUseInventoryTransaction::Type::USE_ITEM_ON:
    case ItemUseInventoryTransaction::Type::USE_ITEM:
    case ItemUseInventoryTransaction::Type::DESTROY: {
      Vector3 point = Vector3(data.pos.x, data.pos.y, data.pos.z);
      bool hasPerm  = LandManager::HasPerm(entry, point);

      if (action == Action::Create || !hasPerm) {
        data.onTransactionError(*entry.player, InventoryTransactionError::Unexcepted);
        token("Blocked by SpawnProtection");
      }

    } break;
    }
  } break;

    /*case ComplexInventoryTransaction::Type::ITEM_USE_ON_ACTOR: {
      auto &data    = (ItemUseOnActorInventoryTransaction const &) transaction;
      auto composed = data.playerPos + data.clickPos;
      data.onTransactionError(*entry.player, InventoryTransactionError::Unexcepted);
      token("Blocked by SpawnProtection");
    } break;*/

  default: break;
  }
}
