typedef struct GaugeOwner {
    unsigned char active;
    unsigned char field_01;
    unsigned char full;
} GaugeOwner;

extern float func_08A66820(GaugeOwner *self);
extern float func_08A66744(GaugeOwner *self);

int func_08A7027C(GaugeOwner *self, void *a, void *b, void *c)
{
    float value;
    if (a == 0 || b == 0 || c == 0) {
        return -1;
    }
    value = func_08A66820(self);
    if (value <= func_08A66744(self)) {
        self->full = 1;
    }
    return 0;
}
