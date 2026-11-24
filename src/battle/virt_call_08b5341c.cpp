// Calls virtual slot 5 of the object held at +0x196C when it exists.
struct Anim08B5341C {
    char data[8];
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
};
struct Owner08B5341C {
    char pad[0x196C];
    Anim08B5341C *anim;
};
extern "C" void func_08B5341C(Owner08B5341C *self)
{
    if (self->anim != 0) {
        self->anim->v5();
    }
}
