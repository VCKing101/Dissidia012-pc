#include "battle/agent31_types.h"

/* Burst effect setup: takes its record, clears it, saves the camera's
 * range and stops the eight burst effects still playing on the fighter. */
struct BurstSetupParts {
    unsigned char pad_00[0xC];
    float near_range;
    float far_range;
    int target;
    unsigned char pad_18[0x38];
};

struct BurstSetupEffect {
    A31VActor *actor;
    unsigned char pad_04[0x10];
    BurstSetupParts *parts;
};

struct BurstSetupCamera {
    unsigned char pad_000[0xC0];
    unsigned char range[4];
};

struct BurstEffectId {
    int id;
    int pad_04;
};

extern BurstSetupCamera *D_08B3D6B8;
extern BurstEffectId D_08A4F08C[8];

extern "C" {

extern void func_08935828(void *dst, int value, int size);
extern float func_0887357C(void *range);
extern float func_08873584(void *range);
extern void *func_0884F75C(void *effects, int id);
extern void func_0884EF7C(void *effect);

void func_08A4DAC8(BurstSetupEffect *self, BurstSetupParts *parts)
{
    BurstEffectId *ids = D_08A4F08C;
    self->parts = parts;
    func_08935828(parts, 0, sizeof(BurstSetupParts));
    self->parts->target = -1;
    void *range = D_08B3D6B8->range;
    self->parts->near_range = func_0887357C(range);
    self->parts->far_range = func_08873584(range);
    void *effects = self->actor->effects;
    int i = 0;
    for (BurstEffectId *id = &ids[i]; i < 8; i++, id++) {
        void *effect = func_0884F75C(effects, id->id);
        if (effect != 0) {
            func_0884EF7C(effect);
        }
    }
}
}
