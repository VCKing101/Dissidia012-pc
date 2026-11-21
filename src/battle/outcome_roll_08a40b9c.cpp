/* Rolls the panel's two outcomes for the attacker's level (2 within the
 * first chance, 1 within the next, else unchanged) and a random scale in
 * the level's range for each. */
struct RollOdds {
    int first;
    int second;
};

struct RollRange {
    float least;
    float most;
};

struct OutcomeParts {
    unsigned char pad_00[0x18];
    int outcomes[2];
    float scales[2];
};

struct OutcomeEffect {
    unsigned char pad_00[0x14];
    OutcomeParts *parts;
};

extern int D_09AF8B00;
extern const RollOdds D_08A41FF0[7];
extern const RollRange D_08A42028[7];

struct OutcomeRand {
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

extern int func_08A40730(OutcomeEffect *self);
extern void *func_08935524(void *dst, const void *src, unsigned int size);

void func_08A40B9C(OutcomeEffect *self)
{
    int level = func_08A40730(self);
    RollOdds odds[7];
    RollRange ranges[7];
    func_08935524(odds, D_08A41FF0, sizeof(odds));
    func_08935524(ranges, D_08A42028, sizeof(ranges));
    int i;
    for (i = 0; i < 2; i++) {
        int roll = OutcomeRand::next() % 101;
        if (roll <= odds[level].first) {
            self->parts->outcomes[i] = 2;
        } else if (roll <= odds[level].first + odds[level].second) {
            self->parts->outcomes[i] = 1;
        }
        self->parts->scales[i] = OutcomeRand::between(ranges[level].least, ranges[level].most);
    }
}

}
