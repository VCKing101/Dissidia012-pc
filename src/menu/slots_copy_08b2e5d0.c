/* Copies the nine slot words pairs out of the state. */
struct Pairs08B2E5D0 {
    int words[18];
};

struct State08B2E5D0 {
    char pad_000[0x62C];
    struct Pairs08B2E5D0 field_62C;
};

struct Owner08B2E5D0 {
    char pad_000[0x844];
    struct State08B2E5D0 *field_844;
};

void func_08B2E5D0(struct Owner08B2E5D0 *self, struct Pairs08B2E5D0 *out)
{
    *out = self->field_844->field_62C;
}
