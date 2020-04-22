#include <stdexcept>

#include <Core/mce.h>
#include <boost/lexical_cast.hpp>

#include <chakra_helper.h>
#include <log.h>
#include <playerdb.h>
#include <modutils.h>
#include <scriptapi.h>

using namespace Mod::Scripting;

JsValueRef PlayerBinding::InitProto() {
  static ValueHolder proto = IIFE([] {
    JsObjectWrapper PlayerProto;
    PlayerProto["xuid"]       = JsObjectWrapper::PropertyDesc{&PlayerBinding::GetXUID};
    PlayerProto["uuid"]       = JsObjectWrapper::PropertyDesc{&PlayerBinding::GetUUID};
    PlayerProto["name"]       = JsObjectWrapper::PropertyDesc{&PlayerBinding::GetNAME};
    PlayerProto["address"]    = JsObjectWrapper::PropertyDesc{&PlayerBinding::GetADDRESS};
    PlayerProto["alive"]      = JsObjectWrapper::PropertyDesc{&PlayerBinding::alive};
    PlayerProto["aux"]        = JsObjectWrapper::PropertyDesc{&PlayerBinding::GetAuxData};
    PlayerProto["getOffline"] = &PlayerBinding::ToOffline;
    PlayerProto["toString"]   = &PlayerBinding::ToString;
    return PlayerProto.ref;
  });
  return *proto;
}

JsValueRef OfflinePlayerBinding::InitProto() {
  static ValueHolder proto = IIFE([] {
    JsObjectWrapper OfflinePlayerProto;
    OfflinePlayerProto["xuid"]      = JsObjectWrapper::PropertyDesc{&OfflinePlayerBinding::GetXUID};
    OfflinePlayerProto["uuid"]      = JsObjectWrapper::PropertyDesc{&OfflinePlayerBinding::GetUUID};
    OfflinePlayerProto["name"]      = JsObjectWrapper::PropertyDesc{&OfflinePlayerBinding::GetNAME};
    OfflinePlayerProto["getOnline"] = &OfflinePlayerBinding::ToOnline;
    OfflinePlayerProto["toString"]  = &OfflinePlayerBinding::ToString;
    return OfflinePlayerProto.ref;
  });
  return *proto;
}

static ModuleRegister reg("ez:player", [](JsObjectWrapper native) -> std::string {
  native["getPlayerByXUID"] = +[](JsValueRef ref) {
    auto xuid = boost::lexical_cast<uint64_t>(JsToString(ref));
    auto &db  = Mod::PlayerDatabase::GetInstance();
    if (auto it = db.Find(xuid); it) return ToJs(*it);
    return GetUndefined();
  };
  native["getPlayerByUUID"] = +[](JsValueRef ref) {
    auto uuid = mce::UUID::fromString(JsToString(ref));
    auto &db  = Mod::PlayerDatabase::GetInstance();
    if (auto it = db.Find(uuid); it) return ToJs(*it);
    return GetUndefined();
  };
  native["getPlayerByNAME"] = +[](std::string const &name) {
    auto &db = Mod::PlayerDatabase::GetInstance();
    if (auto it = db.Find(name); it) return ToJs(*it);
    return GetUndefined();
  };

  native["getOfflinePlayerByXUID"] = +[](JsValueRef ref) {
    auto xuid = boost::lexical_cast<uint64_t>(JsToString(ref));
    auto &db  = Mod::PlayerDatabase::GetInstance();
    if (auto it = db.FindOffline(xuid); it) return ToJs(*it);
    return GetUndefined();
  };
  native["getOfflinePlayerByUUID"] = +[](JsValueRef ref) {
    auto uuid = mce::UUID::fromString(JsToString(ref));
    auto &db  = Mod::PlayerDatabase::GetInstance();
    if (auto it = db.FindOffline(uuid); it) return ToJs(*it);
    return GetUndefined();
  };
  native["getOfflinePlayerByNAME"] = +[](std::string const &name) {
    auto &db = Mod::PlayerDatabase::GetInstance();
    if (auto it = db.FindOffline(name); it) return ToJs(*it);
    return GetUndefined();
  };

  native["getPlayerList"] = +[]() {
    auto &db = Mod::PlayerDatabase::GetInstance();
    return ToJsArray(db.GetData());
  };

  native["onPlayerJoined"] = +[](JsValueRef ref) {
    if (GetJsType(ref) != JsFunction) throw std::runtime_error{"Require function argument"};
    auto &db = Mod::PlayerDatabase::GetInstance();
    db.AddListener(SIG("joined"), [=, fn{ValueHolder{ref}}](Mod::PlayerEntry const &entry) {
      JsValueRef ar[] = {GetUndefined(), ToJs(entry)};
      JsValueRef res;
      JsCallFunction(*fn, ar, 2, &res);
    });
    return GetUndefined();
  };
  native["onPlayerLeft"] = +[](JsValueRef ref) {
    if (GetJsType(ref) != JsFunction) throw std::runtime_error{"Require function argument"};
    auto &db = Mod::PlayerDatabase::GetInstance();
    db.AddListener(SIG("left"), [=, fn{ValueHolder{ref}}](Mod::PlayerEntry const &entry) {
      JsValueRef ar[] = {GetUndefined(), ToJs(entry)};
      JsValueRef res;
      JsCallFunction(*fn, ar, 2, &res);
    });
    return GetUndefined();
  };
  return R"(
    export const getPlayerByXUID = import.meta.native.getPlayerByXUID;
    export const getPlayerByUUID = import.meta.native.getPlayerByUUID;
    export const getPlayerByNAME = import.meta.native.getPlayerByNAME;

    export const getOfflinePlayerByXUID = import.meta.native.getOfflinePlayerByXUID;
    export const getOfflinePlayerByUUID = import.meta.native.getOfflinePlayerByUUID;
    export const getOfflinePlayerByNAME = import.meta.native.getOfflinePlayerByNAME;

    export const getPlayerList = import.meta.native.getPlayerList;

    export const onPlayerJoined = import.meta.native.onPlayerJoined;
    export const onPlayerLeft = import.meta.native.onPlayerLeft;
  )";
});