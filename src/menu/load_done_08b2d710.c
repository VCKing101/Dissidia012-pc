/* Returns the loader's status, or 0 when there is no loader. */
struct State08B2D710 {
    char pad_000[0x6C4];
    void *field_6C4;
};

struct Owner08B2D710 {
    char pad_000[0x844];
    struct State08B2D710 *field_844;
};

extern int func_08A80E34(void *loader);

int func_08B2D710(struct Owner08B2D710 *self)
{
    if (self->field_844->field_6C4 == 0) {
        return 0;
    } else {
        return func_08A80E34(self->field_844->field_6C4);
    }
}
