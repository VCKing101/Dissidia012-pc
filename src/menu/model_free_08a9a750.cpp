/* Detaches the model (its table step 1 with mode 2) and frees its block.
   Retail keeps `self` in the frame across both calls (Kept holder). */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct FreeStep08A9A750 {
    short delta;
    void (*fn)(char *self, int mode);
};

struct FreeModel08A9A750 {
    char pad_0000[0x1EE8];
    FreeStep08A9A750 *field_1EE8;
};

struct FreeState08A9A750 {
    char pad_000[0x974];
    void *field_974;
    FreeModel08A9A750 *field_978;
};

struct FreePage08A9A750 {
    char pad_000[0x138];
    void *field_138;
    char pad_13C[0x154 - 0x13C];
    FreeState08A9A750 *field_154;
};

extern "C" void func_0885C538(void *heap, void *block, int zero);

extern "C" {
void func_08A9A750(FreePage08A9A750 *self)
{
    FreeModel08A9A750 *model = self->field_154->field_978;
    if (model != 0) {
        FreeStep08A9A750 *steps = model->field_1EE8;
        Kept<FreePage08A9A750 *> saved(self);
        {
            FreeStep08A9A750 *step = &steps[1];
            step->fn((char *)model + step->delta, 2);
        }
        self = saved.value;
        func_0885C538(self->field_138, self->field_154->field_974, 0);
        self = saved.value;
        self->field_154->field_978 = 0;
        self->field_154->field_974 = 0;
    }
}
}
