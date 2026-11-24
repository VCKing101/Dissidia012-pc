// Runs the object's third virtual call, creates its three handles from the given ids and registers them.
extern "C" {
void *func_0887B3EC(int a, int b, int c, int d, int e, int f);
void func_088D428C(void *a, void *b, const char *c, int d);
void func_0887B944(void *h);
extern const char D_08B4EB10[];
}

struct V4CD3C {
    char data[0xC];
    virtual void v0();
    virtual void v1();
    virtual void v2();
};

#define F(p, off, T) (*(T *)((char *)(p) + (off)))

extern "C" void func_08B4CD3C(V4CD3C *self, int heap, int a, int b, int c, int d, int e, int f, unsigned char hide)
{
    self->v2();
    F(self, 0x14, void *) = func_0887B3EC(heap, a, 0, b, 0, 0);
    F(self, 0x18, void *) = func_0887B3EC(heap, c, 0, d, 0, 0);
    F(self, 0x1C, void *) = func_0887B3EC(heap, e, 0, f, 0, 0);
    func_088D428C((char *)self + 0x10, (char *)self + 0x14, D_08B4EB10, 1);
    if (hide == 0) {
        func_0887B944(F(self, 0x1C, void *));
    }
}
