#include "battle/agent31_types.h"

/* Places the guard mark's sprites from its layouts (the layout slot follows
 * the fighter's guard state), sets the glow scale, and while the partner
 * guards caps the five ring sprites' alpha at 0x50. */
struct MarkRgba {
    unsigned char r, g, b, a;
    MarkRgba() {}
    MarkRgba(unsigned int value)
    {
        r = value & 0xFF;
        g = value >> 8 & 0xFF;
        b = value >> 16 & 0xFF;
        a = value >> 24 & 0xFF;
    }
};

struct MarkMatrix {
    float m[16];
};

struct MarkSprite {
    unsigned char pad_00[0x28];
    unsigned int color;
};

struct MarkEffect {
    unsigned char pad_000[0x228];
    MarkSprite *parts[9];
    unsigned char pad_24c[8];
    float glow_scale;
    unsigned char pad_258[0x14];
    A31VActor *partner;
    A31VActor *fighter;
};

extern "C" {

extern void func_088DA0A8(void *layout, int slot, MarkMatrix *matrix, MarkRgba *color);
extern void func_088D9B84(void *sprite, MarkMatrix *matrix);
extern void func_088D9AEC(void *sprite, float scale);
extern void func_088D9AF4(void *sprite, MarkRgba *color);

void func_08A3DFD0(MarkEffect *self)
{
    MarkMatrix matrix;
    func_088DA0A8(self->parts[4], self->fighter->guard() != 0, &matrix, 0);
    func_088D9B84(self->parts[5], &matrix);
    func_088DA0A8(self->parts[5], 1, &matrix, 0);
    func_088D9B84(self->parts[7], &matrix);
    func_088D9B84(self->parts[8], &matrix);
    func_088DA0A8(self->parts[5], 0, &matrix, 0);
    func_088D9B84(self->parts[6], &matrix);
    func_088D9AEC(self->parts[6], self->glow_scale);
    if (self->partner->guard()) {
        for (int i = 4; i < 9; i++) {
            if (self->parts[i] == 0) {
                continue;
            }
            MarkRgba color;
            {
                unsigned int *word = &self->parts[i]->color;
                MarkRgba value(*word);
                color = value;
            }
            if (color.a > 0x50) {
                color.a = 0x50;
            }
            func_088D9AF4(self->parts[i], &color);
        }
    }
}

}
