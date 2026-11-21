typedef struct Step08885E2C {
    unsigned short start;              /* +0x00 */
    unsigned short state;              /* +0x02 */
    unsigned short next;               /* +0x04 */
} Step08885E2C;

typedef struct Sprite08885E2C {
    unsigned char unk_00[0xE];
    unsigned short visible;            /* +0x0E */
} Sprite08885E2C;

typedef struct Holder08885E2C {
    void *layout;                      /* +0x00 */
    Sprite08885E2C *sprite;            /* +0x04 */
    int step;                          /* +0x08 index into D_089C9FD8 */
} Holder08885E2C;

extern Step08885E2C D_089C9FD8[];
extern void func_088DA060(Sprite08885E2C *sprite);
extern Sprite08885E2C *func_088DAA40(void *layout, int state, int unk);

void func_08885E2C(Holder08885E2C *self, unsigned char shown, unsigned char second)
{
    if (self->layout != 0) {
        Step08885E2C *step;

        if (self->sprite != 0) {
            func_088DA060(self->sprite);
            self->sprite = 0;
        }
        self->step = second != 0;
        step = &D_089C9FD8[self->step];
        if (shown == 0) {
            self->sprite = func_088DAA40(self->layout, step->start, 0);
            self->sprite->visible = D_089C9FD8[self->step].state;
        } else {
            self->sprite = func_088DAA40(self->layout, step->state, 0);
        }
    }
}
