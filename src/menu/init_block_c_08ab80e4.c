/* Initialises the global block at 0x09B6D818 and calls the setup routine on param + 0x100. */
extern struct Blk { int a, b; unsigned char c, d; char c1[2]; int e, f; } D_09B6D818;
extern void func_088650A4(char *p);
void func_08AB80E4(char *p, int b, int a, unsigned char c, unsigned char d, int e, int f) {
    D_09B6D818.b = b;
    D_09B6D818.a = a;
    D_09B6D818.c = c;
    D_09B6D818.d = d;
    D_09B6D818.e = e;
    D_09B6D818.f = f;
    func_088650A4(p + 0x100);
}
