typedef struct BonusResult {
    unsigned int flags;
    char pad_04[0x10];
    int total;
    int bonus;
} BonusResult;

typedef struct BonusOwner {
    char pad_00[0x28];
    unsigned int flags;

    static int capped(int value)
    {
        int limit = 9999;
        if (value > limit) {
            value = limit;
        }
        return value;
    }
} BonusOwner;

typedef struct BonusScene {
    int multiplier;
} BonusScene;

extern "C" {

extern BonusScene *D_08AE7CA0;
extern int func_08AE3C70(BonusOwner *self, BonusResult *out);

/* Double the owner's base value (capped at 9999) into the result. */
int func_08AE9488(BonusOwner *self, BonusResult *out)
{
    int base = func_08AE3C70(self, out);
    int total = BonusOwner::capped(base + base);
    out->total = total;
    out->bonus = total - base;
    out->flags |= 0x100;
    self->flags |= 4;
    D_08AE7CA0->multiplier = D_08AE7CA0->multiplier + D_08AE7CA0->multiplier;
    return 1;
}

}
