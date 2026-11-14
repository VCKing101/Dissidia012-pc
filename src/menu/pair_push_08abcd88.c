/* Appends a (key, value) pair to the page's queue (up to two) and flags it. */
typedef struct PushPair {
    short field_00;
    short field_02;
} PushPair;

typedef struct PushState {
    char pad_000[0x8DC];
    int field_8DC;
    char pad_8E0[0x8E8 - 0x8E0];
    PushPair field_8E8[2];
    char pad_8F0[0xA58 - 0x8F0];
    unsigned int field_A58;
} PushState;

typedef struct PushOwner {
    char pad_000[0x14C];
    PushState *field_14C;
} PushOwner;

void func_08ABCD88(PushOwner *self, int key, short value)
{
    int count = self->field_14C->field_8DC;
    if (count < 2) {
        self->field_14C->field_8E8[count].field_00 = key;
        self->field_14C->field_8E8[self->field_14C->field_8DC].field_02 = value;
        self->field_14C->field_8DC = self->field_14C->field_8DC + 1;
        self->field_14C->field_A58 = self->field_14C->field_A58 | 0x80;
    }
}
