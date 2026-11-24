// Calls func_08B0D5B4(a, the signed byte at D_09AFB8A0+0x25B1, b).
extern signed char D_09AFB8A0[];
extern void func_08B0D5B4(void *, int, int);
void func_08B0D58C(void *a, int b)
{
    func_08B0D5B4(a, D_09AFB8A0[0x25B1], b);
}
