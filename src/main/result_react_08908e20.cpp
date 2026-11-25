/* Reacts to the pending result code: 2 starts func_08908CD0, 1/3/4 call a virtual on the child (0, 1 or 3 with 0) and then func_08908FDC with a mode. */
struct Child08908E20 { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(int a); };
struct Obj08908E20 {
    char pad0[0x20]; Child08908E20 *child;
};
extern "C" {
int func_089034D0(Obj08908E20 *o);
void func_089034B4(Obj08908E20 *o);
void func_08908CD0(Obj08908E20 *o, void *p);
void func_08908FDC(Obj08908E20 *o, int m);
void func_08908E20(Obj08908E20 *self)
{
    int r = func_089034D0(self);
    if (r != 0) {
        func_089034B4(self);
        switch (r) {
        case 2:
            func_08908CD0(self, (char *)self + 0x240);
            break;
        case 1:
            if (self->child != 0) {
                self->child->v0();
            }
            func_08908FDC(self, 2);
            break;
        case 3:
            if (self->child != 0) {
                self->child->v1();
            }
            func_08908FDC(self, 0);
            break;
        case 4:
            if (self->child != 0) {
                self->child->v3(0);
            }
            func_08908FDC(self, 0);
            break;
        }
    }
}
}
