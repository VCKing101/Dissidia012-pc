/* If *self is nonzero, calls func_088DA060 and clears it. */
extern void func_088DA060(int h);
void func_08B55100(int *self)
{
    if (*self != 0) {
        func_088DA060(*self);
        *self = 0;
    }
}
