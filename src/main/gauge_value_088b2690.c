/* Scales a gauge value: proportional up to the limit, then linear growth; capped at the table maximum. */
typedef struct GaugeTable088B2690 {
    char pad_00[8];
    float max;
    float rate;
} GaugeTable088B2690;

extern GaugeTable088B2690 *D_089D72AC;

float func_088B2690(float base, int value, int limit)
{
    float v;
    float max;
    if (value <= limit) {
        v = base / limit * value;
    } else {
        v = base + (value - limit) * D_089D72AC->rate;
    }
    max = D_089D72AC->max;
    return v < max ? v : max;
}
