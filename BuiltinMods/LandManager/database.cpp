#include "settings.h"

DEF_LOGGER("LandManager");

using json = nlohmann::json;

void LandManager::InitDatabase() {
  landDB = std::make_unique<SQLite::Database>(settings.database, SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
  landDB->exec(
      "CREATE TABLE IF NOT EXISTS lands "
      "(owner INTEGER, trusted TEXT, x1 INTEGER, y1 INTEGER, z1 INTEGER, x2 "
      "INTEGER, y2 INTEGER, z2 INTEGER, chkx1 "
      "INTEGER, chky1 INTEGER, "
      "chkz1 INTEGER, chkx2 INTEGER, chky2 INTEGER, chkz2 INTEGER, created_at "
      "DATETIME DEFAULT(STRFTIME('%Y-%m-%d %H:%M:%f', 'now')))");
}

Vector3 getChunk(Vector3 vec) {
  int x = vec.X > 0 ? std::floor(vec.X / 16) : std::ceil(vec.X / 16);
  int y = vec.Y > 0 ? std::floor(vec.Y / 16) : std::ceil(vec.Y / 16);
  int z = vec.Z > 0 ? std::floor(vec.Z / 16) : std::ceil(vec.Z / 16);

  return Vector3(x, y, z);
}

bool isInside(Vector3 point, Vector3 start, Vector3 end) {
  Cube c = Cube(start, end);

  return (
      point.X >= c.xMin() && point.X <= c.xMax() && point.Y >= c.yMin() && point.Y <= c.yMax() && point.Z >= c.zMin() &&
      point.Z <= c.zMax());
}

bool doIntersect(Vector3 new1, Vector3 new2, Vector3 test1, Vector3 test2) {
  Cube cNew  = Cube(new1, new2);
  Cube cTest = Cube(test1, test2);

  std::ostringstream ree;

  ree << cNew.xMin() << "," << cNew.yMin() << "," << cNew.zMin() << " | " << cNew.xMax() << "," << cNew.yMax() << ","
      << cNew.zMax();
  LOGW(ree.str().c_str());

  return (cNew.xMin() <= cTest.xMax() && cNew.xMax() >= cTest.xMin()) &&
         (cNew.yMin() <= cTest.yMax() && cNew.yMax() >= cTest.yMin()) &&
         (cNew.zMin() <= cTest.zMax() && cNew.zMax() >= cTest.zMin());
}

std::vector<uint16_t> findStandingLand(Mod::PlayerEntry player, Vector3 block) {
  static SQLite::Statement stmt{
      *landDB,
      "SELECT rowid, x1, y1, z1, x2, y2, z2 FROM lands WHERE owner = ? AND ((chkx1 = ? AND chky1 = ? AND chkz1 = "
      "?) OR (chkx2 = ? AND chky2 = ? AND chkz2 = ?))"};

  BOOST_SCOPE_EXIT_ALL() {
    stmt.clearBindings();
    stmt.tryReset();
  };

  std::vector<uint16_t> ids;
  Vector3 chunk = getChunk(block);

  try {
    stmt.bind(1, (int64_t) player.xuid);
    stmt.bind(2, chunk.X);
    stmt.bind(3, chunk.Y);
    stmt.bind(4, chunk.Z);
    stmt.bind(5, chunk.X);
    stmt.bind(6, chunk.Y);
    stmt.bind(7, chunk.Z);

    while (stmt.executeStep()) {
      uint16_t rowid = stmt.getColumn(0).getInt64();
      int x1         = stmt.getColumn(1).getInt();
      int y1         = stmt.getColumn(2).getInt();
      int z1         = stmt.getColumn(3).getInt();
      int x2         = stmt.getColumn(4).getInt();
      int y2         = stmt.getColumn(5).getInt();
      int z2         = stmt.getColumn(6).getInt();

      bool inside = isInside(block, Vector3(x1, y1, z1), Vector3(x2, y2, z2));

      if (inside) { ids.push_back(rowid); }
    }
  } catch (SQLite::Exception ex) {
    LOGE(ex.what());
    return ids;
  }

  return ids;
}

std::optional<Mod::PlayerEntry> LandManager::GetPlayerInstance(Player *player) {
  auto &pdb     = Mod::PlayerDatabase::GetInstance();
  auto instance = pdb.Find(player);

  if (instance) { return instance; }

  return {};
}

std::optional<std::string> LandManager::ReachedLimit(Mod::PlayerEntry player) {
  static SQLite::Statement stmt{*landDB, "SELECT COUNT(*) FROM lands WHERE owner = ?"};

  BOOST_SCOPE_EXIT_ALL() {
    stmt.clearBindings();
    stmt.reset();
  };

  stmt.bind(1, (int64_t) player.xuid);

  try {
    int count = stmt.executeStep() ? stmt.getColumn(0).getInt() : 0;
    if (count >= settings.limit) return "[Zonas] Haz alcanzado el limite de zonas.";
  } catch (SQLite::Exception ex) { return "[Zonas] Ocurrio un error. Contacte al administrador."; }

  return {};
}

std::optional<std::string> LandManager::Overlaps(Mod::PlayerEntry player, Vector3 start, Vector3 end) {
  // We get all the zones in the same chunks and check for overlapping

  static SQLite::Statement stmt{
      *landDB,
      "SELECT x1, y1, z1, x2, y2, z2 FROM lands WHERE (chkx1 = ? AND chky1 = ? AND chkz1 = ?) OR (chkx2 = ? AND chky2 "
      "= ? AND chkz2 = ?) OR (chkx1 = ? AND chky1 = ? AND chkz1 = ?) OR (chkx2 = ? AND chky2 = ? AND chkz2 = ?)"};

  BOOST_SCOPE_EXIT_ALL() {
    stmt.clearBindings();
    stmt.tryReset();
  };

  Vector3 chunk1 = getChunk(start);
  Vector3 chunk2 = getChunk(end);

  stmt.bind(1, chunk1.X);
  stmt.bind(2, chunk1.Y);
  stmt.bind(3, chunk1.Z);
  stmt.bind(4, chunk1.X);
  stmt.bind(5, chunk1.Y);
  stmt.bind(6, chunk1.Z);
  stmt.bind(7, chunk2.X);
  stmt.bind(8, chunk2.Y);
  stmt.bind(9, chunk2.Z);
  stmt.bind(10, chunk2.X);
  stmt.bind(11, chunk2.Y);
  stmt.bind(12, chunk2.Z);

  try {
    while (stmt.executeStep()) {
      int x1 = stmt.getColumn(0).getInt();
      int y1 = stmt.getColumn(1).getInt();
      int z1 = stmt.getColumn(2).getInt();
      int x2 = stmt.getColumn(3).getInt();
      int y2 = stmt.getColumn(4).getInt();
      int z2 = stmt.getColumn(5).getInt();

      bool inters = doIntersect(start, end, Vector3(x1, y1, z1), Vector3(x2, y2, z2));
      if (inters) return "[Zonas] Estas encima de una zona ya existente.";
    }
  } catch (SQLite::Exception ex) { return "[Zonas] Ocurrio un error. Contacte al administrador."; }

  return {};
}

bool LandManager::HasPerm(Mod::PlayerEntry player, Vector3 block) {
  // We get all the zones in the block's chunk and check for perms

  static SQLite::Statement stmt{
      *landDB,
      "SELECT owner, x1, y1, z1, x2, y2, z2 FROM lands WHERE (chkx1 = ? AND chky1 = ? AND chkz1 = ?) OR (chkx2 = ? "
      "AND chky2 = ? AND chkz2 = ?)"};

  BOOST_SCOPE_EXIT_ALL() {
    stmt.clearBindings();
    stmt.tryReset();
  };

  Vector3 chunk = getChunk(block);

  stmt.bind(1, chunk.X);
  stmt.bind(2, chunk.Y);
  stmt.bind(3, chunk.Z);
  stmt.bind(4, chunk.X);
  stmt.bind(5, chunk.Y);
  stmt.bind(6, chunk.Z);

  try {
    while (stmt.executeStep()) {
      uint64_t owner = stmt.getColumn(0).getInt64();
      int x1         = stmt.getColumn(1).getInt();
      int y1         = stmt.getColumn(2).getInt();
      int z1         = stmt.getColumn(3).getInt();
      int x2         = stmt.getColumn(4).getInt();
      int y2         = stmt.getColumn(5).getInt();
      int z2         = stmt.getColumn(6).getInt();

      bool inside = isInside(block, Vector3(x1, y1, z1), Vector3(x2, y2, z2));

      if (inside && owner != player.xuid) {
        // For now we only check if its the player, we should add actual perms later
        return false;
      }
    }
  } catch (SQLite::Exception ex) {
    LOGE(ex.what());
    return false;
  }

  return true;
}

std::optional<std::string> LandManager::BuyLand(Mod::PlayerEntry player, Vector3 start, Vector3 end) {
  // We store the player, start, end and their corresponding chunks

  static SQLite::Statement stmt{
      *landDB,
      "INSERT INTO lands (owner, trusted, x1, y1, z1, x2, y2, z2, chkx1, chky1, chkz1, chkx2, chky2, chkz2) VALUES "
      "(?, "
      "?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"};

  BOOST_SCOPE_EXIT_ALL() {
    stmt.clearBindings();
    stmt.tryReset();
  };

  std::string trusted = json::array({}).dump();
  Vector3 chunk1      = getChunk(start);
  Vector3 chunk2      = getChunk(end);

  stmt.bind(1, (int64_t) player.xuid);
  stmt.bindNoCopy(2, trusted);
  stmt.bind(3, start.X);
  stmt.bind(4, start.Y);
  stmt.bind(5, start.Z);
  stmt.bind(6, end.X);
  stmt.bind(7, end.Y);
  stmt.bind(8, end.Z);
  stmt.bind(9, chunk1.X);
  stmt.bind(10, chunk1.Y);
  stmt.bind(11, chunk1.Z);
  stmt.bind(12, chunk2.X);
  stmt.bind(13, chunk2.Y);
  stmt.bind(14, chunk2.Z);

  try {
    stmt.exec();
    return {};
  } catch (SQLite::Exception const &ex) { return ex.what(); }
}

std::string LandManager::SellLand(Mod::PlayerEntry player, Vector3 block) {
  std::vector<uint16_t> ids = findStandingLand(player, block);

  for (uint16_t id : ids) {
    static SQLite::Statement stmt{*landDB, "DELETE FROM lands WHERE rowid = ?"};

    BOOST_SCOPE_EXIT_ALL() {
      stmt.clearBindings();
      stmt.tryReset();
    };

    stmt.bind(1, id);

    try {
      stmt.exec();
      return "[Zonas] Has vendido una zona.";
    } catch (SQLite::Exception const &ex) { return "[Zonas] Ocurrio un error. Contacte al administrador."; }
  }

  return "[Zonas] No estas dentro de una zona.";
}

std::string LandManager::GiveLand(Mod::PlayerEntry player, Vector3 block) {
  std::vector<uint16_t> ids = findStandingLand(player, block);

  for (uint16_t id : ids) {
    static SQLite::Statement stmt{*landDB, "UPDATE lands SET owner = ? WHERE rowid = ?"};

    BOOST_SCOPE_EXIT_ALL() {
      stmt.clearBindings();
      stmt.tryReset();
    };

    stmt.bind(1, (int64_t) player.xuid);
    stmt.bind(2, id);

    try {
      stmt.exec();
      return "[Zonas] Has transferido una zona.";
    } catch (SQLite::Exception const &ex) { return "[Zonas] Ocurrio un error. Contacte al administrador."; }
  }

  return "[Zonas] No estas dentro de una zona.";
}

LandManager::Transaction::Transaction() : trans(*landDB) {}
void LandManager::Transaction::Commit() { trans.commit(); }
