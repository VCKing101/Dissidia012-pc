typedef struct GaugeUnit {
    char pad_000[0x3C0];
    float gauge;
    char pad_3C4[0x3];
    signed char state;
} GaugeUnit;

extern float D_08A85BF0;

/* True when the gauge is below the limit and the unit is in state 5. */
unsigned char func_08A841B8(GaugeUnit *unit)
{
    int result;

    if (unit == 0) {
        return 0;
    }
    result = 0;
    if (unit->gauge < D_08A85BF0 && unit->state == 5) {
        result = 1;
    }
    return result;
}
