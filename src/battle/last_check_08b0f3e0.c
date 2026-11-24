// Returns 1 when the mode at +0x2C is 2 or the index at +0x28 equals the count at +0x24 minus 1.
typedef struct { char pad[0x24]; int count; int idx; int mode; } Obj08B0F3E0;
unsigned char func_08B0F3E0(Obj08B0F3E0 *self)
{
    int r = 0;
    if (self->mode == 2 || self->idx == self->count - 1) {
        r = 1;
    }
    return r;
}
