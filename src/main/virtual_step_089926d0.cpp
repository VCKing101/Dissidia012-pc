/* Calls virtual 5 of the object itself (vptr at +0x50) with the counter at +0x1C plus one. */
struct Stepper {
    char data0[0x1C];
    int count;
    char data1[0x30];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
    virtual void target(int n);
};
extern "C" void func_089926D0(Stepper *self)
{
    self->target(self->count + 1);
}
