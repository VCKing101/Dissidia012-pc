/* Loads the hit effect's sprites from its sheet: the flash, the core, four
 * sets of three sparks, and the ring. */
struct EffectSprite {
    unsigned char pad_00[0xE];
    short frame;
};

struct EffectParts {
    unsigned char pad_00[0x28];
    EffectSprite *ring;
    void *flash;
    void *sparks_c[4];
    void *core;
    void *sparks_b[4];
    void *sparks_a[4];
};

struct HitEffect {
    unsigned char pad_00[0xC];
    void *sheet;
    unsigned char pad_10[4];
    EffectParts *parts;
};

extern "C" {

extern void *func_088DAA40(void *sheet, int frame, int flags);
extern void func_088D9B48(void *sprite, int color);
extern void *func_088DAA68(void *sheet);

void func_08A2F3F8(HitEffect *self)
{
    self->parts->flash = func_088DAA40(self->sheet, 0x2D, 0);
    func_088D9B48(self->parts->flash, 0xFFFFFF);
    self->parts->core = func_088DAA68(self->sheet);
    for (int i = 0; i < 4; i++) {
        self->parts->sparks_a[i] = func_088DAA68(self->sheet);
        self->parts->sparks_b[i] = func_088DAA68(self->sheet);
        self->parts->sparks_c[i] = func_088DAA68(self->sheet);
    }
    self->parts->ring = (EffectSprite *)func_088DAA40(self->sheet, 0x2E, 0);
    self->parts->ring->frame = 0x2F;
}

}
