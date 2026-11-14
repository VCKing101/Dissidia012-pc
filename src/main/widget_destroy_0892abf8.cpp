extern "C" {
typedef struct Widget { char pad_00[0x24]; void *vtable; } Widget;
extern char D_089CEC00[];
extern void func_08992F04(void *p);
struct WidgetOps {
    static void destroy(Widget *w, int flags, void *vt) {
        if (w != 0) {
            w->vtable = vt;
            if ((flags & 1) != 0) {
                func_08992F04(w);
            }
        }
    }
};
void func_0892ABF8(Widget *w, int flags)
{
    WidgetOps::destroy(w, flags, D_089CEC00);
}
}
