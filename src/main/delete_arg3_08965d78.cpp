/* Deletes the object the fourth argument points to (virtual destructor, flags 3; vtable pointer at 0x20). */
struct Obj08965D78 {
    char data[0x20];
    virtual ~Obj08965D78() {}
};
extern "C" void func_08965D78(int a0, int a1, int a2, Obj08965D78 **a3) {
    delete *a3;
}
