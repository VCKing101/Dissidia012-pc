struct O3138 {
    int *a;
    int b;
    int pad8, padc, pad10;
    int count;
    char pad18[0x30 - 0x18];
    char sub30[0x640 - 0x30];
    char sub640[0xC50 - 0x640];
    int r50, r54, r58, r5c;
};
extern "C" int func_0899350C(int, int, void *, int, void *, int, int);
extern "C" void func_089C2D48(void);
extern "C" void func_089C2688(void);
extern "C" void func_089424A4(void *, int);
extern "C" int func_089748F4(int);
extern "C" void func_08973F94(void *, int, int);
extern "C" int func_08973138(O3138 *self)
{
    self->b = func_0899350C(200, 8, (void *)func_089C2D48, 0, (void *)func_089C2688, 0, 0);
    func_089424A4(self, 200);
    {
        int i = 0;
        int off = 0;
        int off4;
        if (i < self->count) {
            off4 = 0;
            do {
                *(int *)((char *)self->a + off4) = self->b + off;
                i++;
                off += 8;
                off4 += 4;
            } while (i < self->count);
        }
    }
    if (self->r58 != 0) {
        int v = self->r58;
        if (v < 0x8000) {
            self->r58 = 0x8000;
            v = 0x8000;
        }
        int h = func_089748F4(v);
        self->r50 = h;
        func_08973F94(self->sub30, h, self->r58);
    }
    if (self->r5c != 0) {
        if (self->r5c < 0x400) {
            self->r5c = 0x400;
        }
        int h = func_089748F4(self->r5c);
        self->r54 = h;
        func_08973F94(self->sub640, h, self->r5c);
    }
    return 1;
}
