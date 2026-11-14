typedef struct EffectOwnerE8F0 {
    char pad_000[0x130];
    char field_130[0x254];
    unsigned int flags;
    char pad_388[0x20];
    float *field_3A8;
} EffectOwnerE8F0;

extern void func_0882FA10(float x, float y, void *sub, int id, int arg);

void func_08A2E8F0(int unused, EffectOwnerE8F0 *self)
{
    float zero = 0.0f;
    float *value = self->field_3A8;
    func_0882FA10(zero, zero, self->field_130, 0x4E48, 0);
    *value = zero;
    self->flags |= 0x200000;
}
