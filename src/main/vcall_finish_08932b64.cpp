/* Runs the teardown steps on the object and its two sub-blocks, then calls its virtual 0 (vptr at +0x24). */
struct Thing {
    char data[0x24];
    virtual void target();
};
extern "C" void func_0892ACAC(Thing *self);
extern "C" void func_08931DB0(char *p);
extern "C" void func_08933034(Thing *self);
extern "C" void func_08932F64(Thing *self);
extern "C" void func_08932B64(Thing *self)
{
    func_0892ACAC(self);
    func_08931DB0((char *)self + 0x38);
    func_08931DB0((char *)self + 0x5C);
    func_08933034(self);
    func_08932F64(self);
    self->target();
}
