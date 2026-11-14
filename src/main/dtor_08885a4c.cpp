extern "C" void func_08992F04(void *p);

struct Object08885A4C {
    int field_00;

    static void destroy(Object08885A4C *self, int flags) {
        if (self != 0) {
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" {
void func_08885A4C(Object08885A4C *self, int flags) {
    Object08885A4C::destroy(self, flags);
}
}
