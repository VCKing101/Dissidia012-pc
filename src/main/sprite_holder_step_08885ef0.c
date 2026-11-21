typedef struct Step08885EF0 {
    unsigned short unk_00;
    unsigned short state;              /* +0x02 */
    unsigned short next;               /* +0x04 */
} Step08885EF0;

typedef struct Child08885EF0 {
    unsigned char unk_00[0xC];
    short state;                       /* +0x0C */
} Child08885EF0;

typedef struct Holder08885EF0 {
    void *sprite;                      /* +0x00 */
    Child08885EF0 *child;              /* +0x04 */
    int step;                          /* +0x08 index into D_089C9FD8 */
} Holder08885EF0;

extern Step08885EF0 D_089C9FD8[];
extern void func_088D9F5C(Child08885EF0 *child, unsigned short state, int unk);
extern void func_088DB10C(void *sprite);                 /* sprite_drop_tex */
extern void func_088DA060(Child08885EF0 *child);

void func_08885EF0(Holder08885EF0 *self)
{
    if (self->sprite != 0 && self->child != 0) {
        if (self->child->state == D_089C9FD8[self->step].state) {
            func_088D9F5C(self->child, D_089C9FD8[self->step].next, 0);
        } else {
            func_088DB10C(self->sprite);
            if (self->child != 0) {
                func_088DA060(self->child);
                self->child = 0;
            }
        }
    }
}
