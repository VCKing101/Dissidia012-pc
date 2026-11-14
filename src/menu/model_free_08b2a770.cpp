/* Detaches the model (its table step 1 with mode 2) and frees its block.
   Retail keeps `self` in the frame across both calls (Kept holder). */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct Step08B2A770 {
    short delta;
    void (*fn)(char *self, int mode);
};

struct Model08B2A770 {
    char pad_000[0x848];
    Step08B2A770 *field_848;
};

struct State08B2A770 {
    char pad_000[0x704];
    void *field_704;
    Model08B2A770 *field_708;
};

struct Owner08B2A770 {
    char pad_000[0xA14];
    void *field_A14;
    State08B2A770 *field_A18;
};

extern "C" {
void func_0885C538(void *heap, void *block, int flags);

void func_08B2A770(Owner08B2A770 *self)
{
    Model08B2A770 *model = self->field_A18->field_708;
    if (model != 0) {
        Step08B2A770 *steps = model->field_848;
        Kept<Owner08B2A770 *> saved(self);
        {
            Step08B2A770 *step = &steps[1];
            step->fn((char *)model + step->delta, 2);
        }
        self = saved.value;
        func_0885C538(self->field_A14, self->field_A18->field_704, 0);
        self = saved.value;
        self->field_A18->field_704 = 0;
        self->field_A18->field_708 = 0;
    }
}
}
