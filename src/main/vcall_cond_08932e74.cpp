/* When the handle at +0x34 is set, tells the helper about it and calls virtual 5 of the object itself (vptr at +0x24). */
struct Thing {
    char data0[0x24];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
    virtual void target();
};
extern "C" void func_088D9B28(int handle, int z);
extern "C" void func_08932E74(Thing *self)
{
    int h = *(int *)((char *)self + 0x34);
    if (h == 0) {
        return;
    }
    func_088D9B28(h, 0);
    self->target();
}
