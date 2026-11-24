// Releases the object at +0x40 via virtual slot 30 and the handle at +0x44 via func_088A6C70, clearing both pointers.
struct Anim08B42540 {
    char data[1348];
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
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
};
struct Owner08B42540 {
    char pad[0x40];
    Anim08B42540 *anim;
};
extern "C" void func_088A6C70(void *p);
struct Owner2_08B42540 {
    char pad[0x40];
    Anim08B42540 *anim;
    void *handle;
};
extern "C" void func_08B42540(Owner2_08B42540 *self)
{
    if (self->anim != 0) {
        self->anim->v30();
        self->anim = 0;
    }
    if (self->handle != 0) {
        func_088A6C70(self->handle);
        self->handle = 0;
    }
}
