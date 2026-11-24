// Builds two stack records from a source, hands them to the helper and raises the caller's short to the record's value if larger.
struct RecA { int w[0x1DE]; };
struct RecB { int w[12]; };
struct Blk { char pad[0xC8]; char x[1]; };
extern Blk D_09B6DAC0;
extern "C" {
extern void func_088CB9E8(RecA *);
extern void func_088C5E44(RecB *);
extern void func_08931A1C(int, RecA *, RecB *, char *);
extern void func_08B3ADF4(int, short *, RecA *, RecB *, int);
void func_08B3ABE0(int a, short *out, int src, int extra) {
    RecA ra;
    RecB rb;
    func_088CB9E8(&ra);
    func_088C5E44(&rb);
    func_08931A1C(src, &ra, &rb, D_09B6DAC0.x);
    func_08B3ADF4(a, out, &ra, &rb, extra);
    if (*out < ra.w[3]) {
        *out = ra.w[3];
    }
}
}
