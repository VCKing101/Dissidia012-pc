/* Rebases the pointer by the offset and calls the stored handler (with the adjusted second value when it takes one), or the default. */
extern void func_08992EC0(int p);
void func_089932A4(int p, int zbase, int unused, void (*fn)(), int takes2, int off)
{
    p = p - off;
    zbase = off + zbase;
    if (fn == 0) {
        func_08992EC0(p);
    } else if (takes2 != 0) {
        fn(p, zbase);
    } else {
        fn(p);
    }
}
