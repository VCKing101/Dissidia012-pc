extern "C" {
void func_0892F3D8(void *p, int flags);
void func_088CE36C(void *p, int flags);
void func_088CD424(void *p, int flags);
void func_088DE2C4(void *p, int flags);
void func_088D6CFC(void *p, int flags);
void func_08869908(void *p, int flags);
void func_08992F04(void *p);
}
struct Base7C {
    char data[0x24];
    static void destroy(Base7C *p) { if (p != 0) { func_08869908(p, 2); } }
};
struct Screen {
    char pad_00[0x7C];
    Base7C base;
    char field_A0[0x27C];
    char field_31C[0xA4];
    char field_3C0[0x44];
    char field_404[0x14];
    char field_418[4];
    static void destroy(Screen *self, unsigned int flags)
    {
        if (self != 0) {
            func_0892F3D8(self->field_418, 2);
            func_088CE36C(self->field_404, 2);
            func_088CD424(self->field_3C0, 2);
            func_088DE2C4(self->field_31C, 2);
            func_088D6CFC(self->field_A0, 2);
            Base7C::destroy(&self->base);
            if ((flags & 1) != 0) {
                func_08992F04(self);
            }
        }
    }
};
extern "C" void func_08879B28(Screen *self, unsigned int flags)
{
    Screen::destroy(self, flags);
}
