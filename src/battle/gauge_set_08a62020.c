typedef struct GaugeSet {
    char pad_00[0x4];
    float values[10];
} GaugeSet;

extern int func_08A6217C(GaugeSet *self);
extern void func_08A621E8(GaugeSet *self);
extern void func_08874AF8(void);

void func_08A62020(GaugeSet *self, unsigned int index, float value)
{
    if (func_08A6217C(self) == 0) {
        return;
    }
    if (index >= 10) {
        return;
    }
    self->values[index] = value;
    func_08A621E8(self);
    if (index == 9) {
        return;
    }
    func_08874AF8();
}
