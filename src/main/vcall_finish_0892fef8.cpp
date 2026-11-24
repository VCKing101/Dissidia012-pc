/* Runs the teardown steps, releases the handle at +0x2C, then calls the object's virtual 0 (vptr at +0x24). */
struct Thing {
    char data[0x24];
    virtual void target();
};
extern "C" void func_0892ACAC(Thing *self);
extern "C" void func_089302A4(Thing *self);
extern "C" void func_089301D0(Thing *self);
extern "C" void func_088DAB44(int h);
extern "C" void func_0892FEF8(Thing *self)
{
    func_0892ACAC(self);
    func_089302A4(self);
    func_089301D0(self);
    if (*(int *)((char *)self + 0x2C) != 0) {
        func_088DAB44(*(int *)((char *)self + 0x2C));
        *(int *)((char *)self + 0x2C) = 0;
    }
    self->target();
}
