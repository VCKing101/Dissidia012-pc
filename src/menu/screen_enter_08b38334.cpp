/* Marks the screen active, runs the setup calls, clears a global flag and registers its handlers. */
extern "C" {
void func_08B35DC4(void);
void func_08B35EE0(void);
void *func_08B36490(void *self);
void func_089127C4(void *a);
void func_08B3CAF0(int a, void *fn);
void func_08B3D5FC(void);
void func_08B3D618(void);
extern char *D_089D712C;
}

struct S38334 {
    char pad[0x44];
    unsigned flags;
    char pad2[0x4C];
    int f94;
    bool Has(unsigned m) { return (flags & m) != 0; }
};

extern "C" void func_08B38334(S38334 *self)
{
    self->flags |= 0x10000;
    func_08B35DC4();
    func_08B35EE0();
    func_089127C4(func_08B36490(self));
    D_089D712C[0xEFC] = 0;
    self->f94 = 9;
    func_08B3CAF0(2, (void *)func_08B3D5FC);
    if (self->Has(0x8000)) {
        func_08B3CAF0(2, (void *)func_08B3D618);
    }
}
