// Calls virtual slot 1 of the object held at +0x196C when it exists.
struct Anim08B533E4 {
    char data[8];
    virtual void v0();
    virtual void v1();
};
struct Owner08B533E4 {
    char pad[0x196C];
    Anim08B533E4 *anim;
};
extern "C" void func_08B533E4(Owner08B533E4 *self)
{
    if (self->anim != 0) {
        self->anim->v1();
    }
}
