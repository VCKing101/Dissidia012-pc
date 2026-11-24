// For each of the handles at +0xC, +0x10 and +0x30: sets bit 5 of its short flags at +0x20, calls func_088D9D24 then func_088D9CFC on it.
extern void func_088D9D24(void *);
extern void func_088D9CFC(void *);
typedef struct { char pad[0x20]; unsigned short flags; } Handle08B14828;
typedef struct { char pad0[0xC]; Handle08B14828 *h0c; Handle08B14828 *h10; char pad1[0x30 - 0x14]; Handle08B14828 *h30; } Obj08B14828;
void func_08B14828(Obj08B14828 *self)
{
    Handle08B14828 *h = self->h0c;
    if (h != 0) {
        h->flags |= 0x20;
        func_088D9D24(h);
        func_088D9CFC(self->h0c);
    }
    h = self->h10;
    if (h != 0) {
        h->flags |= 0x20;
        func_088D9D24(h);
        func_088D9CFC(self->h10);
    }
    h = self->h30;
    if (h != 0) {
        h->flags |= 0x20;
        func_088D9D24(h);
        func_088D9CFC(self->h30);
    }
}
