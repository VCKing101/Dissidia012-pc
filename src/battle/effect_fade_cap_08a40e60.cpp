#include "battle/agent31_types.h"

/* Panel effect update: runs the hit stage when due and the stage for the
 * current side, and while the fighter is not guarding caps the six glow
 * sprites' alpha at 0x50. */
struct FadeRgba {
    unsigned char r, g, b, a;
    FadeRgba() {}
    FadeRgba(unsigned int value)
    {
        r = value & 0xFF;
        g = value >> 8 & 0xFF;
        b = value >> 16 & 0xFF;
        a = value >> 24 & 0xFF;
    }
};

struct FadeSprite {
    unsigned char pad_00[0x28];
    unsigned int color;
};

struct FadeParts {
    unsigned char pad_000[0x44];
    FadeSprite *glow[6];
};

struct FadeEffect {
    A31VActor *actor;
    unsigned char pad_04[4];
    int second;
    void *sheet;
    unsigned char pad_10[4];
    FadeParts *parts;
};

extern "C" {

extern int func_08A406C8(FadeEffect *self);
extern void func_08A41CE0(FadeEffect *self);
extern void func_08A41558(FadeEffect *self);
extern void func_08A41774(FadeEffect *self);
extern void func_088D9AF4(void *sprite, FadeRgba *color);

void func_08A40E60(FadeEffect *self)
{
    if (func_08A406C8(self)) {
        func_08A41CE0(self);
    }
    if (self->second == 0) {
        func_08A41558(self);
    }
    if (self->second == 1) {
        func_08A41774(self);
    }
    if (self->actor->guard() == 0 && self->sheet != 0) {
        for (int i = 0; i < 6; i++) {
            if (self->parts->glow[i] != 0) {
                FadeRgba color;
                {
                    unsigned int *word = &self->parts->glow[i]->color;
                    FadeRgba value(*word);
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
