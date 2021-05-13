#include "ItemDescriptor.h"

class ItemStackDescriptor : public ItemDescriptor {
public:
    char pad[0x28];

    MCAPI ItemStackDescriptor(void);
    MCAPI ItemStackDescriptor(Item const &, int, unsigned short, CompoundTag const *);
};