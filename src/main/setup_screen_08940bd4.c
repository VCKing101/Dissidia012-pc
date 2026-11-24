// Base step, queues the fixed set of resources, optionally starts two sub-effects by flags, then the common update and a notify.
struct S { char pad[0x18]; int a; int b; char pad2[0x15]; unsigned char f35; char pad3[2]; unsigned char f38; };
extern void func_08940CCC();
extern void func_089A03DC(int);
extern void func_089A12A4(int, int, int, int, int);
extern void func_08948CCC(int);
extern void func_08947100(int);
extern void func_08941128(struct S *);
extern void func_08974A00(int);
void func_08940BD4(struct S *self) {
    func_08940CCC();
    func_089A03DC(0x15);
    func_089A12A4(0, 2, 10, 0, -1);
    func_089A03DC(1);
    func_089A03DC(7);
    func_089A03DC(8);
    func_089A03DC(9);
    if (self->f35 != 0) {
        func_08948CCC(self->b);
    }
    if (self->f38 != 0) {
        func_08947100(self->b);
    }
    func_08941128(self);
    func_08974A00(self->a);
}
