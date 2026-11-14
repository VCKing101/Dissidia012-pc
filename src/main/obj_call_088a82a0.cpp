/* Calls the object's first virtual with arg. */
struct Obj088A82A0 {
    virtual void run(int arg);
};

extern "C" {

void func_088A82A0(Obj088A82A0 *o, int arg)
{
    o->run(arg);
}

}
