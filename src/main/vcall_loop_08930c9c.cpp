/* Tears down the object's 6 sub-blocks, releases the handle at +0x30, then calls its virtual 0 (vptr at +0x24). */
struct Thing {
    char data[0x24];
    virtual void target();
};
extern "C" void func_0892ACAC(Thing *self);
extern "C" void func_089303B0(char *p);
extern "C" void func_088DAB44(int h);
extern "C" void func_08930C9C(Thing *self)
{
    int i;
    char *p;
    { Thing *t = self; func_0892ACAC(t); }
    i = 0;
    p = (char *)self + 0x38;
    do {
        func_089303B0(p);
        i++;
        p += 0x5c;
    } while (i < 6);
    if (*(int *)((char *)self + 0x30) != 0) {
        func_088DAB44(*(int *)((char *)self + 0x30));
        *(int *)((char *)self + 0x30) = 0;
    }
    self->target();
}
