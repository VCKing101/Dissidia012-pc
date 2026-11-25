/* copy the basis vectors of a 4x4 matrix into the object's rows, plus three scale terms. */
typedef struct V3 { float x, y, z; } V3;
typedef struct Src { V3 r0; float w0; V3 r1; float w1; V3 r2; float w2; } Src;
typedef struct Dst { V3 a; float p0; V3 b; float p1; V3 c; float p2; V3 d; float p3; float f40, f44, f48, f4C; } Dst;
int func_0895ABDC(Dst *d, Src *s)
{
    d->a = s->r0;
    d->b = s->r0;
    d->c = s->r1;
    d->d = s->r2;
    d->f44 = s->w0;
    d->f40 = s->w0;
    d->f48 = s->w1;
    d->f4C = s->w2;
    return 1;
}
