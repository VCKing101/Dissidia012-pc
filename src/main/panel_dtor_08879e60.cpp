extern "C" {
void func_08869908(void *p, int flags);
void func_088A6528(void *p, int flags);
void func_08992F04(void *p);
}
struct PanelBase {
    char data[0x5C];
    static void destroy(PanelBase *p) { if (p != 0) { func_088A6528(p, 2); } }
};
struct Panel {
    PanelBase base;
    char field_5C[4];
    static void destroy(Panel *self, unsigned int flags)
    {
        if (self != 0) {
            func_08869908(self->field_5C, 2);
            PanelBase::destroy(&self->base);
            if ((flags & 1) != 0) {
                func_08992F04(self);
            }
        }
    }
};
extern "C" void func_08879E60(Panel *self, unsigned int flags)
{
    Panel::destroy(self, flags);
}
