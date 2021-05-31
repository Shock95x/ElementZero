#pragma once

#include "../BlockActor/BlockActor.h"

class RandomizableBlockActorContainerBase : public BlockActor {
public:
    std::string lootTable;
    int lootTableSeed;
};

static_assert(offsetof(RandomizableBlockActorContainerBase, lootTable) == 208);
static_assert(offsetof(RandomizableBlockActorContainerBase, lootTableSeed) == 240);

static_assert(sizeof(RandomizableBlockActorContainerBase) == 248);