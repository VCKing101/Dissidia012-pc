#include "battle/agent31_types.h"

/* Releases a hit effect: clears the target's hold flag, restores the
 * light, frees the six lights and the two sprites. */
struct ReleaseLight {
    void *light;
    int pad_04;
};

struct LightsParts {
    unsigned char pad_00[0x10];
    ReleaseLight lights[6];
    unsigned char pad_40[0x14];
    void *sprites[2];
};

struct LightsEffect {
    A31VActor *actor;
    A31VActor *target;
    unsigned char pad_08[4];
    void *sheet;
    unsigned char pad_10[4];
    LightsParts *parts;
};

extern void *D_089D6998;

extern "C" {

extern void func_0887E0DC(void *lights, int mode);
extern void func_0887BA5C(void *light);
extern void func_088DA060(void *sprite);

void func_08A3898C(LightsEffect *self)
{
    int i;
    self->target->flags &= ~0x1000;
    func_0887E0DC(D_089D6998, 1);
    for (i = 0; i < 6; i++) {
        if (self->parts->lights[i].light != 0) {
            func_0887BA5C(self->parts->lights[i].light);
            self->parts->lights[i].light = 0;
        }
    }
    if (self->sheet != 0) {
        for (i = 0; i < 2; i++) {
            if (self->parts->sprites[i] != 0) {
                func_088DA060(self->parts->sprites[i]);
                self->parts->sprites[i] = 0;
            }
        }
    }
}

}
