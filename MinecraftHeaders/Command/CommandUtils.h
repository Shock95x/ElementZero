#pragma once

#include <string>
#include "../dll.h"

namespace CommandUtils {
    MCAPI std::string getActorName(class Actor const &);
    MCAPI class ItemStack createItemStack(std::string const &, int, int);
    MCAPI std::vector<class ItemStack> createItemStacks(class ItemInstance const &, int, int &);
}