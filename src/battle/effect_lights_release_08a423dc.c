/* Releases a hit effect: stops the banner's fade, clears the target's hold
 * flag, restores the light, frees the twelve lights and the two sprites. */
typedef struct LightsBanner {
    unsigned char pad_000[0x298];
    float fade;
} LightsBanner;

typedef struct LightsTarget {
    unsigned char pad_000[0x384];
    unsigned int flags;
} LightsTarget;

typedef struct LightsParts {
    unsigned char pad_000[0x180];
    void *lights[12];
    unsigned char pad_1b0[0x10];
    void *sprites[2];
} LightsParts;

typedef struct LightsEffect {
    void *actor;
    LightsTarget *target;
    unsigned char pad_08[4];
    void *sheet;
    unsigned char pad_10[4];
    LightsParts *parts;
} LightsEffect;

extern LightsBanner *D_08A405E0;
extern void *D_089D6998;

extern void func_0887E0DC(void *lights, int mode);
extern void func_0887BA5C(void *light);
extern void func_088DA060(void *sprite);

void func_08A423DC(LightsEffect *self)
{
    int i;
    D_08A405E0->fade = 0.0f;
    self->target->flags &= ~0x1000;
    func_0887E0DC(D_089D6998, 1);
    for (i = 0; i < 12; i++) {
        if (self->parts->lights[i] != 0) {
            func_0887BA5C(self->parts->lights[i]);
            self->parts->lights[i] = 0;
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
