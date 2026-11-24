// For four slots calls func_089B538C with each (x, y) pair at +0x2C/+0x30 (stride 8) when the global's +0x74 block exists, then releases the handle at +0x488 and clears it.
extern "C" {
extern void func_089B538C(int, int, float, float);
extern void func_0899BB1C(int);
}
struct Blk57 {
    char pad0[0xC];
    int h;
};
struct Glob57 {
    char pad0[0x74];
    Blk57 *blk;
    static bool has(Blk57 *b) { return b != 0; }
};
extern "C" Glob57 *D_089D6AC8;
struct Pair57 {
    float x;
    float y;
};
struct Own57 {
    char pad0[0x2C];
    Pair57 xy[4];
    char pad1[0x488 - 0x4C];
    int handle;
};
extern "C" void func_08B56E28(Own57 *self)
{
    if (Glob57::has(D_089D6AC8->blk) == 1) {
        int i = 0;
        float *p = (float *)self;
        do {
            func_089B538C(D_089D6AC8->blk->h, i, p[0x2C / 4], p[0x30 / 4]);
            i = i + 1;
            p = p + 2;
        } while (i < 4);
    }
    func_0899BB1C(self->handle);
    self->handle = 0;
}
