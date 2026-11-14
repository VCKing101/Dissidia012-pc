extern "C" void func_08885B5C(void *p);

struct Object08885B14 {
    int field_00;

    static void destroy(Object08885B14 *self, int flags) {
        if (self != 0) {
            if (flags & 1) {
                func_08885B5C(self);
            }
        }
    }
};

extern "C" {
void func_08885B14(Object08885B14 *self, int flags) {
    Object08885B14::destroy(self, flags);
}
}
