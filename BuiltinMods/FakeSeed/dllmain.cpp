#include <dllentry.h>
#include <hook.h>
#include <base/base.h>
#include <Packet/StartGamePacket.h>
#include "global.h"

DEFAULT_SETTINGS(settings);

THook(void, "?write@StartGamePacket@@UEBAXAEAVBinaryStream@@@Z", StartGamePacket &pkt, BinaryStream &stream) {
    pkt.settings.seed = settings.seed;
    original(pkt, stream);
}