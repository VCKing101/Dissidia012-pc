extern "C" {
void func_088954FC(void *p);
extern char D_089CA890[];
}

struct Object08895448 {
    int field_00;
    int field_04;
    void *vtable;

    static void destroy(Object08895448 *self, int flags, void *vt) {
        if (self != 0) {
            self->vtable = vt;
            if (flags & 1) {
                func_088954FC(self);
            }
        }
    }
};

extern "C" {
void func_08895448(Object08895448 *self, int flags) {
    Object08895448::destroy(self, flags, D_089CA890);
}
}
