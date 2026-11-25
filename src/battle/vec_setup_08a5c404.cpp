struct V4 { float x, y, z, w; };
struct Rec {
    unsigned char a, b, c, pad3;
    float f4, f8, fc;
    int i10;
    char pad14[0xC];
    V4 v20;
    V4 v30;
};
struct Sub { V4 v; int e0; };
struct Src { char pad[0xD0]; Sub sub; char pad2[0x10]; int f4; };
struct Cfg { char pad[0xA8]; float f; };
struct Ctx {
    char p0[0x80];
    V4 v80;
    char p1[0x314 - 0x90];
    Src *src;
    char p2[0x384 - 0x318];
    unsigned flags;
    char p3[0x39C - 0x388];
    Cfg *cfg;
    char p4[0x3A8 - 0x3A0];
    Rec *rec;
};
extern "C" {
void func_08935828(void *, int, int);
void func_089A38E0(V4 *, V4 *, V4 *);
void func_089A3BC8(V4 *, V4 *);
void func_08835C7C(void *, V4 *);
float func_089A3C18(V4 *, V4 *);
float func_089A4500(float);
void func_08834934(void *, int, int);
}

extern "C" void func_08A5C404(void *unused, Ctx *c) {
    Rec *r = c->rec;
    Src *s = c->src;
    Cfg *cfg = c->cfg;
    Sub *sb = &s->sub;
    s->f4 = 0;
    func_08935828(r, 0, 0x40);
    r->v20 = sb->v;
    r->i10 = sb->e0;
    V4 d;
    V4 b;
    V4 a;
    a.x = r->v20.x; a.y = r->v20.y; a.z = r->v20.z; a.w = r->v20.w;
    V4 *pa = &a;
    V4 *pb = &b;
    V4 *cam = &c->v80;
    func_089A38E0(pb, pa, cam);
    func_089A3BC8(pb, pb);
    d = b;
    func_08835C7C(c, &d);
    r->v30.x = b.x;
    r->v30.y = b.y;
    r->v30.z = b.z;
    r->fc = func_089A3C18(pa, cam) / cfg->f;
    float t = func_089A4500(a.y - c->v80.y);
    if (t <= 1.0f) r->b = 1;
    float cf = cfg->f;
    float z = 0;
    r->a = 1;
    r->f4 = cf;
    r->f8 = z;
    r->c = 0;
    func_08834934(c, -1, 0xDD);
    c->flags |= 0x1000;
    c->flags &= ~0x200;
}
