typedef struct EffectSet40DC4 {
    char pad_000[0x3C];
    void *field_3C;
    char pad_040[0x4];
    void *field_44[6];
} EffectSet40DC4;

typedef struct EffectOwner40DC4 {
    char pad_000[0xC];
    int field_0C;
    char pad_010[0x4];
    EffectSet40DC4 *set;
} EffectOwner40DC4;

extern void func_088A6A78(void *effect);
extern void func_088DA060(void *handle);

void func_08A40DC4(EffectOwner40DC4 *self)
{
    int i;
    if (self->set->field_3C) {
        func_088A6A78(self->set->field_3C);
    }
    self->set->field_3C = 0;
    if (self->field_0C) {
        for (i = 0; i < 6; i++) {
            if (self->set->field_44[i]) {
                func_088DA060(self->set->field_44[i]);
                self->set->field_44[i] = 0;
            }
        }
    }
}
