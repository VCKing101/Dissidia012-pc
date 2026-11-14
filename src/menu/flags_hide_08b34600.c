/* Sets the hide flags, fades the object to 0, then swaps in the hidden
   flag set. */
struct Object08B34600 {
    char pad_000[0x388];
    unsigned long long field_388;
};

extern unsigned long long D_08B34BC8;
extern unsigned long long D_08B34BD8;
extern unsigned long long D_08B34BE8;
extern void func_08834DF4(struct Object08B34600 *self, float alpha);

void func_08B34600(struct Object08B34600 *self)
{
    self->field_388 |= D_08B34BC8;
    func_08834DF4(self, 0.0f);
    self->field_388 = (self->field_388 | D_08B34BD8) & D_08B34BE8;
}
