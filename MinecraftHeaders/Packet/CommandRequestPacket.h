#pragma once

#include "../Core/Packet.h"
#include "../Command/CommandOriginData.h"
#include "../dll.h"

class CommandRequestPacket : public Packet {
public:
  std::string command;
  CommandOriginData data;
  bool flag = false;

  inline ~CommandRequestPacket() {}
  MCAPI virtual MinecraftPacketIds getId() const;
  MCAPI virtual std::string getName() const;
  MCAPI virtual void write(BinaryStream &) const;

  private:
    MCAPI virtual StreamReadResult _read(ReadOnlyBinaryStream &);
};

static_assert(offsetof(CommandRequestPacket, command) == 48);
static_assert(offsetof(CommandRequestPacket, data) == 80);
static_assert(offsetof(CommandRequestPacket, flag) == 144);