/* Builds the 0x5E message for the current entry (0 when there is none) and
   points the send slot at it. Returns whether a message was built. */
struct Tail08AF0110 {
    unsigned short field_0;
    unsigned short field_2;
    unsigned short field_4;
    unsigned short field_6;
};

struct Message08AF0110 {
    unsigned short field_0;
    unsigned short field_2;
    unsigned short field_4;
    unsigned short field_6;
    struct Tail08AF0110 field_8;
};

struct Kind08AF0110 {
    char pad_0[6];
    unsigned short field_6;
};

struct State08AF0110 {
    char pad_00[0xC];
    struct Message08AF0110 field_0C;
    char pad_1C[0x2C - 0x1C];
    struct Message08AF0110 *field_2C;
    char pad_30[0x54 - 0x30];
    int field_54;
    char pad_58[4];
    int field_5C;
};

struct Owner08AF0110 {
    char pad_00[0x70];
    struct State08AF0110 *field_70;
};

extern unsigned short D_08AF11F0[];
extern struct Kind08AF0110 D_08AF11D8[];

unsigned char func_08AF0110(struct Owner08AF0110 *self)
{
    struct State08AF0110 *state = self->field_70;
    struct Message08AF0110 *message;
    struct Tail08AF0110 *tail;
    if (state->field_54 == 0) {
        state->field_2C = 0;
        return 0;
    }
    message = &state->field_0C;
    message->field_0 = 0x5E;
    message->field_2 = D_08AF11F0[self->field_70->field_54];
    message->field_4 = 0x5A;
    message->field_6 = D_08AF11D8[self->field_70->field_5C].field_6;
    tail = &message->field_8;
    tail->field_0 = 0xFFFF;
    tail->field_2 = 0xFFFF;
    tail->field_4 = 0xFFFF;
    tail->field_6 = 0xFFFF;
    self->field_70->field_2C = &self->field_70->field_0C;
    return 1;
}
