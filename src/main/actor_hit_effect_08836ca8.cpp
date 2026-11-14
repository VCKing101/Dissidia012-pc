extern "C" {
typedef struct Actor08836CA8 { char pad_000[0x1A8]; float rate; } Actor08836CA8;
extern signed char *D_089D85E8;
extern int func_088377C0(Actor08836CA8 *a);
extern void func_08AD0978(void *target, Actor08836CA8 *a, Actor08836CA8 *b, int c, int arg, int d, int e, float rate);
struct ModeOps {
    static bool isOne() { return *D_089D85E8 == 1; }
};
void func_08836CA8(Actor08836CA8 *a, void *target, int arg)
{
    float rate = func_088377C0(a) == 1 ? a->rate : 0.0f;
    if (ModeOps::isOne()) {
        func_08AD0978(target, a, a, 0, arg, 100, -1, rate);
    }
}
}
