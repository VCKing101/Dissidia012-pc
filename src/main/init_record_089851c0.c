// Fills a record from a source object: copies selected floats, converts a byte to float, keeps pointers.
struct Src089851C0 {
    char p0[0x10]; char inner[0x10];
    float f20, f24, f28, f2C, f30, f34, f38;
    char p3C[0x12]; unsigned short h4E, h50; char p52[0xA]; unsigned char b5C;
};
struct Dst089851C0 {
    int zero; struct Src089851C0 *src;
    float f8; float fC; float f10, f14, f18, f1C, f20, f24;
    int i28, i2C; struct Src089851C0 *s30; char *inner;
};
struct Dst089851C0 *func_089851C0(struct Dst089851C0 *d, struct Src089851C0 *s)
{
    d->zero = 0;
    d->src = s;
    d->f8 = s->f34;
    d->fC = (float)s->b5C;
    d->f10 = s->f38;
    d->f14 = s->f28;
    d->f18 = s->f2C;
    d->f1C = s->f20;
    d->f20 = s->f24;
    d->f24 = s->f30;
    d->i28 = s->h4E;
    char *in = s->inner;
    int t = s->h50;
    d->s30 = s;
    d->i2C = t;

    d->inner = in;
    return d;
}
