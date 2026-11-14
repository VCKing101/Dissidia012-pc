typedef struct Effect {
    char pad0[0x8];
    void *first;
    char padC[0x4];
    void *second;
    char pad14[0x4];
    void *third;
    char pad1C[0x4];
    void *fourth;
} Effect;
extern void func_08842138(void *object);
extern void func_0881A25C(Effect *effect);
void func_0881A284(Effect *effect)
{
    if (effect->fourth != 0) {
        func_08842138(effect->fourth);
    }
    if (effect->third != 0) {
        func_08842138(effect->third);
    }
    if (effect->second != 0) {
        func_08842138(effect->second);
    }
    if (effect->first != 0) {
        func_08842138(effect->first);
    }
    func_0881A25C(effect);
}
