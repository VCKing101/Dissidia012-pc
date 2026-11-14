/* Rolls the hit effect's outcome for the attacker's level: 0 (the level's
 * chance passed), 2 (the second roll failed too) or 1 with a random size. */
struct RollParts {
    unsigned char pad_00[0x20];
    int outcome;
    float size;
};

struct RollEffect {
    unsigned char pad_00[0x14];
    RollParts *parts;
};

extern int D_09AF8B00;
extern float D_08A30A48;

struct BattleRand {
    static int next()
    {
        return (D_09AF8B00 = D_09AF8B00 * 0x343FD + 0x269EC3) >> 16 & 0x7FFF;
    }
};

extern "C" {

extern int func_08A40730(RollEffect *self);

void func_08A2F4E4(RollEffect *self)
{
    int level = func_08A40730(self);
    int chance[7] = {100, 90, 80, 60, 40, 20, 0};
    if (BattleRand::next() % 101 <= chance[level]) {
        self->parts->outcome = 0;
    } else {
        if (BattleRand::next() % 101 <= chance[level]) {
            self->parts->size = D_08A30A48 - ((float)BattleRand::next() * (1.0f / 32767.0f) * 0.5f + 0.5f);
            self->parts->outcome = 1;
        } else {
            self->parts->outcome = 2;
        }
    }
}

}
