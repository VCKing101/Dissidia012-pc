/* Clears the `mask` bits of entry `id` (the list ends at -1). */
struct Entry08B0B28C {
    short id;
    unsigned short flags;
};

struct State08B0B28C {
    char pad_00000[0x20A78];
    struct Entry08B0B28C field_20A78[0x276];
};

struct Owner08B0B28C {
    char pad_000[0x138];
    struct State08B0B28C *field_138;
};

void func_08B0B28C(struct Owner08B0B28C *self, short id, unsigned short mask)
{
    int i;
    for (i = 0; i < 0x276; i++) {
        if (self->field_138->field_20A78[i].id == -1) {
            break;
        }
        if (self->field_138->field_20A78[i].id == id) {
            self->field_138->field_20A78[i].flags &= ~mask;
            break;
        }
    }
}
