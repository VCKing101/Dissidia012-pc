/* Appends a (key, value) pair to the page's queue (up to four) and flags it. */
typedef struct PushPair {
    short field_00;
    short field_02;
} PushPair;

typedef struct PushState {
    char pad_000[0x110];
    int field_110;
    char pad_114[0x120 - 0x114];
    PushPair field_120[4];
    char pad_130[0xC00 - 0x130];
    unsigned int field_C00;
} PushState;

typedef struct PushOwner {
    char pad_000[0xC18];
    PushState *field_C18;
} PushOwner;

void func_08AC75C0(PushOwner *self, int key, short value)
{
    int count = self->field_C18->field_110;
    if (count < 4) {
        self->field_C18->field_120[count].field_00 = key;
        self->field_C18->field_120[self->field_C18->field_110].field_02 = value;
        self->field_C18->field_110 = self->field_C18->field_110 + 1;
        self->field_C18->field_C00 = self->field_C18->field_C00 | 0x400;
    }
}
