typedef struct BlinkState {
    char pad_000[0x5A4];
    unsigned int field_5A4;
    char pad_5A8[0x5D0 - 0x5A8];
    int field_5D0;
} BlinkState;

typedef struct BlinkOwner {
    char pad_000[0x850];
    BlinkState *field_850;
} BlinkOwner;

void func_08AE28C4(BlinkOwner *self, int frames)
{
    self->field_850->field_5D0 = frames;
    self->field_850->field_5A4 = self->field_850->field_5A4 & ~0x200;
    if (self->field_850->field_5D0 > 0) {
        self->field_850->field_5A4 = self->field_850->field_5A4 | 0x200;
    }
}
