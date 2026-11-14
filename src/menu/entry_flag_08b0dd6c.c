/* True when entry (kind, id) is in the list (which ends at a negative id)
   and has flag 4. */
struct Entry08B0DD6C {
    short id;
    short field_2;
    signed char kind;
    unsigned char flags;
};

struct State08B0DD6C {
    char pad_00000[0x222F4];
    struct Entry08B0DD6C field_222F4[0x5DC];
};

struct Owner08B0DD6C {
    char pad_000[0x138];
    struct State08B0DD6C *field_138;
};

unsigned char func_08B0DD6C(struct Owner08B0DD6C *self, signed char kind, short id)
{
    unsigned char set = 0;
    int i;
    for (i = 0; i < 0x5DC; i++) {
        if (self->field_138->field_222F4[i].id < 0) {
            break;
        }
        if (self->field_138->field_222F4[i].kind == kind && self->field_138->field_222F4[i].id == id) {
            if ((self->field_138->field_222F4[i].flags & 4) != 0) {
                set = 1;
            }
            break;
        }
    }
    return set;
}
