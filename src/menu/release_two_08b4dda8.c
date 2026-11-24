/* When the byte at +0xA0 is clear, calls func_0887B944 on the objects at +0x6C and +0x88 if set. */
struct Self { char pad[0x6C]; void *a; char pad2[0x88 - 0x70]; void *b; char pad3[0xA0 - 0x8C]; unsigned char hold; };
extern void func_0887B944(void *p);
void func_08B4DDA8(struct Self *self)
{
    if (self->hold == 0) {
        if (self->a != 0) { func_0887B944(self->a); }
        if (self->b != 0) { func_0887B944(self->b); }
    }
}
