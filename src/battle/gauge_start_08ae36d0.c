typedef struct GaugeLimits {
    char pad_00[0x18];
    float start;
} GaugeLimits;

typedef struct GaugeOwner {
    char pad_000[0x260];
    unsigned char started;
    float value;
} GaugeOwner;

extern GaugeLimits *D_08B4AF14;

void func_08AE36D0(GaugeOwner *self)
{
    GaugeLimits *limits = D_08B4AF14;
    self->started = 1;
    self->value = limits->start;
}

