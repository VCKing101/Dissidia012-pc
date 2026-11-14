/* Applies mode 2, or mode 1 when func_08A8E01C returns 1.
   Retail keeps `mode` in the frame (0($sp), 8-byte local area) and stores it
   at each assignment while the arguments sit in s0/s1. A local array is never
   promoted by SNC -O2; two elements give the 8-byte area. */
void func_08A8CE88(void *self, unsigned char value, int mode);
int func_08A8E01C(void *self);

void func_08A8E0C0(void *self, unsigned char value)
{
    int mode[2];
    mode[0] = 2;
    if (func_08A8E01C(self) == 1) {
        mode[0] = 1;
    }
    func_08A8CE88(self, value, mode[0]);
}
