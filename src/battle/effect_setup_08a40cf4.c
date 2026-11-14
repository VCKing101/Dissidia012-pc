typedef struct EffectSlots40CF4 {
    char pad_000[0x2C];
    int field_2C;
    char pad_030[0x14];
    int field_44[3];
    int field_50;
    int field_54;
    int field_58;
} EffectSlots40CF4;

typedef struct EffectUnit40CF4 {
    char pad_000[0x130];
    char field_130[0x4];
} EffectUnit40CF4;

typedef struct EffectSetup40CF4 {
    EffectUnit40CF4 *unit;
    char pad_004[0x8];
    int field_0C;
    char pad_010[0x4];
    EffectSlots40CF4 *slots;
} EffectSetup40CF4;

extern void func_08935828(void *dst, int value, int size);
extern int func_088DAA68(int source);
extern void func_0882FA10(void *sub, int id, int value, float a, float b);
extern void func_08A40B9C(EffectSetup40CF4 *self);

void func_08A40CF4(EffectSetup40CF4 *self)
{
    int i;
    func_08935828(self->slots, 0, 0x5C);
    for (i = 0; i < 3; i++) {
        self->slots->field_44[i] = func_088DAA68(self->field_0C);
    }
    self->slots->field_50 = func_088DAA68(self->field_0C);
    self->slots->field_54 = func_088DAA68(self->field_0C);
    self->slots->field_58 = func_088DAA68(self->field_0C);
    func_0882FA10(self->unit->field_130, 0x1FA5, 0, 0.0f, 0.0f);
    func_08A40B9C(self);
    self->slots->field_2C = 0;
}
