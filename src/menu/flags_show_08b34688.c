/* Clears the hide flags, fades the object to 1, then swaps in the shown
   flag set. */
struct Object08B34688 {
    char pad_000[0x388];
    unsigned long long field_388;
};

extern unsigned long long D_08B34BD0;
extern unsigned long long D_08B34BE0;
extern unsigned long long D_08B34BF0;
extern void func_08834DF4(struct Object08B34688 *self, float alpha);

void func_08B34688(struct Object08B34688 *self)
{
    self->field_388 &= D_08B34BD0;
    func_08834DF4(self, 1.0f);
    self->field_388 = (self->field_388 & D_08B34BE0) | D_08B34BF0;
}
