/* Clears the `mask` bits of entry `id` (the list ends at -1). */
struct Entry08B0B300 {
    short id;
    unsigned short flags;
};

struct State08B0B300 {
    char pad_00000[0x21450];
    struct Entry08B0B300 field_21450[0x32];
};

struct Owner08B0B300 {
    char pad_000[0x138];
    struct State08B0B300 *field_138;
};

void func_08B0B300(struct Owner08B0B300 *self, short id, unsigned short mask)
{
    int i;
    for (i = 0; i < 0x32; i++) {
        if (self->field_138->field_21450[i].id == -1) {
            break;
        }
        if (self->field_138->field_21450[i].id == id) {
            self->field_138->field_21450[i].flags &= ~mask;
            break;
        }
    }
}
