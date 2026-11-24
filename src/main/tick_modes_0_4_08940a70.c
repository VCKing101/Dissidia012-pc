// Base step, then pushes or pops channels 0..3 by the flag at +0x39, channel 4 likewise after a sync call, then the common update.
struct S { char pad[0x39]; unsigned char flag; };
extern void func_08940CCC();
extern int func_0897034C(int);
extern void func_089708F0(int);
extern void func_08970794(int);
extern void func_089A03DC(int);
extern void func_08941128(struct S *);
void func_08940A70(struct S *self) {
    int r;
    func_08940CCC();
    if (self->flag != 0) {
        func_08970794(0);
        func_08970794(1);
        func_08970794(2);
        r = func_0897034C(3);
        if (r != 0) {
            func_08970794(3);
        }
    } else {
        func_089708F0(0);
        func_089708F0(1);
        func_089708F0(2);
        r = func_0897034C(3);
        if (r != 0) {
            func_089708F0(3);
        }
    }
    func_089A03DC(1);
    if (self->flag != 0) {
        r = func_0897034C(4);
        if (r != 0) {
            func_08970794(4);
        }
    } else {
        r = func_0897034C(4);
        if (r != 0) {
            func_089708F0(4);
        }
    }
    func_08941128(self);
}
