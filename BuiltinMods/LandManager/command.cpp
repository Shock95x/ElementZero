#include <economy.h>
#include <command.h>
#include <Command/Command.h>
#include <Command/CommandOrigin.h>
#include <Command/CommandOutput.h>
#include <Command/CommandRegistry.h>

#include "settings.h"

DEF_LOGGER("LandManager");

CommandSelector<Player> cmd_player;

void reset(bool hard = true) {
  pointA = Vector3();
  pointB = Vector3();
  if (hard) action = Action::None;
}

class LandManagerCommand : public Command {
public:
  Action target_action;
  CommandSelector<Player> target_player;

  LandManagerCommand() {}

  void execute(CommandOrigin const &origin, CommandOutput &output) {
    action     = target_action;
    cmd_player = target_player;
    std::ostringstream re;

    if (action == Action::Create) {
      reset(false);
      output.success("[Zonas] Selecciona el punto inicial o sal con '/land exit'");
    } else if (action == Action::Buy) {
      // If we havent selected two points yet, return
      if (!pointA.init || !pointB.init) {
        output.error("[Zonas] Debes seleccionar dos puntos primero.");
        return;
      }

      // Try to get the player instance and just reuse it
      auto player    = (Player *) origin.getEntity();
      auto pInstance = LandManager::GetPlayerInstance(player);

      if (!pInstance) {
        output.error("[Zonas] Error al realizar accion. Consulte al administrador.");
        return;
      }

      // Check if we have enough money
      auto balance = Mod::Economy::GetBalance(player);
      int price    = pointA.Volume(pointB) * settings.blockPrice;

      if (price > balance) {
        output.success("[Zonas] No cuentas con suficiente dinero.");
        return;
      }

      // Check if we reached the limit of land we can own
      auto limit = LandManager::ReachedLimit(*pInstance);

      if (limit) {
        output.error(*limit);
        return;
      }

      // Check if we are overlapping any other land
      auto overlap = LandManager::Overlaps(*pInstance, pointA, pointB);

      if (overlap) {
        output.error(*overlap);
        return;
      }

      // Finally, try and buy the land
      Mod::Economy::Transaction ecoTrans;
      LandManager::Transaction landTrans;

      auto err = Mod::Economy::UpdateBalance(player, balance - price, "Land Bought.");
      if (err) {
        output.error("[Zonas] Ocurrio un error al realizar la compra, intente de nuevo.");
        return;
      }

      auto err2 = LandManager::BuyLand(*pInstance, pointA, pointB);
      if (err2) {
        output.error("[Zonas] Ocurrio un error al realizar la compra, intente de nuevo.");
        return;
      }

      ecoTrans.Commit();
      landTrans.Commit();

      output.success("[Zonas] Has comprado la zona.");

      reset();
    } else if (action == Action::Sell) {
      auto player    = (Player *) origin.getEntity();
      auto pInstance = LandManager::GetPlayerInstance(player);
      Vec3 pos       = origin.getWorldPosition();

      std::string sold = LandManager::SellLand(*pInstance, Vector3(pos.x, pos.y, pos.z));

      output.success(sold);
    } else if (action == Action::Give) {
      auto &playerdb = Mod::PlayerDatabase::GetInstance().GetData();
      auto results   = target_player.results(origin);

      if (results.count() > 0) {
        auto first = *(results.begin());
        auto it    = playerdb.find(first);
        if (it != playerdb.end()) {
          Vec3 pos = origin.getWorldPosition();

          std::string give = LandManager::GiveLand(*it, Vector3(pos.x, pos.y, pos.z));

          output.success(give);
        }
      } else {
        output.error("[Zonas] No se encontro el jugador objetivo especificado.");
      }
    } else if (action == Action::Exit) {
      reset();
    }
  }

  static void setup(CommandRegistry *registry) {
    using namespace commands;

    registry->registerCommand(
        "land", "Administracion de zonas", CommandPermissionLevel::Any, CommandFlagCheat, CommandFlagNone);

    addEnum<Action>(
        registry, "land-option",
        {{"create", Action::Create}, {"buy", Action::Buy}, {"sell", Action::Sell}, {"exit", Action::Exit}});

    addEnum<Action>(registry, "land-option-give", {{"give", Action::Give}});

    registry->registerOverload<LandManagerCommand>(
        "land", mandatory<CommandParameterDataType::ENUM>(&LandManagerCommand::target_action, "action", "land-option"));
    registry->registerOverload<LandManagerCommand>(
        "land",
        mandatory<CommandParameterDataType::ENUM>(&LandManagerCommand::target_action, "action", "land-option-give"),
        mandatory(&LandManagerCommand::target_player, "target"));
  }
};

void initCommand(CommandRegistry *registry) { LandManagerCommand::setup(registry); }