/* Object whose vtable pointer sits after 0x130 bytes of data. */
class ResetScreen {
public:
    char data[0x130];
    virtual void v0();
    virtual void v1();
    virtual void v2();
};

extern "C" void func_08A55938(ResetScreen *self);
extern "C" void func_08A55878(ResetScreen *self);

extern "C" {
void func_08A55114(ResetScreen *self)
{
    self->v2();
    self->v1();
    func_08A55938(self);
    func_08A55878(self);
}
}
