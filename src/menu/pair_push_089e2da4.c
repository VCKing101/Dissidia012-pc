/* Appends a (key, value) pair to the owner's input queue and flags it. */
typedef struct QueuePair {
    short field_00;
    short field_02;
} QueuePair;

typedef struct InputQueue {
    char pad_000[0x4];
    unsigned int field_04;
    char pad_008[0xB54 - 0x8];
    int field_B54;
    char pad_B58[0xB64 - 0xB58];
    QueuePair field_B64[256];
} InputQueue;

typedef struct QueueOwner {
    char pad_000[0x140];
    InputQueue *field_140;
} QueueOwner;

void func_089E2DA4(QueueOwner *self, int key, short value)
{
    int count = self->field_140->field_B54;
    if (count < 0x100) {
        self->field_140->field_B64[count].field_00 = key;
        self->field_140->field_B64[self->field_140->field_B54].field_02 = value;
        self->field_140->field_B54 = self->field_140->field_B54 + 1;
        self->field_140->field_04 = self->field_140->field_04 | 8;
    }
}
