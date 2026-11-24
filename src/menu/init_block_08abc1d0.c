/* Initialises the global block at 0x09B6D9D8 and calls the setup routine on param + 0x100. */
extern struct Blk { int a, b; char c0[4]; char c; char c1[7]; int d, e; } D_09B6D9D8;
extern void func_088650A4(char *p);
void func_08ABC1D0(char *p, int b, int c) {
    D_09B6D9D8.b = b;
    D_09B6D9D8.a = c;
    D_09B6D9D8.c = 0;
    D_09B6D9D8.d = 0;
    D_09B6D9D8.e = 0;
    func_088650A4(p + 0x100);
}
