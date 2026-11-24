// Runs func_08B61C5C, then calls virtual slot 15 with a value and a fixed effect name.
struct Anim08B60FBC {
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
    virtual void v15(int a, char *name);
};
extern "C" void func_08B61C5C(void *self);
extern char D_08B610D0[];
extern "C" void func_08B60FBC(Anim08B60FBC *self0, int a0)
{
    int a = a0;
    Anim08B60FBC *self = self0;
    func_08B61C5C(self);
    self->v15(a, D_08B610D0);
}
