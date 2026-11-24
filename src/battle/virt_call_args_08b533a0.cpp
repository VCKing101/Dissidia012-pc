// Forwards two arguments to virtual slot 0 of the object held at +0x196C when it exists.
struct Anim08B533A0 {
    char data[8];
    virtual void v0(int a, int b);
};
struct Owner08B533A0 {
    char pad[0x196c];
    Anim08B533A0 *anim;
};
extern "C" void func_08B533A0(Owner08B533A0 *self, int a, int b)
{
    if (self->anim != 0) {
        self->anim->v0(a, b);
    }
}
