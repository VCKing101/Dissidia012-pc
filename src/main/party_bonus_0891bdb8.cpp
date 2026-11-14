/* Party bonus for the current mode: base + extra in mode 0, the single bonus
   in mode 1. The bonus bytes are read through a decayed array pointer in
   in-class helpers (retail keeps `addiu a0, 0x2A04` unfolded). */
extern "C" {

typedef struct Party {
    signed char field_00;
    signed char mode;               /* 0x0001 */
    char pad_0002[0x2A04 - 2];
    signed char bonus[0x10];        /* 0x2A04: [0] base, [1] single, [0xD] extra */
} Party;

struct PartyBonusOps {
    static int total(signed char *b) { return b[0] + b[0xD]; }
    static int single(signed char *b) { return b[1]; }
};

int func_0891BDB8(Party *party)
{
    int r = 0;
    switch (party->mode) {
    case 0:
        r = PartyBonusOps::total(party->bonus);
        break;
    case 1:
        r = PartyBonusOps::single(party->bonus);
        break;
    }
    return r;
}

}
