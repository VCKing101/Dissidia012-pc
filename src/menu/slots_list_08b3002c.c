/* Lists the slots that are empty or flagged. */
struct Slot08B3002C {
    int field_0;
    int field_4;
};

struct State08B3002C {
    char pad_000[0x7F1];
    unsigned char field_7F1[5];
    char pad_7F6[0x8C0 - 0x7F6];
    struct Slot08B3002C field_8C0[5];
    char pad_8E8[0x914 - 0x8E8];
    int field_914[6];
    int field_92C;
};

struct Owner08B3002C {
    char pad_000[0x9D8];
    struct State08B3002C *field_9D8;
};

void func_08B3002C(struct Owner08B3002C *self)
{
    int i;
    self->field_9D8->field_92C = 0;
    for (i = 0; i < 5; i++) {
        if (self->field_9D8->field_8C0[i].field_0 <= 0 || self->field_9D8->field_7F1[i] != 0) {
            self->field_9D8->field_914[self->field_9D8->field_92C] = i;
            self->field_9D8->field_92C++;
        }
    }
}
