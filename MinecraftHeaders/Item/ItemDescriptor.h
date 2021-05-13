#pragma once

class Item;
class Block;

#include "../Core/WeakPtr.h"

class ItemDescriptor {
public:
    char pad[0x28];

    MCAPI bool isValid(void) const;
    MCAPI bool isNull(void) const;
    MCAPI bool hasTags(void) const;

    MCAPI short getId(void) const;
    MCAPI int getIdAux(void) const;
    MCAPI short getAuxValue(void) const;

    MCAPI Block const * getBlock() const;
    MCAPI Item const * getItem() const;
    MCAPI std::string getRawNameId() const;

    MCAPI WeakPtr<BlockLegacy> const & getBlockLegacy() const;
};