// Stores a1 at +0, initialises 32 entries (stride 0x14 from +4) with func_08B436E8 and clears the byte at +0x284.
extern "C" void func_08B436E8(void *);
typedef struct { char data[0x14]; } Ent08B43850;
typedef struct { int v; Ent08B43850 e[32]; char pad[0x284 - 0x4 - 32 * 0x14]; unsigned char flag; } Obj08B43850;
extern "C" void func_08B43850(Obj08B43850 *self, int v)
{

    self->v = v;
    for (int i = 0; i < 32; i++) {
        func_08B436E8(&self->e[i]);
    }
    self->flag = 0;
}
