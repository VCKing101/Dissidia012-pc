typedef struct FxGroup { char pad_00[0x28]; int active; } FxGroup;
extern void func_0892D474(FxGroup *g);
void func_0892D448(FxGroup *g)
{
    if (g->active == 1) {
        func_0892D474(g);
    }
}
