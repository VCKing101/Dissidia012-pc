#include "battle/agent31_types.h"

/* Hit effect update: runs the hit stage when due and the extra stage,
 * and while the fighter is not guarding caps the eight glow sprites' alpha
 * at 0x50. */
struct CapRgba {
    unsigned char r, g, b, a;
    CapRgba() {}
    CapRgba(unsigned int value)
    {
        r = value & 0xFF;
        g = value >> 8 & 0xFF;
        b = value >> 16 & 0xFF;
        a = value >> 24 & 0xFF;
    }
};

struct CapSprite {
    unsigned char pad_00[0x28];
    unsigned int color;
};

struct CapParts {
    unsigned char pad_000[0x88];
    CapSprite *glow[8];
};

struct CapEffect {
    A31VActor *actor;
    unsigned char pad_04[4];
    int second;
    void *sheet;
    unsigned char pad_10[4];
    CapParts *parts;
};

extern "C" {

extern int func_08A406C8(CapEffect *self);
extern void func_08A36114(CapEffect *self);
extern void func_08A36638(CapEffect *self);
extern void func_088D9AF4(void *sprite, CapRgba *color);

void func_08A3652C(CapEffect *self)
{
    if (func_08A406C8(self)) {
        func_08A36114(self);
    }
    func_08A36638(self);
    if (self->actor->guard() == 0 && self->sheet != 0) {
        for (int i = 0; i < 8; i++) {
            if (self->parts->glow[i] != 0) {
                CapRgba color;
                {
                    unsigned int *word = &self->parts->glow[i]->color;
                    CapRgba value(*word);
                    color = value;
                }
                if (color.a > 0x50) {
                    color.a = 0x50;
                }
                func_088D9AF4(self->parts->glow[i], &color);
            }
        }
    }
}

}
