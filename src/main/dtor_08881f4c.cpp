extern "C" void func_08992F04(void *p);

struct Object08881F4C {
    int field_00;

    static void destroy(Object08881F4C *self, int flags) {
        if (self != 0) {
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" {
void func_08881F4C(Object08881F4C *self, int flags) {
    Object08881F4C::destroy(self, flags);
}
}
