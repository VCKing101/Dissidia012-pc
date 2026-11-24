// Sets the byte at +0x24C to 1 and, by the word at +0xA0 (0 or 1), calls func_088D9CD8 / func_088D9CFC on the two pointers in opposite order.
extern void func_088D9CD8(void *);
extern void func_088D9CFC(void *);
typedef struct { void *p0; void *p4; char pad0[0xA0 - 8]; int mode; char pad1[0x24C - 0xA4]; unsigned char flag; } Obj08B4B504;
void func_08B4B504(Obj08B4B504 *self)
{
    int one = 1;
    self->flag = one;
    switch (self->mode) {
    case 0:
        func_088D9CD8(self->p0);
        func_088D9CFC(self->p4);
        break;
    case 1:
        func_088D9CFC(self->p0);
        func_088D9CD8(self->p4);
        break;
    }
}
