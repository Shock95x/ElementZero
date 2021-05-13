#include <dllentry.h>
#include <hook.h>

#include "global.h"

void PreInit() {
  mod_enabled = true;

    GetServerSymbolWithOffset<PatchSpan<6>>(
        "?onServerThreadStarted@MinecraftServerScriptEngine@@UEAA?AW4EventResult@@AEAVServerInstance@@@Z", 0x32)
        ->VerifyPatchFunction({0x0F, 0x84, 0x5B, 0x01, 0x00, 0x00}, NopFilled{});
    GetServerSymbolWithOffset<PatchSpan<6>>(
        "?onServerThreadStarted@MinecraftServerScriptEngine@@UEAA?AW4EventResult@@AEAVServerInstance@@@Z", 0x3F)
        ->VerifyPatchFunction({0x0F, 0x84, 0x4E, 0x01, 0x00, 0x00}, NopFilled{});
    GetServerSymbolWithOffset<PatchSpan<6>>(
        "?onServerThreadStarted@MinecraftServerScriptEngine@@UEAA?AW4EventResult@@AEAVServerInstance@@@Z", 0x63)
        ->VerifyPatchFunction({0x0F, 0x84, 0x2A, 0x01, 0x00, 0x00}, NopFilled{});
}