#include "main/agent33_texture.h"

extern void *func_0885C6CC(void *heap, int size, int align, int tag, int one, int from_end); /* heap_alloc */
extern void func_0885C538(void *heap, void *block, int align);                              /* heap_free */
/* pack_load_into: load each listed pack into `buffer` and register its textures and seq objects. */
extern int func_088D2980(D012TexManager *mgr, void *buffer, int capacity, unsigned char group,
                         D012PkgListEntry *list, void *file);

/* pack_load_list: load a list of packs (GIM packs become texture entries, seq packs layout
 * objects of `group`) through a staging buffer of `capacity` bytes taken from `heap` and freed
 * again afterwards; the image library has copied the pixels by then. With `file` set, the packs
 * come from that already-loaded archive and no buffer is needed. */
int func_088D00FC(D012TexManager *mgr, void *heap, int capacity, unsigned char group,
                  D012PkgListEntry *list, void *file)
{
    /* Values the original keeps in the stack frame across the heap calls
     * (one-element arrays reproduce those spills; see build/dc-results/stack-spill.md). */
    int kept_result[1];
    unsigned char kept_group[1];
    D012PkgListEntry *kept_list[1];
    int kept_capacity[1];
    D012TexManager *kept_mgr[1];
    void *buffer = 0;
    int result;

    if (file == 0) {
        int one = 1;

        kept_mgr[0] = mgr;
        kept_capacity[0] = capacity;
        kept_list[0] = list;
        kept_group[0] = group;
        buffer = func_0885C6CC(heap, capacity, 0x40, 0, one, 1);   /* 64-byte aligned, from the top */
        group = kept_group[0];
        list = kept_list[0];
        capacity = kept_capacity[0];
        mgr = kept_mgr[0];
    }
    result = func_088D2980(mgr, buffer, capacity, group, list, file);
    if (file == 0) {
        kept_result[0] = result;
        func_0885C538(heap, buffer, 0x40);
        result = kept_result[0];
    }
    return result;
}
