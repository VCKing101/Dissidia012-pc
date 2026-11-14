extern "C" {
void func_088734D4(void *p, int flags);
void func_0885E834(void *p, int flags);
void func_0880FBA0(void *p, int flags);
void func_08992F04(void *p);
}
struct Part {
    char pad_00[0xC0];
    char field_C0[0x1C0];
    static void destroy(Part *p) { if (p != 0) { func_088734D4(p->field_C0, 2); func_0885E834(p, 2); } }
};
struct Stage {
    Part main;
    char pad_280[0x4D0];
    char field_750[0x180];
    Part extra;
    static void destroy(Stage *self, unsigned int flags)
    {
        if (self != 0) {
            Part::destroy(&self->extra);
            func_0880FBA0(self->field_750, 2);
            func_088734D4(self->main.field_C0, 2);
            func_0885E834(self, 2);
            if ((flags & 1) != 0) {
                func_08992F04(self);
            }
        }
    }
};
extern "C" void func_08879C20(Stage *self, unsigned int flags)
{
    Stage::destroy(self, flags);
}
