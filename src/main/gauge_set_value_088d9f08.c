/* Stores the gauge value and keeps the "below limit" flag (bit 2 at 0x20) in step. */
typedef struct Gauge {
    float limit;
    float value;
    char pad_08[0x20 - 8];
    unsigned short flags;
} Gauge;

void func_088D9F08(Gauge *gauge, float value)
{
    gauge->value = value;
    if (!(gauge->value <= gauge->limit)) {
        gauge->flags &= ~4;
    } else if (gauge->value < gauge->limit) {
        gauge->flags |= 4;
    }
}
