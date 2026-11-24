// Store a value, run two setup calls and flag the object active.
struct Obj { int active; bool flag; char pad[3]; int param; };
extern "C" void func_0892E9B4(void);
extern "C" void func_0892EA90(Obj *self, int b, int c, int d);
extern "C" void func_0892E77C(Obj *self, int a1, int a2, int a3, int a4)
{
    self->param = a4;
    func_0892E9B4();
    func_0892EA90(self, a2, a3, a4);
    self->active = 1;
    self->flag = true;
}
