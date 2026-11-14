/* Detaches the model (its table step 1 with mode 2) and frees its block.
   Retail keeps `self` in the frame across both calls (Kept holder). */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct FreeStep08A9AE6C {
    short delta;
    void (*fn)(char *self, int mode);
};

struct FreeModel08A9AE6C {
    char pad_0000[0xA1C];
    FreeStep08A9AE6C *field_A1C;
};

struct FreeState08A9AE6C {
    char pad_000[0x9B4];
    void *field_9B4;
    FreeModel08A9AE6C *field_9B8;
};

struct FreePage08A9AE6C {
    char pad_000[0x138];
    void *field_138;
    char pad_13C[0x154 - 0x13C];
    FreeState08A9AE6C *field_154;
};

extern "C" void func_0885C538(void *heap, void *block, int zero);

extern "C" {
void func_08A9AE6C(FreePage08A9AE6C *self)
{
    FreeModel08A9AE6C *model = self->field_154->field_9B8;
    if (model != 0) {
        FreeStep08A9AE6C *steps = model->field_A1C;
        Kept<FreePage08A9AE6C *> saved(self);
        {
            FreeStep08A9AE6C *step = &steps[1];
            step->fn((char *)model + step->delta, 2);
        }
        self = saved.value;
        func_0885C538(self->field_138, self->field_154->field_9B4, 0);
        self = saved.value;
        self->field_154->field_9B8 = 0;
        self->field_154->field_9B4 = 0;
    }
}
}
