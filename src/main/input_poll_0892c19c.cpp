/* Polls the selection: if the index changed, swaps the two entries (0x44 bytes each) and flags 1; else asks the input child for key A (flag 3) or key B (flag 2). */
struct Child0892C19C { char d[0x34]; virtual int v0(unsigned long long k); virtual int v1(unsigned long long k); };
struct Obj0892C19C {
    char pad0[0x19C]; Child0892C19C *child; int flag; int h1A4; int sel;
};
extern "C" {
extern unsigned long long D_089CED28;
extern unsigned long long D_089CED30;
void func_088D688C(int *h, int a);
void func_0892B590(void *p);
void func_0892B544(void *p);
void func_0892C19C(Obj0892C19C *self)
{
    int old = self->sel;
    self->flag = 0;
    func_088D688C(&self->h1A4, 0);
    if (old != self->sel) {
        func_0892B590((char *)self + old * 0x44 + 0x44);
        func_0892B544((char *)self + self->sel * 0x44 + 0x44);
        self->flag = 1;
    } else if (self->child->v1(D_089CED28) != 0) {
        self->flag = 3;
    } else if (self->child->v1(D_089CED30) != 0) {
        self->flag = 2;
    }
}
}
