struct Work4EDE0 {
    char pad_00[0x6FCC];
    int count;
};

struct Screen4EDE0 {
    char pad_00[0x5C];
    Work4EDE0 *work;
};

extern "C" {
extern int D_089D7F90;
extern int func_088FDC98(int o);
extern void func_08A4C5E0(Screen4EDE0 *self, int mode);
}

extern "C" bool func_08A4EDE0(Screen4EDE0 *self)
{
    self->work->count = 5;
    if (func_088FDC98(D_089D7F90) != 0) {
        func_08A4C5E0(self, 2);
        return true;
    } else {
        func_08A4C5E0(self, 1);
        return false;
    }
}
