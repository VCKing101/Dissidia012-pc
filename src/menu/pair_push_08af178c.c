/* Appends a (key, value) pair to the page's queue (up to 40) and flags it. */
typedef struct PushPair {
    short field_00;
    short field_02;
} PushPair;

typedef struct PushState {
    char pad_000[0xB40];
    unsigned int field_B40;
    char pad_B44[0x2D24 - 0xB44];
    int field_2D24;
    char pad_2D28[0x2D34 - 0x2D28];
    PushPair field_2D34[40];
} PushState;

typedef struct PushOwner {
    char pad_000[0x140];
    PushState *field_140;
} PushOwner;

void func_08AF178C(PushOwner *self, int key, short value)
{
    int count = self->field_140->field_2D24;
    if (count < 0x28) {
        self->field_140->field_2D34[count].field_00 = key;
        self->field_140->field_2D34[self->field_140->field_2D24].field_02 = value;
        self->field_140->field_2D24 = self->field_140->field_2D24 + 1;
        self->field_140->field_B40 = self->field_140->field_B40 | 4;
    }
}
