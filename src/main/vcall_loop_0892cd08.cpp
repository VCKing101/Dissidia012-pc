/* Tears down the object's 2 sub-blocks, releases the handle at +0x2c, then calls its virtual 0 (vptr at +0x24). */
struct Thing {
    char data[0x24];
    virtual void target();
};
extern "C" void func_0892ACAC(Thing *self);
extern "C" void func_0892C8F0(char *p);
extern "C" void func_088DAB44(int h);
extern "C" void func_0892CFF8(Thing *self);
extern "C" void func_0892CD08(Thing *self)
{
    int i;
    char *p;
    { Thing *t = self; func_0892ACAC(t); }
    i = 0;
    p = (char *)self + 0x38;
    do {
        func_0892C8F0(p);
        i++;
        p += 0x28;
    } while (i < 2);
    func_0892CFF8(self);
    if (*(int *)((char *)self + 0x2c) != 0) {
        func_088DAB44(*(int *)((char *)self + 0x2c));
        *(int *)((char *)self + 0x2c) = 0;
    }
    self->target();
}
