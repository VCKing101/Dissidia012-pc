typedef struct ItemPairOwner {
    char pad_00[0x8];
    void *handle;
    char pad_0C[0xC];
    void *items[2];
} ItemPairOwner;

extern "C" {

extern void func_08ABB85C(void *item);
extern void func_0899BB1C(void *handle);
extern void func_08ABA878(ItemPairOwner *self);

/* Release both items and the handle, then clear the owner. */
void func_08ABA8AC(ItemPairOwner *self)
{
    for (int i = 0; i < 2; i++) {
        if (self->items[i] != 0) {
            func_08ABB85C(self->items[i]);
        }
    }
    if (self->handle != 0) {
        func_0899BB1C(self->handle);
    }
    func_08ABA878(self);
}

}
