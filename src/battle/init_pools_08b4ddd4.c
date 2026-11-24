// Initialises two pools from the context in a1: allocates 0x12C0 bytes at +0x1C and 0x640 at +0x20, runs func_08B4D8B0 on each of the 50 0x60-byte entries, zeroes the float at +0x14 of each 0x20-byte entry and clears +0x24.
extern int func_0885C6CC(int, int, int, int, int, int);
extern void func_08B4D8B0(void *, void *, int);
typedef struct Small57 {
    char pad0[0x14];
    float f14;
    char pad1[8];
} Small57;
typedef struct Pools57 {
    char pad0[4];
    int ctx;
    char pad1[0x14];
    char *big;
    Small57 *small;
    int count;
} Pools57;
void func_08B4DDD4(Pools57 *self, int ctx)
{
    int i;
    int one;
    self->ctx = ctx;
    one = 1;
    self->big = (char *)func_0885C6CC(ctx, 0x12C0, 0, 0, one, 0);
    self->small = (Small57 *)func_0885C6CC(self->ctx, 0x640, 0, 0, one, 0);
    for (i = 0; i < 50; i++) {
        func_08B4D8B0(self, self->big + i * 0x60, 1);
    }
    for (i = 0; i < 50; i++) {
        self->small[i].f14 = 0.0f;
    }
    self->count = 0;
}
