// Damage scale: starts at 1.0, multiplied by HP-ratio and gauge-based percent bonuses and a global factor when the three checks hold.
typedef struct Stat57 {
    char pad0[8];
    int cur;
    int max;
    char pad1[8];
    float rate;
} Stat57;
typedef struct Prof57 {
    char pad[9];
    unsigned char a;
    unsigned char b;
} Prof57;
typedef struct Fighter57 {
    char pad0[0x39C];
    Prof57 *prof;
    char pad1[0x52C - 0x3A0];
    Stat57 *stat;
} Fighter57;
extern int func_08B345F8(Fighter57 *);
extern int func_08B34650(Fighter57 *);
extern int func_08B346B0(Fighter57 *);
extern float D_08B3ABD8;
float func_08B34244(Fighter57 *self)
{
    float scale = 1.0f;
    if (func_08B345F8(self) != 0) {
        int max = self->stat->max;
        int lost = max - self->stat->cur;
        lost = lost < 0 ? 0 : lost;
        scale = (float)(lost * 100 / max * self->prof->a + 100) * 0.01f * scale;
    }
    if (func_08B34650(self) != 0) {
        scale = (float)((int)(self->stat->rate * 100.0f) / 10000 * self->prof->b + 100) * 0.01f * scale;
    }
    if (func_08B346B0(self) != 0) {
        scale = D_08B3ABD8 * scale;
    }
    return scale;
}
