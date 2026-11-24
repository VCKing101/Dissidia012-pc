/* Base init, sets a flag byte in the block at 0x09B6DAC0 if a check on val passes, copies the five-word source plus val, calls the loader. */
struct S5 { int a, b, c, d, e; };
struct Blk { char pad0[0x10]; char flag; char pad1[0x8F]; struct S5 s; int v; };
extern struct Blk D_09B6DAC0;
extern void func_08B39F78(void);
extern int func_0892379C(int);
extern void func_08B3CAF0(int, void *);
extern void func_08B40FBC(void);
void func_08B37E00(int *self, struct S5 *src, int val) {
    func_08B39F78();
    if (func_0892379C(val)) {
        D_09B6DAC0.flag = 1;
    }
    D_09B6DAC0.s = *src;
    D_09B6DAC0.v = val;
    func_08B3CAF0(0, func_08B40FBC);
}
