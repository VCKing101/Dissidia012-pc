/* Runs three teardown steps on the object, then calls its virtual 0 (vptr at +0x24). */
struct Thing {
    char data[0x24];
    virtual void target();
};
extern "C" void func_0892ACAC(Thing *self);
extern "C" void func_0892F6DC(Thing *self);
extern "C" void func_0892FC10(Thing *self);
extern "C" void func_0892F5AC(Thing *self)
{
    func_0892ACAC(self);
    func_0892F6DC(self);
    func_0892FC10(self);
    self->target();
}
