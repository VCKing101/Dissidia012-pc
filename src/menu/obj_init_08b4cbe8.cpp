// Runs the object's first virtual call, stores two values, creates a handle and applies its layout results.
extern "C" {
void *func_088DAA40(int a, int b, int c);
void func_088DA0A8(int a, int b, void *out1, void *out2);
void func_088D9B84(void *h, void *a);
void func_088D9AF4(void *h, void *a);
}

struct V4CBE8 {
    char data[0xC];
    virtual void v0();
    virtual void v1();
};

#define F(p, off, T) (*(T *)((char *)(p) + (off)))

extern "C" void func_08B4CBE8(V4CBE8 *self, int a, int b, int c)
{
    char big[64];
    char small[4];
    self->v1();
    F(self, 4, int) = b;
    F(self, 8, int) = c;
    F(self, 0x10, void *) = func_088DAA40(a, 6, 0);
    func_088DA0A8(F(self, 4, int), F(self, 8, int), big, small);
    func_088D9B84(F(self, 0x10, void *), big);
    func_088D9AF4(F(self, 0x10, void *), small);
}
