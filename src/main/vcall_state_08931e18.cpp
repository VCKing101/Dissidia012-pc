/* When the state word is 1, calls virtuals 3 and 4 of the object itself (vptr at +0xC). */
struct Thing {
    int state;
    char data[8];
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
};
extern "C" void func_08931E18(Thing *self)
{
    while (self->state == 1) {
        self->v3();
        self->v4();
        break;
    }
}
