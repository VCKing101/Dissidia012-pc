typedef struct FxGroupItem { char data[0x28]; } FxGroupItem;
typedef struct FxGroup { char motion[0xC]; float f0C; float f10; float f14; float f18; char pad_1C[0xC]; int f28; int f2C; int f30[1]; int f34; FxGroupItem items[2]; } FxGroup;
extern void func_0892AC2C(FxGroup *g);
extern void func_0892C758(FxGroupItem *item);
void func_0892CC78(FxGroup *g)
{
    int i;
    func_0892AC2C(g);
    g->f28 = 0;
    g->f2C = 0;
    for (i = 0; i < 1; i++) {
        g->f30[i] = 0;
    }
    g->f34 = 0;
    for (i = 0; i < 2; i++) {
        func_0892C758(&g->items[i]);
    }
    g->f0C = 0.0f;
    g->f10 = 0.0f;
    g->f14 = 0.0f;
    g->f18 = 0.0f;
}
