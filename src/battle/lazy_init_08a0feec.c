typedef struct LazyFEEC {
    char pad_000[0x8];
    int field_08;
} LazyFEEC;

extern int func_08A0FF20(LazyFEEC *);

int func_08A0FEEC(LazyFEEC *self)
{
    if (!self->field_08) {
        return func_08A0FF20(self);
    }
    return 0;
}
