#include "main/agent33_texture.h"

/* One shared picture (0x340 bytes), keyed by the caller. */
struct ImageSlot {
    int key;                           /* +0x00 */
    D012ImageObject *image;            /* +0x04 0 = slot free */
    int refcount;                      /* +0x08 */
    unsigned char unk_0C[0x340 - 0xC];
};

struct ImageSlots {
    ImageSlot slots[2];
};

extern "C" {

/* image_slot_load: a two-slot picture cache used by the fighter .objx loader (FUN_088459E0).
 * A key that is already loaded only gains a reference; otherwise the GIM goes into the
 * first free slot. With both slots taken the picture is dropped. */
void func_08852378(ImageSlots *self, const void *gim, int size, int key)
{
    for (int i = 0; i < 2; i++) {
        ImageSlot *slot = &self->slots[i];
        if (slot->key != key) {
            continue;
        }
        slot->refcount++;
        return;
    }
    for (int i = 0; i < 2; i++) {
        ImageSlot *slot = &self->slots[i];
        if (slot->image != 0) {
            continue;
        }
        slot->image = func_0899B9F0(0);
        func_0899BB68(slot->image, gim, size, 0);
        slot->key = key;
        slot->refcount++;
        return;
    }
}

}
