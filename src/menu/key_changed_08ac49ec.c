typedef struct TripleKey {
    unsigned short field_00;
    unsigned short field_02;
    unsigned short field_04;
    unsigned short field_06;
    unsigned short field_08;
    unsigned short field_0A;
} TripleKey;

typedef struct TripleState {
    char pad_000[0x9F4];
    TripleKey field_9F4;
} TripleState;

typedef struct TripleOwner {
    char pad_000[0x138];
    TripleState *field_138;
} TripleOwner;

int func_08AC49EC(TripleOwner *self, TripleKey *key)
{
    TripleKey *current = &self->field_138->field_9F4;
    if (current->field_00 != key->field_00) {
        return 1;
    }
    if (current->field_02 != key->field_02) {
        return 1;
    }
    if (current->field_04 != key->field_04) {
        return 1;
    }
    return 0;
}
