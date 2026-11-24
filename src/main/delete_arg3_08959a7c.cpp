/* Deletes the object the fourth argument points to (virtual destructor, flags 3; vtable pointer at 0x0). */
struct Obj08959A7C {
    virtual ~Obj08959A7C() {}
};
extern "C" void func_08959A7C(int a0, int a1, int a2, Obj08959A7C **a3) {
    delete *a3;
}
