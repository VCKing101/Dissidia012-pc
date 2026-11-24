// Clears two int tables and calls the first virtual on four arrays of embedded items; stores item pointers.
struct ItemBase { virtual void v0(); };
struct ItemA { char d[0x84]; virtual void v0(); };
struct ItemB : ItemBase { int d; };
struct ItemC : ItemBase { int d[2]; };
struct Holder {
    int a;
    int b;
    int c;
    int tab0[14];
    char gap0[0xB4 - 0x44];
    int tab1[3];
    char gap1[0xE0 - 0xC0];
    ItemA ia[2];
    int e0;
    ItemB ib[2];
    ItemC ic[2];
    ItemC id[1];
    ItemC *p0;
    ItemC *p1;
    ItemC *p2;
    ItemB *p3;
    ItemB *p4;
};
extern "C" void func_08A5327C(Holder *h) {
    int i;
    h->b = 0;
    for (i = 0; i < 14; i++) h->tab0[i] = 0;
    for (i = 0; i < 3; i++) h->tab1[i] = 0;
    for (i = 0; i < 2; i++) h->ia[i].v0();
    h->c = 0;
    h->e0 = 0;
    for (i = 0; i < 2; i++) h->ib[i].v0();
    for (i = 0; i < 2; i++) h->ic[i].v0();
    for (i = 0; i < 1; i++) h->id[i].v0();
    h->p0 = &h->ic[0];
    h->p1 = &h->id[0];
    h->p2 = &h->ic[1];
    h->p3 = &h->ib[0];
    h->p4 = &h->ib[1];
}
