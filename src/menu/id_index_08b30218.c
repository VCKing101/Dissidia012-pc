/* Returns the position of `id` in the state's id list, or -1. */
struct State08B30218 {
    char pad_000[0x98C];
    int field_98C;
    int field_990[1];
};

struct Owner08B30218 {
    char pad_000[0x9D8];
    struct State08B30218 *field_9D8;
};

int func_08B30218(struct Owner08B30218 *self, int id)
{
    int found = -1;
    int i;
    for (i = 0; i < self->field_9D8->field_98C; i++) {
        if (self->field_9D8->field_990[i] == id) {
            found = i;
            break;
        }
    }
    return found;
}
