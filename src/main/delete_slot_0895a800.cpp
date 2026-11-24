/* Deletes the object whose pointer the fourth argument points to, then clears the pointer. */
struct Obj0895A800 {
    char data[0xC];
    virtual ~Obj0895A800() {}
};
extern "C" void func_0895A800(int a0, int a1, int a2, Obj0895A800 **slot) {
    Obj0895A800 **s = slot;
    if (*s) {
        delete *s;
        *s = 0;
    }
}
