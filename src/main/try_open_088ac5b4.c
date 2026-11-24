/* Starts a request on the object: func_088AB800(self, 1); depending on func_08867894 uses func_08A489D8/func_08A48808 or func_08A489A0/func_08A48750 with a local handle; sets bit 0x20 of +0xEF8 and returns 1 when the open fails, else clears it and returns 0. */
typedef struct Obj088ac5b4 { unsigned char pad0[0xEF8]; unsigned flags; unsigned char pad1[0xF08 - 0xEFC]; unsigned char buf[1]; } Obj088ac5b4;
extern int D_08A49748;
extern void func_088AB800(Obj088ac5b4 *o, int a);
extern int func_08867894(void);
extern int func_08A489D8(void *h, int a);
extern void func_08A48808(void *h, void *buf, int a, int b, int c);
extern int func_08A489A0(void *h, int a);
extern void func_08A48750(void *h, void *buf, int a, int b, int c);
unsigned char func_088AC5B4(Obj088ac5b4 *self, int a, int b)
{
    int h[1];
    int extra;
    func_088AB800(self, 1);
    extra = D_08A49748;
    if (func_08867894() != 0) {
        if (func_08A489A0(h, a) != 0) {
            func_08A48750(h, self->buf, a, b, extra);
            self->flags = self->flags & 0xFFFFFFDF;
            return (self->flags & 0x20) != 0;
        } else {
            self->flags = self->flags | 0x20;
            return (self->flags & 0x20) != 0;
        }
    } else {
        if (func_08A489D8(h, a) != 0) {
            func_08A48808(h, self->buf, a, b, extra);
            self->flags = self->flags & 0xFFFFFFDF;
            return (self->flags & 0x20) != 0;
        } else {
            self->flags = self->flags | 0x20;
            return (self->flags & 0x20) != 0;
        }
    }
}
