/* Teardown: base cleanup, one member release, release of the handle at +0x2C, then the first virtual function. */
struct Obj {
    char data[0x24];
    virtual void f0();
    int *at(int off) { return (int *)((char *)this + off); }
};
extern "C" void func_0892ACAC(Obj *self);
extern "C" void func_08B52A24(Obj *self);
extern "C" void func_088DAB44(int h);
extern "C" void func_08B526D0(Obj *self)
{
    func_0892ACAC(self);
    func_08B52A24(self);
    if (*self->at(0x2C) != 0) {
        func_088DAB44(*self->at(0x2C));
        *self->at(0x2C) = 0;
    }
    self->f0();
}
