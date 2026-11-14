/* Clears bit 0 of the flags of entry `id` (the list ends at -1). */
struct Entry08B1EC68 {
    short id;
    unsigned short flags;
};

struct State08B1EC68 {
    char pad_0000[0x88D0];
    struct Entry08B1EC68 field_88D0[0x276];
};

struct Owner08B1EC68 {
    char pad_000[0x138];
    struct State08B1EC68 *field_138;
};

void func_08B1EC68(struct Owner08B1EC68 *self, short id)
{
    int i;
    for (i = 0; i < 0x276; i++) {
        if (self->field_138->field_88D0[i].id == -1) {
            break;
        }
        if (self->field_138->field_88D0[i].id == id) {
            self->field_138->field_88D0[i].flags &= ~1;
            break;
        }
    }
}
