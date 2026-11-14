/* Calls the object's first virtual with arg. */
struct Obj088A8274 {
    virtual void run(int arg);
};

extern "C" {

void func_088A8274(Obj088A8274 *o, int arg)
{
    o->run(arg);
}

}
