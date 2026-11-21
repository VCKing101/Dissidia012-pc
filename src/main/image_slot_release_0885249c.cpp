extern "C" {
typedef struct Slot0885249C {
    int key;                           /* +0x00 -1 = free */
    void *image;                       /* +0x04 D012ImageObject */
    int refs;                          /* +0x08 */
    unsigned char unk_0C[0x340 - 0xC];
} Slot0885249C;

typedef struct Images0885249C {
    Slot0885249C slots[2];
} Images0885249C;

extern void func_088525A8(Images0885249C *self, int key);
extern void func_0899BB1C(void *image);   /* img_destroy */

/* Drop one reference to the image slot holding `key`; the last one frees the image. */
void func_0885249C(Images0885249C *self, int key)
{
    for (int i = 0; i < 2; i++) {
        Slot0885249C *slot = &self->slots[i];

        if (slot->key != key) {
            continue;
        }
        if (--slot->refs == 0) {
            func_088525A8(self, key);
            func_0899BB1C(slot->image);
            slot->image = 0;
            slot->key = -1;
        }
        goto done;
    }
done:
    return;
}
}
