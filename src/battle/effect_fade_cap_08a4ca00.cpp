#include "battle/agent31_types.h"

/* Hit effect update: runs the hit stage when due and the first side's
 * extra stage, and while the fighter is not guarding caps the glow
 * sprites' alpha at 0x50. */
struct CapRgba {
    unsigned char r, g, b, a;
};

struct CapSprite {
    unsigned char pad_00[0x28];
    unsigned int color;

    static void get_color(CapSprite *sprite, CapRgba *c)
    {
        unsigned int *word = &sprite->color;
        unsigned int value = *word;
        c->r = value & 0xFF;
        c->g = value >> 8 & 0xFF;
        c->b = value >> 16 & 0xFF;
        c->a = value >> 24 & 0xFF;
    }
};

struct CapParts {
    unsigned char pad_000[0x2C];
    CapSprite *glow[2];
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
extern void func_08A4CB1C(CapEffect *self);
extern void func_08A4CCE8(CapEffect *self);
extern void func_088D9AF4(void *sprite, CapRgba *color);

void func_08A4CA00(CapEffect *self)
{
    if (func_08A406C8(self) == 1) {
        func_08A4CB1C(self);
    }
    if (self->second == 0) {
        func_08A4CCE8(self);
    }
    if (self->actor->guard() == 0 && self->sheet != 0) {
        for (int i = 0; i < 2; i++) {
            if (self->parts->glow[i] != 0) {
                CapRgba color;
                CapSprite::get_color(self->parts->glow[i], &color);
                if (color.a > 0x50) {
                    color.a = 0x50;
                }
                func_088D9AF4(self->parts->glow[i], &color);
            }
        }
    }
}

}
