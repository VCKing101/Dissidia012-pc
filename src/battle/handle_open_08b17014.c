/* Opens the owner's handle (variant picked by the flag byte) and sets state 1 / 0. */
typedef struct OpenSelf { int owner; char pad_04[0x10]; int handle; char pad_18[0x10]; int state; int zero; } OpenSelf;
extern int func_088DAA40(int owner, int variant, int z);
void func_08B17014(OpenSelf *self, unsigned char flag)
{
    int one = 1;
    if (flag == one) {
        int h = func_088DAA40(self->owner, 0, 0);
        self->handle = h;
    } else {
        int h = func_088DAA40(self->owner, 1, 0);
        self->handle = h;
    }
    self->state = one;
    self->zero = 0;
}
