#include "main/agent33_texture.h"

extern "C" {

/* tex_entry_load: fill a texture entry from a GIM (or TIM2/BMP) file in memory.
 * The image library copies the pixels, so `gim` may be freed afterwards.
 * Returns true when the image loaded. */
bool func_088DE0E8(D012TexEntry *entry, const void *gim, int size, int unk_06,
                   int id, unsigned char group)
{
    entry->image_addr = func_0899B9F0(0);
    if (!func_0899BB68(entry->image_addr, gim, size, 0)) {
        func_0899BB1C(entry->image_addr);
        entry->image_addr = 0;
    } else {
        entry->unk_06 = unk_06;
        entry->id = id;
        entry->refcount = 1;
        entry->group = group;
    }
    return entry->image_addr != 0;
}

}
