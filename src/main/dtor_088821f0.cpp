extern "C" void func_08992F04(void *p);

struct Object088821F0 {
    int field_00;

    static void destroy(Object088821F0 *self, int flags) {
        if (self != 0) {
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" {
void func_088821F0(Object088821F0 *self, int flags) {
    Object088821F0::destroy(self, flags);
}
}
