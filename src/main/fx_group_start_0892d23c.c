typedef struct FxGroup { char pad_00[0x28]; int active; } FxGroup;
extern void func_0892D4C8(FxGroup *g);
extern void func_0892D594(FxGroup *g, int a, int b, int c, int d, int e);
void func_0892D23C(FxGroup *g, int a, int b, int c, int d, int e)
{
    func_0892D4C8(g);
    func_0892D594(g, a, b, c, d, e);
    g->active = 1;
}
