/* True when the state's flag 8 is set. */
struct State08B2E790 {
    unsigned int field_0;
};

struct Owner08B2E790 {
    char pad_000[0x844];
    struct State08B2E790 *field_844;
};

unsigned char func_08B2E790(struct Owner08B2E790 *self)
{
    if ((self->field_844->field_0 & 8) != 0) {
        return 1;
    }
    return 0;
}
