typedef struct FxGroup { char motion[0xC]; float f0C; float f10; float f14; float f18; char pad_1C[0xC]; int f28; int f2C; int f30[5]; int f44[12]; } FxGroup;
extern void func_0892AC2C(FxGroup *g);
void func_0892D160(FxGroup *g)
{
    int i;
    func_0892AC2C(g);
    g->f28 = 0;
    g->f2C = 0;
    for (i = 0; i < 5; i++) {
        g->f30[i] = 0;
    }
    for (i = 0; i < 12; i++) {
        g->f44[i] = 0;
    }
    g->f0C = 0.0f;
    g->f10 = 0.0f;
    g->f14 = 0.0f;
    g->f18 = 0.0f;
}
