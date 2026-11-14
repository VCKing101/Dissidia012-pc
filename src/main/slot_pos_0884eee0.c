typedef struct PosSlot {
    char pad_00[0x14];
    short x;
    short y;
    char pad_18;
    unsigned char dirty;
} PosSlot;

void func_0884EEE0(PosSlot *self, int x, int y)
{
    self->dirty = 1;
    self->x = x;
    self->y = y;
}
