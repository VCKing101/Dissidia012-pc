typedef struct Fighter55E88 {
    char pad_000[0x5AC];
    void *field_5AC;
} Fighter55E88;

extern int func_0883739C(Fighter55E88 *fighter);
extern void func_08B3AF54(void *sub);
extern void func_08834DF4(Fighter55E88 *fighter, float rate);

void func_08A55E88(void *self, Fighter55E88 *fighter)
{
    if (func_0883739C(fighter) == 1 && fighter->field_5AC) {
        func_08B3AF54(fighter->field_5AC);
    }
    func_08834DF4(fighter, 1.0f);
}
