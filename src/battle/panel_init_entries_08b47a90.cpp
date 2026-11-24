// Initialises the menu panel's sub-objects and 14 entries, then calls its virtual refresh.
struct Panel { char data[0x24]; virtual void v0(); };
extern "C" void func_08B492F4(void *);
extern "C" void func_08B471E8(void *);
extern "C" void func_08B47508(void *);
extern "C" void func_08B483F0(void *);
extern "C" void func_08B48300(void *);
extern "C" void func_08B48240(void *);

extern "C" void func_08B47A90(Panel *self)
{
    int i;
    char *e;
    func_08B492F4((char *)self + 0x58);
    func_08B471E8((char *)self + 0xB0);
    for (i = 0, e = (char *)self + 0xBC; i < 14; i++, e += 0x4C) {
        func_08B47508(e);
    }
    func_08B483F0(self);
    func_08B48300(self);
    func_08B48240(self);
    self->v0();
}
