/* Rolls the trail effect's strength for the attacker's level: up to three
 * tries at the level's chance, the first success giving 3, 2 or 1; then a
 * random 0 to 1 spread in hundredths. */
struct StrengthParts {
    unsigned char pad_00[0x20];
    int strength;
    float spread;
};

struct StrengthEffect {
    unsigned char pad_00[0x14];
    StrengthParts *parts;
};

extern int D_09AF8B00;

struct StrengthRand {
    static int next()
    {
        return (D_09AF8B00 = D_09AF8B00 * 0x343FD + 0x269EC3) >> 16 & 0x7FFF;
    }
};

extern "C" {

extern int func_08A40730(StrengthEffect *self);

void func_08A4799C(StrengthEffect *self)
{
    int level = func_08A40730(self);
    int chance[7] = {100, 90, 80, 60, 40, 20, 0};
    for (int n = 3; n > 0; n--) {
        if (StrengthRand::next() % 101 <= chance[level]) {
            self->parts->strength = n;
            break;
        }
    }
    self->parts->spread = (float)(StrengthRand::next() % 101) / 100.0f;
}

}
