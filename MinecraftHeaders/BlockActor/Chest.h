#pragma once

#include "../Container/RandomizableBlockActorFillingContainer.h"

class ChestBlockActor : public RandomizableBlockActorFillingContainer {
    char pad[144];

public:
    using BlockActor::setCustomName;

    MCAPI virtual ~ChestBlockActor(void);
    MCAPI ChestBlockActor(enum BlockActorType, std::string const &,enum BlockActorRendererId, class BlockPos const &, bool);
    
    MCAPI bool isLargeChest(void) const;
    MCAPI virtual void onRemoved(class BlockSource &);
    MCAPI virtual class Container const * getContainer(void) const;
   
    MCAPI virtual void addContentChangeListener(class ContainerContentChangeListener *);
    MCAPI bool canOpen(class BlockSource &) const;
    MCAPI bool canPairWith(class BlockActor *,class BlockSource &);
    MCAPI virtual void clearCache(void);
    MCAPI virtual int clearInventory(int);
    MCAPI void forceCloseChest(class BlockSource &);
    MCAPI virtual class BlockActor * getCrackEntity(class BlockSource &,class BlockPos const &);
    MCAPI virtual void getDebugText(class std::vector<std::string,class std::allocator<std::string>> &,class BlockPos const &);
    MCAPI virtual class ItemStack const & getItem(int) const;
    MCAPI virtual std::string getName(void) const;
    MCAPI int getOpenCount(void) const;
    MCAPI float getOpenness(void) const;
    MCAPI class BlockPos const & getPairedChestPosition(void);
    MCAPI virtual void initializeContainerContents(class BlockSource &);
    MCAPI bool isFindable(void) const;
    MCAPI virtual void load(class Level &,class CompoundTag const &, class DataLoadHelper &);
    MCAPI virtual void loadItems(class CompoundTag const &,class Level &);
    MCAPI virtual void onChanged(class BlockSource &);
    MCAPI void onMove(class BlockSource &,class BlockPos const &,class BlockPos const &);
    MCAPI virtual void onPlace(class BlockSource &);
    MCAPI void openBy(class Player &);
    MCAPI void pairWith(class ChestBlockActor *,bool);
    MCAPI virtual void removeContentChangeListener(class ContainerContentChangeListener *);
    MCAPI virtual bool save(class CompoundTag &) const;
    MCAPI virtual bool saveItemInstanceData(class CompoundTag &);
    MCAPI virtual bool saveItems(class CompoundTag &) const;
    MCAPI virtual void serverInitItemStackIds(int,int,class std::function<void (int,class ItemStack const &)>);
    MCAPI virtual void setContainerChanged(int);
    MCAPI void setFindable(bool);
    MCAPI virtual void setItem(int,class ItemStack const &);
    MCAPI virtual void startOpen(class Player &);
    MCAPI virtual void stopOpen(class Player &);
    MCAPI virtual void tick(class BlockSource &);
    MCAPI virtual void triggerEvent(int,int);
    MCAPI void unpair(class BlockSource &);
};

static_assert(sizeof(ChestBlockActor) == 648);