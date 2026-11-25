// Rolls a random colour (red/green/blue in set ranges), scales it by a random factor and stores it.
struct Col { float r, g, b, a; };
extern int D_09AFB89C;

struct ColRand {
    static int next()
    {
        return (D_09AFB89C = D_09AFB89C * 0x343FD + 0x269EC3) >> 16 & 0x7FFF;
    }
    static float between(float least, float most)
    {
        return (float)next() * (1.0f / 32767.0f) * (most - least) + least;
    }
};

extern "C" {

extern void func_089A3A44(float scale, Col *dst, Col *src);

extern void func_089A3868(Col *dst, void *src);
extern void func_089A3B98(Col *dst, Col *src);
extern void func_089A3B30(Col *dst, void *a, Col *b);
extern char D_09AF84E0[];
void func_08B57268(Col *out, int unused, void *p)
{
    Col c, base;
    func_089A3868(&base, p);
    base.g = 0.0f;
    func_089A3B98(&base, &base);
    func_089A3B30(&base, D_09AF84E0, &base);
    c.r = ColRand::between(-0.4f, 0.4f) + base.r;
    c.g = ColRand::between(0.05f, 0.25f);
    c.b = ColRand::between(-0.4f, 0.4f) + base.b;
    func_089A3A44(ColRand::between(0.0035f, 0.004f), &c, &c);
    *out = c;
}

}
