/* Deletes the object the fourth argument points to (virtual destructor, flags 3; vtable pointer at 0x20). */
struct Obj0896608C {
    char data[0x20];
    virtual ~Obj0896608C() {}
};
extern "C" void func_0896608C(int a0, int a1, int a2, Obj0896608C **a3) {
    delete *a3;
}
