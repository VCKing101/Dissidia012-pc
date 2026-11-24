/* Sets or clears the 0x4000 flag on the page's child records depending on the current choice. */
typedef struct Ch { char pad0[0x20]; unsigned short flags; } Ch;
typedef struct Sf { char pad0[0x14]; int choice; char pad1[4]; Ch *c1C; Ch *c20; char pad2[0x90 - 0x24]; int count; } Sf;
extern void func_088D9D24(Ch *);
extern void func_08A65F2C(Sf *, int);
void func_08A6610C(Sf *self) {
    Ch *c;
    int i;
    switch (self->choice) {
    case 0:
        i = 0;
        do {
            c = self->c1C;
            c->flags = c->flags | 0x4000;
            func_088D9D24(c);
            i++;
            self = (Sf *)((char *)self + 4);
        } while (i < 7);
        break;
    case 1:
        c = self->c20;
        c->flags = c->flags & ~0x4000;
        func_088D9D24(c);
        c = self->c1C;
        c->flags = c->flags | 0x4000;
        func_088D9D24(c);
        func_08A65F2C(self, self->count - 1);
        break;
    case 2:
        c = self->c20;
        c->flags = c->flags | 0x4000;
        func_088D9D24(c);
        c = self->c1C;
        c->flags = c->flags & ~0x4000;
        func_088D9D24(c);
        func_08A65F2C(self, self->count - 1);
        break;
    }
}
