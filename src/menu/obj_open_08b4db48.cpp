// Runs the object's third virtual call, opens a handle sized by a flag and registers its widgets.
extern "C" {
void *func_0887B3EC(int a, int b, int c, int d, int e, int f);
void *func_0887E534(void *h);
void func_088D428C(void *a, void *b, const char *c, int d);
void func_08B4DDF8(void *self);
extern const char D_08B4EBB8[];
}

struct V4DB48 {
    char data[0xC];
    virtual void v0();
    virtual void v1();
    virtual void v2();
};

#define F(p, off, T) (*(T *)((char *)(p) + (off)))

extern "C" void func_08B4DB48(V4DB48 *self, int a, int b)
{
    int kind;
    void *h;
    self->v2();
    kind = 0x20;
    if (F(self, 0xA0, unsigned char) != 0) {
        kind = 0x1C;
    }
    h = func_0887B3EC(a, b, 0, kind, 0, 0);
    F(self, 0x68, void *) = h;
    F(self, 0x84, void *) = func_0887E534(h);
    func_088D428C((char *)self + 0x10, (char *)self + 0x40, D_08B4EBB8, 1);
    func_08B4DDF8(self);
}
