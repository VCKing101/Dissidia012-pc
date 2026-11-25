/* Blends between two neighbouring entries of a four-colour table for a cycling time value. */
typedef struct Rgb { unsigned char r, g, b, a; } Rgb;
typedef union RgbWord { Rgb c; unsigned w; } RgbWord;
extern unsigned D_09B6CD50[4];
extern float D_08B59DB8;

unsigned func_08B58BD0(float t)
{
    RgbWord out;
    RgbWord c0;
    RgbWord c1;
    float seg = D_08B59DB8;
    int k = (int)(t / seg);
    float f;
    f = t - (float)k * seg;
    unsigned *tb = D_09B6CD50;
    unsigned *p0 = (unsigned *)(k % 4 * 4 + (int)tb);
    unsigned *p1;
    c0.w = *p0;
    p1 = (unsigned *)((k + 1) % 4 * 4 + (int)tb);
    c1.w = *p1;
    f = f / seg;
    out.c.r = (c1.c.r - c0.c.r) * f + c0.c.r;
    out.c.g = (c1.c.g - c0.c.g) * f + c0.c.g;
    out.c.b = (c1.c.b - c0.c.b) * f + c0.c.b;
    return out.w;
}
