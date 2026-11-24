// Adjusts a float field by two scaled values, calls virtual slot 2, returns 1.
struct G; struct F;
struct V { virtual void v0(); virtual void v1(); virtual void v2(int b, struct G *g, struct F **p, float s); };
struct F { char d[4]; float x; };
struct G { char d[0x14]; float w; };
extern "C" int func_0895A434(V *o, int b, G *g1, F **pp, G *g2, float s1, float s2)
{
    float a = g2->w * s2;
    F *f = *pp;
    float c = g1->w * s1;
    f->x = a + f->x - c;
    o->v2(b, g1, pp, s1);
    return 1;
}
