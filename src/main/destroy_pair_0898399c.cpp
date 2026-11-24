/* Calls func_0898A030(a3[0], 3) then deletes the object at a3[1]. */
struct Obj0898399C {
    int data;
    virtual ~Obj0898399C() {}
};
extern "C" void func_0898A030(int, int);
extern "C" void func_0898399C(int a0, int a1, int a2, int *a3) {
    func_0898A030(a3[0], 3);
    delete (Obj0898399C *)a3[1];
}
