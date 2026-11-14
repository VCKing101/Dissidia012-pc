/* Stops the loader (mode 2) and frees its block. Retail keeps `self` in the
   frame across both calls (Kept holder). */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct State08B2D744 {
    char pad_000[0x6C0];
    void *field_6C0;
    void *field_6C4;
};

struct Owner08B2D744 {
    char pad_000[0x840];
    void *field_840;
    State08B2D744 *field_844;
};

extern "C" {
void func_08A80D40(void *loader, int mode);
void func_0885C538(void *heap, void *block, int flags);

void func_08B2D744(Owner08B2D744 *self)
{
    void *loader = self->field_844->field_6C4;
    if (loader != 0) {
        Kept<Owner08B2D744 *> saved(self);
        func_08A80D40(loader, 2);
        self = saved.value;
        func_0885C538(self->field_840, self->field_844->field_6C0, 0);
        self = saved.value;
        self->field_844->field_6C0 = 0;
        self->field_844->field_6C4 = 0;
    }
}
}
