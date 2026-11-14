typedef struct LazyAA6C {
    char pad_000[0x8];
    int field_08;
} LazyAA6C;

extern int func_089FAAA0(LazyAA6C *);

int func_089FAA6C(LazyAA6C *self)
{
    if (!self->field_08) {
        return func_089FAAA0(self);
    }
    return 0;
}
