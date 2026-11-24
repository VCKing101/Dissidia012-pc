/* Sets or clears the 0x4000 flag on two child records by the current choice, notifying after each. */
typedef struct Ch { char pad0[0x20]; unsigned short flags; } Ch;
typedef struct Tf { unsigned char active; char pad1[0x23]; Ch *c24; Ch *c28; char pad2[0x6C - 0x2C]; int choice; } Tf;
extern void func_088D9D24(Ch *);
void func_08A64F20(Tf *self) {
    Ch *c;
    if (self->active != 0) {
        switch (self->choice) {
        case 0:
            c = self->c24;
            c->flags = c->flags & ~0x4000;
            func_088D9D24(c);
            c = self->c28;
            c->flags = c->flags | 0x4000;
            func_088D9D24(c);
            break;
        case 1:
            c = self->c24;
            c->flags = c->flags | 0x4000;
            func_088D9D24(c);
            c = self->c28;
            c->flags = c->flags & ~0x4000;
            func_088D9D24(c);
            break;
        }
    } else {
        c = self->c24;
        c->flags = c->flags | 0x4000;
        func_088D9D24(c);
        c = self->c28;
        c->flags = c->flags | 0x4000;
        func_088D9D24(c);
    }
}
