// Adds the percent modifiers of two status ids (0x11F, 0x120) to the floats at +0x3D4 and +0x3D8 when the owner's status list has them.
extern "C" int *func_088BF4CC(int, int);
struct Has57 { static bool has(int *p) { return p != 0; } };
struct Mod57 {
    char pad0[4];
    short amount;
};
struct Self57 {
    char pad0[0x310];
    int list;
    char pad1[0x3D4 - 0x314];
    float f3D4;
    float f3D8;
};
extern "C" void func_08B2E050(Self57 *self)
{
    if (Has57::has(func_088BF4CC(self->list, 0x11F))) {
        float base = self->f3D4;
        Mod57 *m = (Mod57 *)func_088BF4CC(self->list, 0x11F);
        short *p = &m->amount;
        self->f3D4 = base + (float)*p / 100.0f;
    }
    if (Has57::has(func_088BF4CC(self->list, 0x120))) {
        float base = self->f3D8;
        Mod57 *m = (Mod57 *)func_088BF4CC(self->list, 0x120);
        short *p = &m->amount;
        self->f3D8 = base + (float)*p / 100.0f;
    }
}
