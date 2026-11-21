#include "battle/agent31_types.h"

/* Releases the trail effect: frees its three trails and seven sprites,
 * stops the two trail effects on the fighter's model, marks the screen for
 * a redraw and restores the camera's range. */
template <class T> struct TrailKept {
    T value;
    TrailKept(T v) : value(v) {}
};

struct TrailReleaseParts {
    unsigned char pad_00[0x34];
    float near_range;
    float far_range;
    void *trails[3];
    void *sprites[7];
};

struct TrailReleaseEffect {
    A31VActor *actor;
    unsigned char pad_04[8];
    void *sheet;
    unsigned char pad_10[4];
    TrailReleaseParts *parts;
};

struct TrailScreen {
    unsigned char pad_000[0x6B2];
    unsigned char redraw;
};

struct TrailCamera {
    unsigned char pad_000[0xC0];
    unsigned char range[4];
};

extern TrailScreen *D_089D6AC8;
extern TrailCamera *D_08B3D6B8;

extern "C" {

extern void func_088A6C70(void *trail);
extern void func_088DA060(void *sprite);
extern void func_088AA114(void *bone, int id);
extern void func_08873594(void *range, float near_range, float far_range);

void func_08A47770(TrailReleaseEffect *self)
{
    for (int i = 0; i < 3; i++) {
        if (self->parts->trails[i] != 0) {
            func_088A6C70(self->parts->trails[i]);
            self->parts->trails[i] = 0;
        }
    }
    if (self->sheet != 0) {
        for (int i = 0; i < 7; i++) {
            if (self->parts->sprites[i] != 0) {
                func_088DA060(self->parts->sprites[i]);
                self->parts->sprites[i] = 0;
            }
        }
    }
    A31ModelPart *part = &self->actor->model->part;
    void *bone = part->bone;
    if (bone != 0) {
        TrailKept<void *> saved(bone);
        func_088AA114(bone, 0x66);
        func_088AA114(saved.value, 0x67);
    }
    D_089D6AC8->redraw = 1;
    TrailCamera *camera = D_08B3D6B8;
    func_08873594(camera->range, self->parts->near_range, self->parts->far_range);
}

}
