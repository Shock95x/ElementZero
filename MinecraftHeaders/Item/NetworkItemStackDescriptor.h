#pragma once

#include "ItemStackDescriptor.h"
#include "ItemDescriptor.h"
#include "Block//BlockPalette.h"
#include "ItemStackNetIdVariant.h"
#include "ItemStack.h"

class NetworkItemStackDescriptor : public ItemStackDescriptor {
public:
    bool includeNetIds;                           // 80
    ItemStackNetIdVariant netIdVariant;           // 84
    std::string unk;

    inline NetworkItemStackDescriptor() {}
    MCAPI NetworkItemStackDescriptor(ItemStack const &);
    MCAPI NetworkItemStackDescriptor(ItemStackDescriptor const &);

    MCAPI void read(ReadOnlyBinaryStream &);
    MCAPI void write(BinaryStream &) const;
    MCAPI void setIncludeNetIds(bool);

    inline ~NetworkItemStackDescriptor() {}

private:
    MCAPI ItemStack getItemStack(BlockPalette const &);
};

static_assert(offsetof(NetworkItemStackDescriptor, includeNetIds) == 80);
static_assert(offsetof(NetworkItemStackDescriptor, netIdVariant) == 84);
static_assert(offsetof(NetworkItemStackDescriptor, unk) == 96);