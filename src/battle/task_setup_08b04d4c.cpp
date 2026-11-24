/* Creates the update task and its effect holder, then sets up the two 0x24-byte sub-slots; returns 0 on success. */
/* Creates the update task and its effect holder, then sets up the two 0x24-byte sub-slots; returns 0 on success. */
extern "C" {
typedef struct Task { char pad[4]; void *owner; } Task;
typedef struct Eff { char pad0[0x15]; unsigned char a15; unsigned char a16; } Eff;
typedef struct Obj { unsigned char valid; char pad0[3]; int pad1; Eff *eff; char v0c[0x10]; struct Slot { char b[0x24]; } slots[2]; } Obj;
extern void func_08B04CA0(Obj *);
extern Task *func_088823C4(int, int, int, void *, const char *);
extern void func_08B04ED0(void *);
extern Eff *func_088D0700(void *, int, int, int);
extern void func_088CFF30(void *, Eff *);
extern void func_08B0546C(void *, int, Eff *, void *);
extern void *D_089D7830;
extern const char D_08B05250[];
int func_08B04D4C(Obj *self, int a, int b, int c) {
    Task *t;
    Eff *e;
    func_08B04CA0(self);
    if (a == 0) return -1;
    t = func_088823C4(a, b, c, func_08B04ED0, D_08B05250);
    if (t == 0) return -2;
    t->owner = self;
    self->eff = func_088D0700(D_089D7830, 1, 0x254, 0x1E);
    e = self->eff;
    if (e == 0) return -3;
    e->a16 = 2;
    self->eff->a15 = 200;
    { Eff *h = self->eff; func_088CFF30(D_089D7830, h); }
    for (int i = 0; i < 2; i++) {
        func_08B0546C(&self->slots[i], i, self->eff, self->v0c);
    }
    self->valid = 1;
    return 0;
}
}
