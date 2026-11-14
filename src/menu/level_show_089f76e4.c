typedef struct LevelGauge {
    char pad_00[0x20];
    unsigned short field_20;
} LevelGauge;

typedef struct LevelState {
    char pad_0000[0x500];
    LevelGauge *field_500;
    char pad_0504[0x2724 - 0x504];
    unsigned int field_2724;
    char pad_2728[0x2731 - 0x2728];
    signed char field_2731;
    char pad_2732[0x31E0 - 0x2732];
    float field_31E0;
} LevelState;

typedef struct LevelOwner {
    char pad_000[0x14C];
    LevelState *field_14C;
} LevelOwner;

extern void func_089F7634(LevelOwner *self);
extern void func_088D9F08(LevelGauge *gauge, float value);
extern void func_088D9D24(LevelGauge *gauge);
extern void func_089F777C(LevelOwner *self);

void func_089F76E4(LevelOwner *self)
{
    LevelGauge *gauge;
    func_089F7634(self);
    self->field_14C->field_31E0 = (float)self->field_14C->field_2731 * 10.0f;
    func_088D9F08(self->field_14C->field_500, self->field_14C->field_31E0);
    gauge = self->field_14C->field_500;
    gauge->field_20 = gauge->field_20 | 2;
    gauge->field_20 = gauge->field_20 & ~0x20;
    func_088D9D24(gauge);
    self->field_14C->field_2724 = self->field_14C->field_2724 | 8;
    func_089F777C(self);
}
