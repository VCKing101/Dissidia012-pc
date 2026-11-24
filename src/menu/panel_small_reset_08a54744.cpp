// Clears two int tables and calls the first virtual on the one embedded item; stores its pointer.
struct ItemBase { virtual void v0(); };
struct ItemB : ItemBase { int d; };
struct Holder {
    int a;
    int b;
    int tab0[4];
    char gap0[0x38 - 0x18];
    int tab1[1];
    char gap1[0x50 - 0x3C];
    ItemB ib[1];
    ItemB *p0;
    int last;
};
extern "C" void func_08A54744(Holder *h) {
    int i;
    h->b = 0;
    for (i = 0; i < 4; i++) h->tab0[i] = 0;
    for (i = 0; i < 1; i++) h->tab1[i] = 0;
    for (i = 0; i < 1; i++) h->ib[i].v0();
    h->p0 = &h->ib[0];
    h->last = 0;
}
