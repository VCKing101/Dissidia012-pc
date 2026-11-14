struct Screen08A1F178 {
    char pad_00[0x138];
    int handle;
    int effect_13C[3];
};

template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

extern "C" {
extern int D_089D7260;
extern void func_088AF044(int o);
extern void func_088AEF98(int o);
extern void func_0885C538(int handle, int effect, int flag);
}

extern "C" void func_08A1F178(Screen08A1F178 *self)
{
    if (self->effect_13C[0] == 0) {
        return;
    }
    Kept<Screen08A1F178 *> saved(self);
    func_088AF044(D_089D7260);
    func_088AEF98(D_089D7260);
    self = saved.value;
    func_0885C538(self->handle, self->effect_13C[0], 0);
    self = saved.value;
    self->effect_13C[0] = 0;
}
