/* Calls virtual 0 with 3 on the object the fourth argument points at, when it is set. */
struct Obj {
    char data[0x14];
    virtual void target(int n);
    static void Notify(Obj *o) { if (o != 0) { o->target(3); } }
};
extern "C" void func_0897DF14(int a, int b, int c, Obj **pp)
{
    Obj *o = *pp;
    if (o != 0) {
        Obj::Notify(o);
    }
}
