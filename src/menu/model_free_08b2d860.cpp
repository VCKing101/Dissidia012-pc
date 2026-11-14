/* Detaches the loader's model (its table step 1 with mode 2) and frees its
   block. Retail keeps `self` in the frame across both calls (Kept holder). */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct Step08B2D860 {
    short delta;
    void (*fn)(char *self, int mode);
};

struct Model08B2D860 {
    char pad_000[0x9DC];
    Step08B2D860 *field_9DC;
};

struct State08B2D860 {
    char pad_000[0x524];
    void *field_524;
    Model08B2D860 *field_528;
};

struct Owner08B2D860 {
    char pad_000[0x840];
    void *field_840;
    State08B2D860 *field_844;
};

extern "C" {
void func_0885C538(void *heap, void *block, int flags);

void func_08B2D860(Owner08B2D860 *self)
{
    Model08B2D860 *model = self->field_844->field_528;
    if (model != 0) {
        Step08B2D860 *steps = model->field_9DC;
        Kept<Owner08B2D860 *> saved(self);
        {
            Step08B2D860 *step = &steps[1];
            step->fn((char *)model + step->delta, 2);
        }
        self = saved.value;
        func_0885C538(self->field_840, self->field_844->field_524, 0);
        self = saved.value;
        self->field_844->field_524 = 0;
        self->field_844->field_528 = 0;
    }
}
}
