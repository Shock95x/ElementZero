#pragma once

#include <unordered_set>
#include <deque>
#include <vector>
#include <functional>

#include "ContainerType.h"
#include "ContainerContentChangeListener.h"
#include "ContainerSizeChangeListener.h"
#include "../Core/NBT.h"
#include "../Item/ItemStack.h"
#include "../Math/Vec3.h"

#include "../dll.h"

class Player;
class BlockSource;
class ItemDescriptor;

class Container {
public:
  ContainerType type;
  std::unordered_set<ContainerContentChangeListener *> content_change_listeners; // 16
  std::unordered_set<ContainerSizeChangeListener *> content_size_listeners;      // 80
  std::deque<std::function<void(Container &, int, ItemStack const &, ItemStack const &)>> transactions;
  std::string custom_name;
  bool has_custom_name;

  MCAPI Container(ContainerType);
  MCAPI virtual ~Container();
  MCAPI virtual void init();
  MCAPI virtual void addContentChangeListener(class ContainerContentChangeListener *);
  MCAPI virtual void removeContentChangeListener(class ContainerContentChangeListener *);
  MCAPI virtual bool hasRoomForItem(class ItemStack const &);
  MCAPI virtual void addItem(class ItemStack &);
  MCAPI virtual bool addItemToFirstEmptySlot(class ItemStack &);
  MCAPI
  MCAPI virtual void setItemWithForceBalance(int, class ItemStack const &, bool);
  MCAPI virtual void removeItem(int, int);
  MCAPI virtual void removeAllItems(void);
  MCAPI virtual void dropContents(class BlockSource &, class Vec3 const &, bool);
  virtual int getContainerSize() const = 0;
  virtual int getMaxStackSize() const  = 0;
  virtual void startOpen(Player &)     = 0;
  virtual void stopOpen(Player &)      = 0;
  virtual class ItemStack const &getItem(int) const = 0;
  virtual void setItem(int, class ItemStack const &) = 0;
  virtual void serverInitItemStackIds(int, int, std::function<void(int, class ItemStack const &)>) = 0;
  MCAPI virtual std::vector<ItemStack> getSlotCopies() const;
  MCAPI virtual std::vector<ItemStack const *> const getSlots() const;
  MCAPI virtual int getItemCount(ItemStack const &) const;
  MCAPI virtual int findFirstSlotForItem(ItemStack const &) const;
  MCAPI virtual bool canPushInItem(BlockSource &, int, int, ItemInstance const &) const;
  MCAPI virtual bool canPullOutItem(BlockSource &, int, int, ItemInstance const &) const;
  MCAPI virtual void setContainerChanged(int);
  MCAPI virtual void setContainerMoved();
  MCAPI virtual void setCustomName(std::string const &);
  MCAPI virtual bool hasCustomName() const;
  MCAPI virtual void readAdditionalSaveData(CompoundTag const &);
  MCAPI virtual void addAdditionalSaveData(CompoundTag &);
  MCAPI virtual void createTransactionContext(
      std::function<void(Container &, int, ItemStack const &, ItemStack const &)>, std::function<void()>);
  MCAPI void triggerTransactionChange(int, ItemStack const &, ItemStack const &);
};

static_assert(offsetof(Container, content_change_listeners) == 16);
static_assert(offsetof(Container, content_size_listeners) == 80);
static_assert(offsetof(Container, custom_name) == 184);
static_assert(offsetof(Container, has_custom_name) == 216);