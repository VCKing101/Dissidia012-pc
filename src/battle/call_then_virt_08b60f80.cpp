// Runs func_08B61C08 on the object, then calls virtual slot 14.
extern "C" void func_08B61C08(void *self);
struct Obj08B60F80 {
    char data[8];
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
};
extern "C" void func_08B60F80(Obj08B60F80 *self)
{
    func_08B61C08(self);
    self->v14();
}
