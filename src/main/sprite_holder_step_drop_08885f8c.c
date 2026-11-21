typedef struct Step08885F8C {
    unsigned short unk_00;
    unsigned short state;              /* +0x02 */
    unsigned short next;               /* +0x04 */
} Step08885F8C;

typedef struct Child08885F8C {
    unsigned char unk_00[0xC];
    short state;                       /* +0x0C */
    unsigned char unk_0E[0x20 - 0xE];
    unsigned short flags;              /* +0x20 */
} Child08885F8C;

typedef struct Holder08885F8C {
    void *sprite;                      /* +0x00 */
    Child08885F8C *child;              /* +0x04 */
    int step;                          /* +0x08 index into D_089C9FD8 */
} Holder08885F8C;

extern Step08885F8C D_089C9FD8[];
extern void func_088DB10C(void *sprite);                 /* sprite_drop_tex */
extern void func_088DA060(Child08885F8C *child);

void func_08885F8C(Holder08885F8C *self)
{
    if (self->sprite != 0 && self->child != 0 && self->child->state == D_089C9FD8[self->step].next &&
        (unsigned char)((self->child->flags & 0x20) != 0)) {
        func_088DB10C(self->sprite);
        if (self->child != 0) {
            func_088DA060(self->child);
            self->child = 0;
        }
    }
}
