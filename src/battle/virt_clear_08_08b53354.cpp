// Calls virtual slot 3 of the object held at +0x196C when it exists, then clears the pointer and the byte at +0.
struct Anim08B53354 {
    char data[8];
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
};
struct Owner08B53354 {
    char pad[0x196c];
    Anim08B53354 *anim;
};
struct Owner2_08B53354 {
    unsigned char flag;
    char pad[0x196B];
    Anim08B53354 *anim;
};
extern "C" void func_08B53354(Owner2_08B53354 *self)
{
    if (self->anim != 0) {
        self->anim->v3();
    }
    self->anim = 0;
    self->flag = 0;
}
