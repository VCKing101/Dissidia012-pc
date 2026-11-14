typedef struct EffectSet33EF8 {
    char pad_000[0x7C];
    void *field_7C[3];
    void *field_88[12];
} EffectSet33EF8;

typedef struct EffectOwner33EF8 {
    char pad_000[0xC];
    int field_0C;
    char pad_010[0x4];
    EffectSet33EF8 *set;
} EffectOwner33EF8;

extern void func_088A6A78(void *effect);
extern void func_088DA060(void *handle);

void func_08A33EF8(EffectOwner33EF8 *self)
{
    int i;
    for (i = 0; i < 3; i++) {
        if (self->set->field_7C[i]) {
            func_088A6A78(self->set->field_7C[i]);
        }
        self->set->field_7C[i] = 0;
    }
    if (self->field_0C) {
        for (i = 0; i < 12; i++) {
            if (self->set->field_88[i]) {
                func_088DA060(self->set->field_88[i]);
                self->set->field_88[i] = 0;
            }
        }
    }
}
