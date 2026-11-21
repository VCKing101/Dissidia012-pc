/* Rolls the glow effect's ranges for the attacker's level (the size and
 * speed ranges from two level tables) and a random size in the first. */
struct GlowRange {
    float least;
    float most;
};

struct GlowRollParts {
    unsigned char pad_000[0x314];
    GlowRange size;
    GlowRange speed;
    unsigned char pad_324[8];
    int time;
    float value;
};

struct GlowRollEffect {
    unsigned char pad_00[0x14];
    GlowRollParts *parts;
};

extern int D_09AF8B00;
extern const GlowRange D_08A472F8[7];
extern const GlowRange D_08A47330[7];

struct GlowRand {
    static int next()
    {
        return (D_09AF8B00 = D_09AF8B00 * 0x343FD + 0x269EC3) >> 16 & 0x7FFF;
    }
    static float between(float least, float most)
    {
        return (float)next() * (1.0f / 32767.0f) * (most - least) + least;
    }
};

extern "C" {

extern int func_08A40730(GlowRollEffect *self);
extern void *func_08935524(void *dst, const void *src, unsigned int size);

void func_08A45A5C(GlowRollEffect *self)
{
    int level = func_08A40730(self);
    GlowRange sizes[7];
    GlowRange speeds[7];
    func_08935524(sizes, D_08A472F8, sizeof(sizes));
    func_08935524(speeds, D_08A47330, sizeof(speeds));
    self->parts->size.least = sizes[level].least;
    self->parts->size.most = sizes[level].most;
    self->parts->speed.least = speeds[level].least;
    self->parts->speed.most = speeds[level].most;
    self->parts->value = GlowRand::between(self->parts->size.least, self->parts->size.most);
    self->parts->time = 0;
}

}
