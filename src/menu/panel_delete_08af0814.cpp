extern "C" void func_08AF0878(void *self);

struct DeletePanel {
    int field_00;

    static void destroy(DeletePanel *self, int flags)
    {
        if (self != 0) {
            if ((flags & 1) != 0) {
                func_08AF0878(self);
            }
        }
    }
};

extern "C" {
void func_08AF0814(DeletePanel *self, int flags)
{
    DeletePanel::destroy(self, flags);
}
}
