// Runs the base step, then pushes or pops channel 5 depending on the flag at +0x39, then the common update.
struct S { char pad[0x39]; unsigned char flag; };
extern void func_08940CCC();
extern int func_0897034C(int);
extern void func_089708F0(int);
extern void func_08970794(int);
extern void func_08941128(struct S *);
void func_08940A00(struct S *self) {
    int r;
    func_08940CCC();
    if (self->flag != 0) {
        r = func_0897034C(5);
        if (r != 0) {
            func_08970794(5);
        }
    } else {
        r = func_0897034C(5);
        if (r != 0) {
            func_089708F0(5);
        }
    }
    func_08941128(self);
}
