// Copies two rows of a source record into the destination through temporary vectors, and its 0x2E word.
typedef struct V4 { float x, y, z, w; } V4;
struct Src { V4 r0; V4 pad; V4 r2; char pad2[0xB8 - 0x30]; float f2e; };
struct Dst { V4 a; V4 b; float f20; };
extern void func_089A3868(void *dst, V4 *src);

void func_08AD021C(struct Dst *dst, struct Src *src)
{
    struct { V4 out; V4 t1; V4 t2; V4 p1; V4 p2; } L;
    L.t1 = src->r0;
    L.out = L.t1;
    func_089A3868(&dst->a, &L.out);
    L.t2 = src->r2;
    L.out = L.t2;
    func_089A3868(&dst->b, &L.out);
    dst->f20 = src->f2e;
}
