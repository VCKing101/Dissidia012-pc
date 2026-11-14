/* Appends a (key, value) pair to the page's queue (up to three) and flags it. */
typedef struct PushPair {
    short field_00;
    short field_02;
} PushPair;

typedef struct PushState {
    char pad_000[0x908];
    int field_908;
    char pad_90C[0x914 - 0x90C];
    PushPair field_914[3];
    char pad_920[0xE40 - 0x920];
    unsigned int field_E40;
    char pad_E44[0xE4C - 0xE44];
    int field_E4C;
} PushState;

typedef struct PushOwner {
    char pad_000[0x14C];
    PushState *field_14C;
} PushOwner;

void func_08A41674(PushOwner *self, int key, short value)
{
    int count = self->field_14C->field_908;
    if (count < 3) {
        self->field_14C->field_914[count].field_00 = key;
        self->field_14C->field_914[self->field_14C->field_908].field_02 = value;
        self->field_14C->field_908 = self->field_14C->field_908 + 1;
        self->field_14C->field_E4C = 0;
        self->field_14C->field_E40 = self->field_14C->field_E40 | 0x40;
    }
}
