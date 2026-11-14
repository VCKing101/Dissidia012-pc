struct Screen1F108 {
    char pad_00[0x138];
    int handle;
    int effect;
};

extern "C" {
extern int D_089D7260;
extern void func_08A1F268(Screen1F108 *self);
extern int func_0885C6CC(int handle, int id, int a, int b, int c, int d);
extern void func_08B58080(int effect);
extern void func_088AF020(int o, int handle);
}

extern "C" void func_08A1F108(Screen1F108 *self)
{
    func_08A1F268(self);
    int id = 0x3D38;
    int one = 1;
    if (self->effect != 0) {
        return;
    }
    self->effect = func_0885C6CC(self->handle, id, 0, 0, one, 0);
    func_08B58080(self->effect);
    func_088AF020(D_089D7260, self->handle);
}
