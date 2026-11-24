/* Constructor: base init, allocates a list head via func_0885C6CC, fills it and clears two fields. */
struct Head { int f0; int pad[4]; int f14; int f18; int f1c; char pad2[0x12C]; int f14c; char pad3[0xA0]; int f1f0; };
struct Self { char pad[0x18]; struct Head *h; };
extern void *func_08B42B40(struct Self *);
extern struct Head *func_0885C6CC(void *, int, int, int, int, int);
extern int D_089D8518;
extern void func_08B477AC(struct Self *);
void func_08B46E7C(struct Self *self) {
    void *r = func_08B42B40(self);
    int one = 1;
    self->h = func_0885C6CC(r, 0x1F4, 0, 0, one, 0);
    self->h->f0 = 0;
    self->h->f1c = D_089D8518;
    self->h->f14 = -1;
    self->h->f18 = -1;
    func_08B477AC(self);
    self->h->f14c = 0;
    self->h->f1f0 = 0;
}
