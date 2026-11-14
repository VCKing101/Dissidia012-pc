typedef struct Fighter55138 {
    char pad_000[0x188];
    unsigned int field_188;
    char pad_18C[0x1F8];
    unsigned int flags;
    char pad_388[0x1B0];
    int field_538;
} Fighter55138;

extern int func_0883739C(Fighter55138 *fighter);
extern void func_08B34ED0(Fighter55138 *fighter, int kind);
extern void func_08834DF4(Fighter55138 *fighter, float rate);

void func_08A55138(void *self, Fighter55138 *fighter)
{
    fighter->field_538 = 0;
    fighter->flags &= ~0x1000;
    if (func_0883739C(fighter) == 1) {
        func_08B34ED0(fighter, 2);
        fighter->field_188 &= ~0x10;
        func_08834DF4(fighter, 1.0f);
    }
}
