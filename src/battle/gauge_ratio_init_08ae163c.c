typedef struct GaugeRatio {
    float ratio;
    float field_04;
    float field_08;
} GaugeRatio;

typedef struct GaugeOwner {
    char pad_000[0x254];
    GaugeRatio gauge;
    char pad_260[0x48];
    char panel_2A8[1];
} GaugeOwner;

typedef struct GaugeLimits {
    char pad_00[0x10];
    float max;
} GaugeLimits;

extern GaugeLimits *D_08B4AF14;
extern int func_08AE0ADC(GaugeOwner *self);
extern void func_08ADFC84(void *panel);

void func_08AE163C(GaugeOwner *self)
{
    GaugeRatio *gauge = &self->gauge;
    GaugeLimits *limits = D_08B4AF14;
    int value = func_08AE0ADC(self);
    gauge->ratio = (float)value / limits->max;
    gauge->field_04 = 0.0f;
    gauge->field_08 = 0.0f;
    func_08ADFC84(self->panel_2A8);
}
