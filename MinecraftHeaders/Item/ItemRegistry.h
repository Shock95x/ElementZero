class ItemRegistry {
public:

    MCAPI static int getItemCount(void);

    MCAPI static class WeakPtr<class Item> getItem(short);
    MCAPI static class WeakPtr<class Item> getItem(Block const &);
    MCAPI static class WeakPtr<class Item> getItem(BlockLegacy const &);
};