/* Base init, copies a five-word source plus one value into the block at 0x09B6DAC0, sets state 6 and calls the loader. */
struct S5 { int a, b, c, d, e; };
struct Blk { char pad[0xA0]; struct S5 s; int v; };
extern struct Blk D_09B6DAC0;
extern void func_08B39F78(void);
extern void func_08B3CAF0(int, void *);
extern void func_08B40FBC(void);
void func_08B37F60(int *self, struct S5 *src, int val) {
    int v = val;
    int *o = self;
    struct S5 *s = src;
    func_08B39F78();
    D_09B6DAC0.s = *s;
    D_09B6DAC0.v = v;
    o[0x94 / 4] = 6;
    func_08B3CAF0(0, func_08B40FBC);
}
