typedef struct Vec4 { float x; float y; float z; float w; } Vec4;
typedef struct Masks { unsigned long long bits[4]; Vec4 color; float alpha; } Masks;

extern void func_089A3890(Vec4 *v);

void func_08871B40(Masks *m, int unused, unsigned long long keep)
{
    m->bits[0] &= keep;
    m->bits[1] &= keep;
    m->bits[2] &= keep;
    m->bits[3] &= keep;
    m->alpha = 0.0f;
    func_089A3890(&m->color);
}
