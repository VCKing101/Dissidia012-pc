// Clears a float field then calls virtual slots 7 (with 0) and 5.
struct C { char data[0x1C]; float f; int pad; virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7(int a); };
extern "C" void func_089313C0(C *o) { o->f = 0.0f; o->v7(0); o->v5(); }
