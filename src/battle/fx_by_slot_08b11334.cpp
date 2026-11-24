// Unless func_088F47E0(0x16) is set, looks up the slot (index at node+8, stride 0x10 from +0x78) in the table at owner->+0x34->+0x14 and kicks its effect with id and the float at node+4.
extern "C" {
extern int func_088F47E0(int);
extern void func_08B1209C(void *, int);
extern void func_08B122CC(float, void *);
}
struct Slot57 {
    char pad0[0x10];
    short id;
};
struct Tab57 {
    char pad0[0x14];
    char *tab;
};
struct Own57 {
    char pad0[0x34];
    Tab57 *t;
};
struct Node57 {
    int pad0;
    int amount;
    int index;
};
struct Has57 { static bool has(int v) { return v != 0; } };
struct Row57 {
    char pad0[0xC];
    Slot57 *slot;
};
extern "C" int func_08B11334(Own57 *self, int *np)
{
    if (!Has57::has(func_088F47E0(0x16))) {
        np = np + 1;
        char *tab = self->t->tab;
        Row57 *row = (Row57 *)(tab + np[1] * 0x10 + 0x6C);
        Slot57 *s = row->slot;
        char *fx = tab + 0xA0;
        if (s != 0) {
            func_08B1209C(fx, s->id);
            func_08B122CC((float)np[0], fx);
        }
    }
    return 0;
}
