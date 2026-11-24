/* Deletes the object whose pointer the fourth argument points to, then clears the pointer. */
struct Obj089597C0 {
    char data[0xC];
    virtual ~Obj089597C0() {}
};
extern "C" void func_089597C0(int a0, int a1, int a2, Obj089597C0 **slot) {
    Obj089597C0 **s = slot;
    if (*s) {
        delete *s;
        *s = 0;
    }
}
