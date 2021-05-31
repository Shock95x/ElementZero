#pragma once

#include "../Item/NetworkItemStackDescriptor.h"
#include "../Item/ItemStack.h"
#include "../Core/ContainerID.h"
#include "../Core/Packet.h"

class InventoryContentPacket : public Packet {
public:

    ContainerID id;
    std::vector<NetworkItemStackDescriptor> items;

    inline ~InventoryContentPacket() {}

    InventoryContentPacket(void);
    InventoryContentPacket(ContainerID containerId, std::vector<class ItemStack> const & items);

    MCAPI virtual MinecraftPacketIds getId() const;
    MCAPI virtual std::string getName() const;
    MCAPI virtual void write(BinaryStream &) const;

private:
    MCAPI virtual StreamReadResult _read(ReadOnlyBinaryStream &);
};