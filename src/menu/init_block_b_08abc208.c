/* Initialises the global block at 0x09B6D9D8 with flags and values, then calls the setup routine on param + 0x100. */
extern struct Blk { int a, b; char c0[4]; unsigned char c, d; char c1[6]; int e, f; } D_09B6D9D8;
extern void func_088650A4(char *p);
void func_08ABC208(char *p, int b, int c, int e, int f) {
    D_09B6D9D8.b = b;
    D_09B6D9D8.a = c;
    D_09B6D9D8.c = 1;
    D_09B6D9D8.d = 0;
    D_09B6D9D8.e = e;
    D_09B6D9D8.f = f;
    func_088650A4(p + 0x100);
}
