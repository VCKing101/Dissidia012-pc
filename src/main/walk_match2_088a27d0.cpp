// Walks the global node list and applies func_088A1F2C with two floats to each node whose object reports the given id.
struct Ob { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7(); virtual int v8(); };
struct N { char d[0x50]; Ob *ob; int pad; N *next; };
extern N *D_09B4CF98;
extern "C" void func_088A1F2C(N *n, float f, float g);
extern "C" void func_088A27D0(int id, float f, float g)
{
    N *n = 0;
    while ((n = n == 0 ? D_09B4CF98 : n->next) != 0) {
        if (n->ob->v8() == id)
            func_088A1F2C(n, f, g);
    }
}
