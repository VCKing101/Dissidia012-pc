#include "battle/agent31_types.h"

/* Places the banner's two sprites from its layout, capping their alpha at
 * 0x50 while the fighter is not guarding. */
struct PlaceRgba {
    unsigned char r, g, b, a;
};

struct PlaceMatrix {
    float m[16];
};

struct PlaceBanner {
    unsigned char pad_000[0x22C];
    void *layout;
    void *sprite_a;
    void *sprite_b;
    unsigned char pad_238[0x34];
    A31VActor *fighter;
};

extern "C" {

extern void func_088DA0A8(void *layout, int slot, PlaceMatrix *matrix, PlaceRgba *color);
extern void func_088D9B84(void *sprite, PlaceMatrix *matrix);
extern void func_088D9AF4(void *sprite, PlaceRgba *color);

void func_08A3E14C(PlaceBanner *self)
{
    if (self->layout != 0) {
        PlaceMatrix matrix;
        PlaceRgba color;
        func_088DA0A8(self->layout, 1, &matrix, &color);
        if (self->sprite_a != 0) {
            if (self->fighter->guard() == 0 && color.a > 0x50) {
                color.a = 0x50;
            }
            func_088D9B84(self->sprite_a, &matrix);
            func_088D9AF4(self->sprite_a, &color);
        }
        func_088DA0A8(self->layout, 0, &matrix, &color);
        if (self->sprite_b != 0) {
            if (self->fighter->guard() == 0 && color.a > 0x50) {
                color.a = 0x50;
            }
            func_088D9B84(self->sprite_b, &matrix);
            func_088D9AF4(self->sprite_b, &color);
        }
    }
}

}
