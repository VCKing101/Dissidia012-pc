/* Destructor-like teardown: base cleanup func_0892ACAC, four member cleanups, two release helpers, then the first virtual function of the object. */
struct Obj {
    char data[0x24];
    virtual void f0();
};
extern "C" void func_0892ACAC(Obj *self);
extern "C" void func_08931DB0(char *p);
extern "C" void func_08B4E9A0(Obj *self);
extern "C" void func_08B4E8D0(Obj *self);
extern "C" void func_08B4E0DC(Obj *self)
{
    func_0892ACAC(self);
    func_08931DB0((char *)self + 0x40);
    func_08931DB0((char *)self + 0x64);
    func_08931DB0((char *)self + 0x84);
    func_08931DB0((char *)self + 0xD8);
    func_08B4E9A0(self);
    func_08B4E8D0(self);
    self->f0();
}
