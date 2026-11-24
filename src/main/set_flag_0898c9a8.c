/* Calls func_0898CCA8(self) then stores whether the byte at 0x1C of other is non-zero at 0x10 of self. */
extern void func_0898CCA8(void *);
typedef struct { char pad[0x10]; int flag; } Dst0898C9A8;
typedef struct { char pad[0x1C]; unsigned char b; } Src0898C9A8;
void func_0898C9A8(Dst0898C9A8 *self, Src0898C9A8 *other) {
    Src0898C9A8 *o = other;
    Dst0898C9A8 *d = self;
    func_0898CCA8(d);
    d->flag = o->b != 0;
}
