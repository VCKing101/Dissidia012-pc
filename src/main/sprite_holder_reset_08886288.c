typedef struct Sprite08886288 {
    unsigned char unk_00[0xE];
    short visible;                     /* +0x0E */
} Sprite08886288;

typedef struct Holder08886288 {
    void *layout;                      /* +0x00 */
    Sprite08886288 *sprite;            /* +0x04 */
} Holder08886288;

extern void func_088DA060(Sprite08886288 *sprite);
extern Sprite08886288 *func_088DAA40(void *layout, int a, int b);

void func_08886288(Holder08886288 *self, unsigned char hidden)
{
    if (self->layout != 0) {
        if (self->sprite != 0) {
            func_088DA060(self->sprite);
            self->sprite = 0;
        }
        if (hidden == 0) {
            Sprite08886288 *sprite = func_088DAA40(self->layout, 0, 0);
            self->sprite = sprite;
            sprite->visible = 1;
        } else {
            self->sprite = func_088DAA40(self->layout, 1, 0);
        }
    }
}
