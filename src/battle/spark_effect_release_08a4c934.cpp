#include "battle/agent31_types.h"

/* Releases the spark effect: frees its two sprites, stops the spark on the
 * fighter's model, the screen shake and restores the camera's range. */
struct SparkReleaseParts {
    unsigned char pad_00[0x2C];
    void *sprites[2];
    float near_range;
    float far_range;
};

struct SparkReleaseEffect {
    A31VActor *actor;
    unsigned char pad_04[8];
    void *sheet;
    unsigned char pad_10[4];
    SparkReleaseParts *parts;
};

struct SparkScreen {
    unsigned char pad_000[0x190];
    unsigned char shake[4];
};

struct SparkCamera {
    unsigned char pad_000[0xC0];
    unsigned char range[4];
};

extern SparkScreen *D_089D4DC8;
extern SparkCamera *D_08B3D6B8;

extern "C" {

extern void func_088DA060(void *sprite);
extern void func_088AA114(void *bone, int id);
extern void func_08858ACC(void *shake, int a, int b);
extern void func_08873594(void *range, float near_range, float far_range);

void func_08A4C934(SparkReleaseEffect *self)
{
    int i;
    if (self->sheet != 0) {
        for (i = 0; i < 2; i++) {
            if (self->parts->sprites[i] != 0) {
                func_088DA060(self->parts->sprites[i]);
                self->parts->sprites[i] = 0;
            }
        }
    }
    A31ModelPart *part = &self->actor->model->part;
    void *bone = part->bone;
    if (bone != 0) {
        func_088AA114(bone, 0xF);
    }
    func_08858ACC(D_089D4DC8->shake, 0, 0);
    SparkCamera *camera = D_08B3D6B8;
    func_08873594(camera->range, self->parts->near_range, self->parts->far_range);
}

}
