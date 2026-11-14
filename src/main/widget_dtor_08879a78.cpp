extern "C" {
void func_0899372C(void *arr, int count, int size, void (*dtor)(void *), int a, int b);
void func_088D31D0(void *p);
void func_088D2E7C(void *p);
void func_0887B1F4(void *p, int flags);
void func_08992F04(void *p);
}
struct HandlePair {
    int a[7];
    int b[3];
    static void destroy(HandlePair *p)
    {
        if (p != 0) {
            func_0899372C(p->b, 3, 4, func_088D31D0, 0, 0);
            func_0899372C(p->a, 7, 4, func_088D2E7C, 0, 0);
        }
    }
};
struct Sub148 {
    char pad_00[0x34];
    char field_34[8];
    static void destroy(Sub148 *p)
    {
        if (p != 0) {
            func_0887B1F4(p->field_34, 2);
        }
    }
};
struct Widget {
    char pad_00[0x148];
    Sub148 sub;
    HandlePair handles;
    static void destroy(Widget *self, unsigned int flags)
    {
        if (self != 0) {
            HandlePair::destroy(&self->handles);
            Sub148::destroy(&self->sub);
            if ((flags & 1) != 0) {
                func_08992F04(self);
            }
        }
    }
};
extern "C" void func_08879A78(Widget *self, unsigned int flags)
{
    Widget::destroy(self, flags);
}
