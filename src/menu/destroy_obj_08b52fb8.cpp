/* Teardown: base cleanup, two member releases, release of the handle at +0x2C, then the first virtual function. */
struct Obj {
    char data[0x24];
    virtual void f0();
    int *at(int off) { return (int *)((char *)this + off); }
};
extern "C" void func_0892ACAC(Obj *self);
extern "C" void func_08B53494(Obj *self);
extern "C" void func_08B5331C(Obj *self);
extern "C" void func_088DAB44(int h);
extern "C" void func_08B52FB8(Obj *self)
{
    func_0892ACAC(self);
    func_08B53494(self);
    func_08B5331C(self);
    if (*self->at(0x2C) != 0) {
        func_088DAB44(*self->at(0x2C));
        *self->at(0x2C) = 0;
    }
    self->f0();
}
