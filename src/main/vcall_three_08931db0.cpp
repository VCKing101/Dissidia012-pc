/* Calls virtuals 1, 2 and 0 of the object itself (vptr at +0xC). */
struct Thing {
    char data[0xC];
    virtual void v0(); virtual void v1(); virtual void v2();
};
extern "C" void func_08931DB0(Thing *self)
{
    self->v1();
    self->v2();
    self->v0();
}
