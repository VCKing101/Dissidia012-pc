/* Returns the position of `id` in the state's id list, or 0. */
struct State08B2B668 {
    char pad_000[0x898];
    int field_898;
    int field_89C[1];
};

struct Owner08B2B668 {
    char pad_000[0xA18];
    struct State08B2B668 *field_A18;
};

int func_08B2B668(struct Owner08B2B668 *self, int id)
{
    int found = 0;
    int i;
    for (i = 0; i < self->field_A18->field_898; i++) {
        if (self->field_A18->field_89C[i] == id) {
            found = i;
            break;
        }
    }
    return found;
}
