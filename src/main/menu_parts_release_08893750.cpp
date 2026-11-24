/* Releases a menu's loaded parts, its extra part and item list, then the
   menu's own resources. */
extern "C" {
struct PartSlot {
    char pad_000[0x74];
    int loaded;
    char pad_078[0x130 - 0x78];

    static bool IsLoaded(PartSlot *s) { return s->loaded != 0; }
};

struct Menu {
    int heap;
    char pad_004[0x130 - 4];
    unsigned int flags;
    char pad_134[0x140 - 0x134];
    PartSlot parts[3];
    char pad_4D0[0x10];
    PartSlot extra;
    unsigned char has_extra;
    char pad_611[0x6B8 - 0x611];
    void *items;
    int count;
    int capacity;
};

void func_08894EA4(void *slot);
void func_08892418(Menu *self);
void func_0885C538(int heap, void *block, int flag);

void func_08893750(Menu *self)
{
    for (int i = 0; i < 3; i++) {
        if (PartSlot::IsLoaded(&self->parts[i])) {
            func_08894EA4(&self->parts[i]);
        }
    }
    if (PartSlot::IsLoaded(&self->extra)) {
        func_08894EA4(&self->extra);
    }
    self->has_extra = 0;
    self->flags &= ~1;
    func_08892418(self);
    if (self->items != 0) {
        func_0885C538(self->heap, self->items, 0);
        self->items = 0;
        self->count = 0;
        self->capacity = 0;
    }
    func_08894EA4(self);
}

}
