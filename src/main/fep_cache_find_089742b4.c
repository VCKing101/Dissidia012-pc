#include "main/agent33_texture.h"

/* fep_cache_find: the effect VRAM cache slot holding the texture with this key, or 0. */
D012FepCacheSlot *func_089742B4(D012FepCache *cache, unsigned int key)
{
    D012FepCacheSlot *slot;

    for (slot = cache->slots; slot != 0; slot = slot->next) {
        if (slot->key == key) {
            return slot;
        }
    }
    return 0;
}
