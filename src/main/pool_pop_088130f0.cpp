/* Takes an item from the free list of the 64-byte item pool and clears
   its owner. */
struct PoolItem088130F0 {
    char pad_00[0x28];
    void *owner;
    char pad_2C[0x14];
};

struct Pool088130F0 {
    PoolItem088130F0 items[4];
    int free[4];
    int free_count;
};

extern "C" {

extern Pool088130F0 D_089D47B0;

PoolItem088130F0 *func_088130F0(void)
{
    D_089D47B0.free_count--;
    PoolItem088130F0 *item = &D_089D47B0.items[D_089D47B0.free[D_089D47B0.free_count]];
    item->owner = 0;
    return item;
}

}
