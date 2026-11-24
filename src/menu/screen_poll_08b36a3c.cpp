/* If the global object is idle: runs the update, then either the normal step or the flag-0x10000 exit path. */
extern "C" {
int func_0887E718(void *g);
void func_08B3B464(void *self);
void func_08B35680(void *self);
void func_088FE9CC(void);
void func_08B35CF0(void *self, int a);
void func_088AC8C8(void *g, int a);
extern void *D_089D6998;
extern void *D_089D712C;
}

struct S36A3C {
    char pad[0x44];
    unsigned flags;
    char pad48[0x7C];
    int *state;
    bool Has(unsigned m) { return (flags & m) != 0; }
};

extern "C" void func_08B36A3C(S36A3C *self)
{
    if (func_0887E718(D_089D6998) != 0) {
        return;
    }
    func_08B3B464(self);
    if (self->Has(0x10000)) {
        func_088FE9CC();
        self->flags &= 0xFFFEFFFF;
        func_08B35CF0(self, 0);
        func_088AC8C8(D_089D712C, 1);
        *self->state = 0x1F;
    } else {
        func_08B35680(self);
    }
}
