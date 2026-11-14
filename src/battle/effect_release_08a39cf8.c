typedef struct EffectSet39CF8 {
    char pad_000[0x24];
    void *field_24[8];
    void *field_44[7];
} EffectSet39CF8;

typedef struct EffectOwner39CF8 {
    char pad_000[0xC];
    int field_0C;
    char pad_010[0x4];
    EffectSet39CF8 *set;
} EffectOwner39CF8;

extern void func_088A6A78(void *effect);
extern void func_088DA060(void *handle);

void func_08A39CF8(EffectOwner39CF8 *self)
{
    int i;
    for (i = 0; i < 8; i++) {
        if (self->set->field_24[i]) {
            func_088A6A78(self->set->field_24[i]);
        }
        self->set->field_24[i] = 0;
    }
    if (self->field_0C) {
        for (i = 0; i < 7; i++) {
            if (self->set->field_44[i]) {
                func_088DA060(self->set->field_44[i]);
                self->set->field_44[i] = 0;
            }
        }
    }
}
