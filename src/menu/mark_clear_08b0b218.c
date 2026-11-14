/* Clears the `mask` bits of entry `id` (the list ends at -1). */
struct Entry08B0B218 {
    short id;
    unsigned short flags;
};

struct State08B0B218 {
    char pad_00000[0x1FFCC];
    struct Entry08B0B218 field_1FFCC[0x2AB];
};

struct Owner08B0B218 {
    char pad_000[0x138];
    struct State08B0B218 *field_138;
};

void func_08B0B218(struct Owner08B0B218 *self, short id, unsigned short mask)
{
    int i;
    for (i = 0; i < 0x2AB; i++) {
        if (self->field_138->field_1FFCC[i].id == -1) {
            break;
        }
        if (self->field_138->field_1FFCC[i].id == id) {
            self->field_138->field_1FFCC[i].flags &= ~mask;
            break;
        }
    }
}
