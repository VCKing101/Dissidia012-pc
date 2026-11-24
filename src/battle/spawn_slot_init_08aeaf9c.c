/* Allocates the effect buffer and primes two globals. */
extern void *D_089D7830;
extern void *D_08AEB9BC;
extern int D_09B6C278;
extern unsigned char D_08AEB9C0;
extern void *func_088D09D8(void *, int, int);

void func_08AEAF9C(void)
{
    D_08AEB9BC = func_088D09D8(D_089D7830, 1, 0x14);
    D_09B6C278 = 0x165;
    D_08AEB9C0 = 1;
}
