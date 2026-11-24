// Forwards its arguments to virtual slot 1 with the second float argument, returns 1.
struct V { virtual void v0(); virtual void v1(int a, int b, int c, int d, float x); };
extern "C" int func_0897B8E4(V *o, int a, int b, int c, int d, float f, float g) { o->v1(a, b, c, d, g); return 1; }
