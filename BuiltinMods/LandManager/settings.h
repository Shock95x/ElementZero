#pragma once

#include <algorithm>
#include <yaml.h>
#include <string>
#include <optional>
#include <nlohmann/json.hpp>
#include <boost/scope_exit.hpp>
#include <SQLiteCpp/SQLiteCpp.h>

#include <log.h>
#include <playerdb.h>

struct Settings {
  int blockPrice = 1;
  int limit      = 3;

  std::string database = "landmanager.db";

  template <typename IO> static inline bool io(IO f, Settings &settings, YAML::Node &node) {
    return f(settings.blockPrice, node["blockPrice"]) && f(settings.limit, node["limit"]);
  }
};

inline Settings settings;

struct Vector3 {
  bool init = false;
  int X, Y, Z;

  inline Vector3(void) { init = false; }
  inline Vector3(const int x, const int y, const int z) {
    init = true;
    X    = x;
    Y    = y;
    Z    = z;
  }

  inline Vector3 operator+(const Vector3 &A) const { return Vector3(X + A.X, Y + A.Y, Z + A.Z); }

  inline Vector3 operator+(const int A) const { return Vector3(X + A, Y + A, Z + A); }

  inline int Dot(const Vector3 &A) const { return A.X * X + A.Y * Y + A.Z * Z; }

  inline int Volume(const Vector3 &A) const {
    int x2 = abs(A.X - X) + 1;
    int y2 = abs(A.Y - Y) + 1;
    int z2 = abs(A.Z - Z) + 1;

    return x2 * y2 * z2;
  }
};

struct Cube {
  Vector3 A, B;

  inline Cube(void) {}
  inline Cube(Vector3 a, Vector3 b) {
    A = a;
    B = b;
  }

  inline int xMin() { return std::min(A.X, B.X); }
  inline int yMin() { return std::min(A.Y, B.Y); }
  inline int zMin() { return std::min(A.Z, B.Z); }
  inline int xMax() { return std::max(A.X, B.X); }
  inline int yMax() { return std::max(A.Y, B.Y); }
  inline int zMax() { return std::max(A.Z, B.Z); }
};

extern std::unique_ptr<SQLite::Database> landDB;

namespace LandManager {
class Transaction {
  SQLite::Transaction trans;

public:
  Transaction();
  void Commit();
};

void InitDatabase();

std::optional<Mod::PlayerEntry> GetPlayerInstance(Player *player);
std::optional<std::string> ReachedLimit(Mod::PlayerEntry owner);
std::optional<std::string> Overlaps(Mod::PlayerEntry owner, Vector3 start, Vector3 end);
bool HasPerm(Mod::PlayerEntry player, Vector3 block);
std::optional<std::string> BuyLand(Mod::PlayerEntry owner, Vector3 start, Vector3 end);
std::string SellLand(Mod::PlayerEntry player, Vector3 block);
std::string GiveLand(Mod::PlayerEntry player, Vector3 block);
} // namespace LandManager

enum class Action { None, Create, Buy, Sell, Give, Exit };

extern Action action;
extern Vector3 pointA;
extern Vector3 pointB;

class CommandRegistry;
void initCommand(CommandRegistry *registry);