#include "main/agent33_texture.h"

struct FepCachePool {
    /* Take an unused slot record from the cache's pool, or 0 when none is left. */
    static D012FepCacheSlot *take(D012FepCache *cache) {
        if (cache->free_count > 0) {
            return cache->free_slots[--cache->free_count];
        }
        return 0;
    }
};

extern "C" {

/* fep_cache_split: cut a slot in two. Its first `size` bytes are given to `key`;
 * the rest becomes a new free slot right after it. Returns 0 when the pool is empty. */
int func_089742F8(D012FepCache *cache, D012FepCacheSlot *slot, unsigned int size, int key)
{
    D012FepCacheSlot *rest = FepCachePool::take(cache);

    if (rest == 0) {
        return 0;
    }
    rest->vram_offset = slot->vram_offset + size;
    rest->size = slot->size - size;
    rest->key = -1;
    rest->age = 0;
    rest->next = slot->next;
    slot->size = size;
    slot->key = key;
    slot->age = 0;
    slot->next = rest;
    return 1;
}

}
