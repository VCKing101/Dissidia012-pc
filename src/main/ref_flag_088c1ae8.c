/* 1 when bit n-1 of the reference's flags is set; 0 for n <= 0. */
typedef struct Ref088C1AE8 Ref088C1AE8;
unsigned int func_088C1998(Ref088C1AE8 *ref);
int func_088C1AE8(Ref088C1AE8 *ref, int n)
{
    if (n > 0) {
        return (func_088C1998(ref) & (1 << (n - 1))) != 0;
    }
    return 0;
}
