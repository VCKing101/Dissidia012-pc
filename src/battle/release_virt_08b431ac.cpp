// Calls virtual slot 30 of the object held at +0x1C, then clears the pointer.
struct Anim08B431AC {
    char data[0x544];
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
struct Owner08B431AC {
    char pad[0x1c];
    Anim08B431AC *anim;
};
extern "C" void func_08B431AC(Owner08B431AC *self)
{
    self->anim->v30();
    self->anim = 0;
}
