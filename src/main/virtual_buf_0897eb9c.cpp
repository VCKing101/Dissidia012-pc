// Builds a 56-byte record from a2 and passes it with a1 to virtual slot 1 of **a3.
struct O { char data[0x14]; virtual void v0(); virtual void v1(void *rec, int a); };
extern "C" void *func_089851C0(void *buf, int src);
extern "C" void func_0897EB9C(int a0, int a1, int a2, O **a3)
{
    O *o = *a3;
    char buf[56];
    o->v1(func_089851C0(buf, a2), a1);
}
