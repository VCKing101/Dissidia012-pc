// Reads a float from virtual slot 20 and, when positive, passes it to virtual slots 18 and 19.
struct Obj08B61D3C {
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
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18(float f);
    virtual void v19(float f);
    virtual float v20();
};
extern "C" void func_08B61D3C(Obj08B61D3C *self)
{
    float f = self->v20();
    if (f > 0.0f) {
        self->v18(f);
        self->v19(f);
    }
}
