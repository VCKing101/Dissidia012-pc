/* 1 when the unit holds a positive charge and has a charging flag set. */
struct Charge08837764 {
    int field_00;
    short amount;

    static bool positive(Charge08837764 *c) { return c->amount > 0; }
};

struct Unit08837764 {
    char pad_000[0x388];
    unsigned long long flags;
    char pad_390[0xDC];
    Charge08837764 *charge;

    static bool any(Unit08837764 *u, unsigned long long mask) { return (u->flags & mask) != 0; }
};

extern "C" {

extern unsigned long long D_089C7FB0;

unsigned char func_08837764(Unit08837764 *u)
{
    int result = 0;
    if (u->charge != 0 && Charge08837764::positive(u->charge) && Unit08837764::any(u, D_089C7FB0)) {
        result = 1;
    }
    return result;
}

}
