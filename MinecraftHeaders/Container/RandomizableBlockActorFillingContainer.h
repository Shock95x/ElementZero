#pragma once

#include "FillingContainer.h"
#include "RandomizableBlockActorContainerBase.h"

class RandomizableBlockActorFillingContainer : public RandomizableBlockActorContainerBase, protected FillingContainer {

    MCAPI RandomizableBlockActorFillingContainer(enum BlockActorType, std::string const &, class BlockPos const &, int , enum ContainerType);
};