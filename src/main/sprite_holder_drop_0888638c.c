typedef struct Child0888638C {
    unsigned char unk_00[0xC];
    short state;                       /* +0x0C */
    unsigned char unk_0E[0x20 - 0xE];
    unsigned short flags;              /* +0x20 */
} Child0888638C;

typedef struct Holder0888638C {
    void *sprite;                      /* +0x00 */
    Child0888638C *child;              /* +0x04 */
    unsigned int flags;                /* +0x08 bit 0: keep */
} Holder0888638C;

extern void func_088DB10C(void *sprite);                 /* sprite_drop_tex */
extern void func_088DA060(Child0888638C *child);

void func_0888638C(Holder0888638C *self)
{
    if (self->sprite != 0 && self->child != 0 && self->child->state == 2 &&
        (unsigned char)((self->child->flags & 0x20) != 0) && (self->flags & 1) == 0) {
        func_088DB10C(self->sprite);
        if (self->child != 0) {
            func_088DA060(self->child);
            self->child = 0;
        }
    }
}
