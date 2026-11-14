extern "C" int D_08AF12E0;
extern "C" void func_08AF12B0(void *self);

struct DeletePanel {
    void *field_00;

    static void destroy(DeletePanel *self, int flags, void *vtable)
    {
        if (self != 0) {
            self->field_00 = vtable;
            if ((flags & 1) != 0) {
                func_08AF12B0(self);
            }
        }
    }
};

extern "C" {
void func_08AF1244(DeletePanel *self, int flags)
{
    DeletePanel::destroy(self, flags, &D_08AF12E0);
}
}
