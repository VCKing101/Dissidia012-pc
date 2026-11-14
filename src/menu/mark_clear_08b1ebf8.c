/* Clears bit 0 of the flags of entry `id` (the list ends at -1). */
struct Entry08B1EBF8 {
    short id;
    unsigned short flags;
};

struct State08B1EBF8 {
    char pad_0000[0x8740];
    struct Entry08B1EBF8 field_8740[100];
};

struct Owner08B1EBF8 {
    char pad_000[0x138];
    struct State08B1EBF8 *field_138;
};

void func_08B1EBF8(struct Owner08B1EBF8 *self, short id)
{
    int i;
    for (i = 0; i < 100; i++) {
        if (self->field_138->field_8740[i].id == -1) {
            break;
        }
        if (self->field_138->field_8740[i].id == id) {
            self->field_138->field_8740[i].flags &= ~1;
            break;
        }
    }
}
