/* Switches to the shown flag set and starts fading towards `value`. */
struct Object08B347D4 {
    char pad_000[0x388];
    unsigned long long field_388;
};

extern unsigned long long D_08B34BE0;
extern unsigned long long D_08B34BF0;
extern unsigned long long D_08B34BD0;
extern void func_08834DF4(struct Object08B347D4 *self, float alpha);
extern void func_08834D60(struct Object08B347D4 *self, float to);

void func_08B347D4(struct Object08B347D4 *self, float value)
{
    self->field_388 = ((self->field_388 & D_08B34BE0) | D_08B34BF0) & D_08B34BD0;
    func_08834DF4(self, 1.0e-7f);
    func_08834D60(self, value);
}
