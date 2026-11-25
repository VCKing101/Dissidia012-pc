struct O301C {
    int *a;
    int b;
    int pad8;
    int *c;
    int pad10;
    int count;
    int pad18, pad1c;
    void (*fn)(int);
    char pad24[0x30 - 0x24];
    char sub30[0x640 - 0x30];
    char sub640[0xC50 - 0x640];
    int r50;
    int r54;
    int r58;
    int r5c;
};
extern "C" void func_08993754(int, int, int, void *, void *, int);
extern "C" void func_089C2D60(void);
extern "C" void func_089C26A4(void);
extern "C" void func_08973FF0(void *);
extern "C" void func_08974940(int);
extern "C" void func_08942544(void *);
extern "C" void func_0897301C(struct O301C *self)
{
    for (int i = 0, off = 0; i < self->count; i++, off += 4) {
        if (*(int *)((char *)self->a + off) != 0) {
            *(int *)((char *)self->a + off) = 0;
        }
        if (*(int *)((char *)self->c + off) != 0) {
            self->fn(*(int *)((char *)self->c + off));
            *(int *)((char *)self->c + off) = 0;
        }
    }
    if (self->b != 0) {
        func_08993754(self->b, -1, 8, func_089C2D60, func_089C26A4, 0);
        self->b = 0;
    }
    if (self->r50 != 0) {
        func_08973FF0(self->sub30);
        func_08974940(self->r50);
        self->r50 = 0;
        self->r58 = 0;
    }
    if (self->r54 != 0) {
        func_08973FF0(self->sub640);
        func_08974940(self->r54);
        self->r54 = 0;
        self->r5c = 0;
    }
    func_08942544(self);
}
