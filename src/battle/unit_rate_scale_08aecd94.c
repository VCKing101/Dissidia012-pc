typedef struct UnitRates {
    char pad_00[0x14];
    int rate;
} UnitRates;

typedef struct RateUnit {
    char pad_0000[0x1084];
    UnitRates rates;
} RateUnit;

/* base scaled by the unit's rate percentage. */
int func_08AECD94(void *self, RateUnit *unit, int base)
{
    UnitRates *rates = &unit->rates;
    return rates->rate * base / 100;
}
