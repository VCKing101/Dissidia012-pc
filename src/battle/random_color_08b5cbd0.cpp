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

void func_08B5CBD0(Col *out)
{
    Col c;
    c.r = ColRand::between(0.6f, 1.4f);
    c.g = ColRand::between(0.05f, 0.25f);
    c.b = ColRand::between(0.6f, 1.4f);
    func_089A3A44(ColRand::between(0.0015f, 0.002f), &c, &c);
    *out = c;
}

}
