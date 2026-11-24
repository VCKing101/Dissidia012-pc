/* Calls func_08B4FFAC(self) when the first word of self is 1. */
extern void func_08B4FFAC(int *self);
void func_08B4FF80(int *self)
{
    if (*self == 1) { func_08B4FFAC(self); }
}
