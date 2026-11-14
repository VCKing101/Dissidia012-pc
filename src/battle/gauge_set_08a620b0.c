typedef struct GaugeSet {
    char pad_00[0x4];
    float values[10];
} GaugeSet;

extern int func_08A6217C(GaugeSet *self);
extern void func_08A621E8(GaugeSet *self);

void func_08A620B0(GaugeSet *self, unsigned int index)
{
    if (func_08A6217C(self) == 0) {
        return;
    }
    if (index >= 10) {
        return;
    }
    self->values[index] = 2.0f;
    func_08A621E8(self);
}
