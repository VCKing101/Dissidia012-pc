typedef struct Model61AC8 {
    char pad_000[0xF4];
    int field_F4;
} Model61AC8;

typedef struct Fighter61AC8 {
    char pad_000[0x130];
    char effects[0x1E4];
    Model61AC8 *model;
    char pad_318[0x278];
    char field_590[0x4];
} Fighter61AC8;

extern void func_0882FA10(void *effects, int id, int arg, float a, float b);
extern int func_0883739C(Fighter61AC8 *fighter);
extern void func_089A3868(void *sub, int value);

void func_08A61AC8(Fighter61AC8 *fighter, int value)
{
    fighter->model->field_F4 = 1;
    func_0882FA10(fighter->effects, 0x19A, 0, 0.0f, 0.0f);
    if (func_0883739C(fighter) == 1) {
        func_089A3868(fighter->field_590, value);
    }
}
