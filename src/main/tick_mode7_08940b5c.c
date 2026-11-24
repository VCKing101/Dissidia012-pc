// Runs the base step, then pushes or pops channel 7 depending on the flag at +0x39, then the common update.
struct S { char pad[0x39]; unsigned char flag; };
extern void func_08940CCC();
extern int func_0897034C(int);
extern void func_089708F0(int);
extern void func_08970794(int);
extern void func_089A03DC(int);
extern void func_08941128(struct S *);
void func_08940B5C(struct S *self) {
    int r;
    func_08940CCC();
    func_089A03DC(1);
    if (self->flag != 0) {
        r = func_0897034C(7);
        if (r != 0) {
            func_08970794(7);
        }
    } else {
        r = func_0897034C(7);
        if (r != 0) {
            func_089708F0(7);
        }
    }
    func_08941128(self);
}
