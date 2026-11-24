// Releases the 4 + 2 slot entries, runs the cleanup, drops the handle and calls the object's first virtual function.
extern "C" {
void func_0892ACAC(void *self);
void func_08B4F030(char *slot);
void func_08B4F754(void *self);
void func_088DAB44(void);
}

struct V4F314 {
    char data[0x24];
    virtual void v0();
};

extern "C" void func_08B4F314(V4F314 *self)
{
    int i;
    char *p;
    func_0892ACAC(self);
    p = (char *)self;
    i = 0;
    p += 0x3C;
    do {
        func_08B4F030(p);
        i++;
        p += 0x40;
    } while (i < 4);
    i = 0;
    p = (char *)self + 0x13C;
    do {
        func_08B4F030(p);
        i++;
        p += 0x40;
    } while (i < 2);
    func_08B4F754(self);
    if (*(int *)((char *)self + 0x2C) != 0) {
        func_088DAB44();
        *(int *)((char *)self + 0x2C) = 0;
    }
    self->v0();
}
