// Sets the unit's facing from two vectors (angle of their cross direction in xz) and its height to the middle of the entry's range.
typedef struct V4 { float x, y, z, w; } V4;
struct Range { float lo, hi; };
struct Entry { char pad[0x44]; struct Range r; char pad2[0x94 - 0x4C]; };
struct Table { struct Entry e[1]; };
struct Out { char pad[4]; float h; float z; float a, b, c; };
struct Param { char pad[0x11C]; unsigned idx; };
extern struct Table *D_089D4798;
extern void func_08B40804(struct Out *self, V4 *out, struct Param *p);
extern void func_08B40858(struct Out *self, V4 *out, struct Param *p);
extern void func_089A38FC(V4 *dst, V4 *a, V4 *b);
extern float func_088686FC(V4 *v);

void func_08B40BFC(struct Out *self, struct Param *p)
{
    struct { V4 a; V4 b; V4 c; } L;
    struct Range *r = &D_089D4798->e[p->idx].r;
    float angle;
    func_08B40804(self, &L.a, p);
    func_08B40858(self, &L.b, p);
    func_089A38FC(&L.c, &L.a, &L.b);
    L.c.y = 0.0f;
    angle = func_088686FC(&L.c);
    self->a = angle;
    self->b = angle;
    self->c = 0.0f;
    self->h = (r->lo + r->hi) * 0.5f;
    self->z = 0.0f;
}
