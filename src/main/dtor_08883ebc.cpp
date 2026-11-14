extern "C" void func_08992F04(void *p);

struct Object08883EBC {
    int field_00;

    static void destroy(Object08883EBC *self, int flags) {
        if (self != 0) {
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};

extern "C" {
void func_08883EBC(Object08883EBC *self, int flags) {
    Object08883EBC::destroy(self, flags);
}
}
