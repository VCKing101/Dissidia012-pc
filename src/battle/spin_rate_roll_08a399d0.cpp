/* Rolls the hit effect's spin rate for the attacker's level: within the
 * level's chance the die turns in 28 to 30 frames, otherwise faster (down
 * to 20); the rate is 240 over the frame count. */
struct SpinRateParts {
    unsigned char pad_00[0x1C];
    float rate;
};

struct SpinRateEffect {
    unsigned char pad_00[0x14];
    SpinRateParts *parts;
};

struct FrameRange {
    int most;
    int least;
};

extern int D_09AF8B00;
extern const FrameRange D_08A3B1F8[7];
extern const FrameRange D_08A3B230[7];
extern "C" void *func_08935524(void *dst, const void *src, unsigned int size);

struct SpinRateRand {
    static int next()
    {
        return (D_09AF8B00 = D_09AF8B00 * 0x343FD + 0x269EC3) >> 16 & 0x7FFF;
    }
    static int between(int least, int most)
    {
        return next() % (most - least + 1) + least;
    }
};

extern "C" {

extern int func_08A40730(SpinRateEffect *self);

void func_08A399D0(SpinRateEffect *self)
{
    int level = func_08A40730(self);
    int chance[7] = {100, 90, 80, 60, 40, 20, 0};
    FrameRange slow[7];
    FrameRange fast[7];
    func_08935524(slow, D_08A3B1F8, sizeof(slow));
    func_08935524(fast, D_08A3B230, sizeof(fast));
    if (SpinRateRand::next() % 101 <= chance[level]) {
        self->parts->rate = 240.0f / (float)SpinRateRand::between(slow[level].least, slow[level].most);
    } else {
        self->parts->rate = 240.0f / (float)SpinRateRand::between(fast[level].least, fast[level].most);
    }
}

}
