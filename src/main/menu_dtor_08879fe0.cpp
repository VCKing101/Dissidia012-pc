extern "C" {
void func_0887A644(void *p, int flags);
void func_0899372C(void *arr, int count, int size, void (*dtor)(void *, int), int a, int b);
void func_08890B04(void *p, int flags);
void func_088CE36C(void *p, int flags);
void func_088D6CFC(void *p, int flags);
void func_088DE2C4(void *p, int flags);
void func_088CC88C(void *p, int flags);
void func_088CD424(void *p, int flags);
void func_08992F04(void *p);
}
struct Base7A8 {
    char data[4];
    static void destroy(Base7A8 *p) { if (p != 0) { func_0887A644(p, 2); } }
};
struct Menu {
    char pad_00[0x6C];
    char field_6C[0x44];
    char field_B0[0x38];
    char field_E8[2][0xA4];
    char field_230[2][0x27C];
    char field_728[0x2C];
    char field_754[2][0x28];
    char pad_7A4[4];
    Base7A8 base;
    static void destroy(Menu *self, unsigned int flags)
    {
        if (self != 0) {
            Base7A8::destroy(&self->base);
            func_0899372C(self->field_754, 2, 0x28, func_08890B04, 0, 0);
            func_088CE36C(self->field_728, 2);
            func_0899372C(self->field_230, 2, 0x27C, func_088D6CFC, 0, 0);
            func_0899372C(self->field_E8, 2, 0xA4, func_088DE2C4, 0, 0);
            func_088CC88C(self->field_B0, 2);
            func_088CD424(self->field_6C, 2);
            if ((flags & 1) != 0) {
                func_08992F04(self);
            }
        }
    }
};
extern "C" void func_08879FE0(Menu *self, unsigned int flags)
{
    Menu::destroy(self, flags);
}
