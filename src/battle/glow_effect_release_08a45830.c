/* Releases the glow effect: ends its stage, frees its sprite and its two
 * trails, restores the target's draw scale, the camera's range and the
 * screen tint. */
typedef struct GlowTarget {
    unsigned char pad_0000[0x110C];
    float draw_scale;
} GlowTarget;

typedef struct GlowParts {
    unsigned char pad_000[0x2E4];
    float near_range;
    float far_range;
    unsigned char pad_2ec[0x1C];
    void *sprites[1];
    void *trails[2];
} GlowParts;

typedef struct GlowEffect {
    void *actor;
    GlowTarget *target;
    unsigned char pad_08[4];
    void *sheet;
    unsigned char pad_10[4];
    GlowParts *parts;
} GlowEffect;

typedef struct GlowCamera {
    unsigned char pad_000[0xC0];
    unsigned char range[4];
} GlowCamera;

typedef struct GlowScreen {
    unsigned char pad_000[0x190];
    unsigned char shake[4];
} GlowScreen;

extern GlowCamera *D_08B3D6B8;
extern GlowScreen *D_089D4DC8;

extern void func_08A45B5C(GlowEffect *self);
extern void func_088DA060(void *sprite);
extern void func_088A6C70(void *trail);
extern void func_08873594(void *range, float near_range, float far_range);
extern void func_08858ACC(void *shake, int a, int b);

void func_08A45830(GlowEffect *self)
{
    int i;
    func_08A45B5C(self);
    if (self->sheet != 0) {
        for (i = 0; i < 1; i++) {
            if (self->parts->sprites[i] != 0) {
                func_088DA060(self->parts->sprites[i]);
                self->parts->sprites[i] = 0;
            }
        }
    }
    for (i = 0; i < 2; i++) {
        if (self->parts->trails[i] != 0) {
            func_088A6C70(self->parts->trails[i]);
            self->parts->trails[i] = 0;
        }
    }
    self->target->draw_scale = 1.0f;
    func_08873594(D_08B3D6B8->range, self->parts->near_range, self->parts->far_range);
    func_08858ACC(D_089D4DC8->shake, 0, 0);
}
