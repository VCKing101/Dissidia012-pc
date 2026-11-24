/* Deletes the object the fourth argument points to (virtual destructor, flags 3; vtable pointer at 0x20). */
struct Obj089674AC {
    char data[0x20];
    virtual ~Obj089674AC() {}
};
extern "C" void func_089674AC(int a0, int a1, int a2, Obj089674AC **a3) {
    delete *a3;
}
