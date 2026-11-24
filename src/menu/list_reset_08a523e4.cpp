// Clears the slot tables and calls the first virtual on each of the two embedded items.
struct ItemBase { virtual void v0(); };
struct Item : ItemBase { int data; };
struct Holder {
    int a;
    int count;
    int slots[6];
    int gap[0x48 / 4 - 6];
    int flags[2];
    char pad[0x74 - 0x58];
    Item items[2];
    Item *first;
    Item *second;
    int last;
};
extern "C" void func_08A523E4(Holder *h) {
    int i;
    h->count = 0;
    for (i = 0; i < 6; i++) h->slots[i] = 0;
    for (i = 0; i < 2; i++) h->flags[i] = 0;
    for (i = 0; i < 2; i++) h->items[i].v0();
    h->first = &h->items[0];
    h->second = &h->items[1];
    h->last = 0;
}
