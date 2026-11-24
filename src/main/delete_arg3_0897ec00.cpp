/* Deletes the object the fourth argument points to (virtual destructor, flags 3; vtable pointer at 0x14). */
struct Obj0897EC00 {
    char data[0x14];
    virtual ~Obj0897EC00() {}
};
extern "C" void func_0897EC00(int a0, int a1, int a2, Obj0897EC00 **a3) {
    delete *a3;
}
