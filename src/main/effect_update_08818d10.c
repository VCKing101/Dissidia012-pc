typedef struct Effect {
    char pad0[0xC];
    unsigned char active;
    char padD[0x3];
    int state;
    char pad14[0x4];
    char sub[0x10];
} Effect;
extern float func_08819294(Effect *effect);
extern void func_088193A4(Effect *effect, float time);
extern void func_08819480(Effect *effect, float time);
extern void func_08819538(Effect *effect);
extern void func_088195C8(Effect *effect);
extern void func_08819F84(void *sub);
int func_08818D10(Effect *effect)
{
    if (effect->active == 0) {
        return -1;
    }
    if (effect->state == 1) {
        float time = func_08819294(effect);
        func_088193A4(effect, time);
        func_08819480(effect, time);
        func_08819538(effect);
        func_088195C8(effect);
    }
    if (effect->state == 4) {
        func_08819F84(effect->sub);
    }
    return 0;
}
