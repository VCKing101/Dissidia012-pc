typedef struct FxGroup { char pad_00[0x28]; int active; } FxGroup;
extern void func_0892D8F0(FxGroup *g, int a, int b, int c);
void func_0892D380(FxGroup *g, int a, int b, int c)
{
    if (g->active == 0) {
        return;
    }
    func_0892D8F0(g, a, b, c);
}
