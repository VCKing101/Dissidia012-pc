struct V4 { float x, y, z, w; };
struct PObj { char pad0[0x80]; V4 pos; char pad90[0x30C]; void *h39c; };
struct Ent { short a; short b; short tab[1]; };
struct Link { char pad0[4]; PObj *obj; };
struct Self { unsigned char active; char pad1[3]; Link *link; };
extern float D_08AB2EC8;
extern "C" {
extern int func_08A80200(int);
extern int func_08AAE300(Self *);
extern int func_08AAE84C(Self *);
extern int func_08AB249C(Self *);
extern Ent *func_08AAC4B8(int, int, int);
extern Ent *func_08AACEE0(int, int, int);
extern PObj *func_08A973A8(Link *);
extern void func_089A38FC(V4 *, V4 *, V4 *);
extern float func_089A3BF8(V4 *);
extern float func_0884E474(void *);
extern float func_0884E47C(void *);
extern int func_08AAEF94(Self *);
extern int func_088374B0(PObj *);
extern int D_08A80218;

int func_08AAEC0C(Self *self)
{
    if (self->active == 0) return -1;
    Ent *e;
    int t = func_08A80200(D_08A80218);
    int k = func_08AAE300(self);
    int kd = func_08AAE84C(self);
    if (func_08AB249C(self) == 0) {
        e = func_08AAC4B8(t, k, kd);
        if (e == 0) return -1;
        return e->b;
    }
    e = func_08AACEE0(t, k, kd);
    if (e == 0) return -1;
    Link *l = self->link;
    PObj *o = l->obj;
    if (o == 0) return -1;
    PObj *p = func_08A973A8(l);
    if (p == 0) return -1;
    void *h = o->h39c;
    if (h == 0) return -1;
    V4 v;
    V4 *op = &o->pos;
    func_089A38FC(&v, &p->pos, op);
    float z = v.y;
    v.y = 0.0f;
    float len = func_089A3BF8(&v);
    int idx = 3;
    if (z < D_08AB2EC8) {
        idx = 0;
        if (len > func_0884E474(h)) {
            if (len <= func_0884E47C(h)) idx = 1; else idx = 2;
        }
    }
    unsigned fa = func_08AAEF94(self) != 0;
    unsigned fb = func_088374B0(o) != 0;
    char *q = (char *)e + 2;
    return *(short *)(q + idx * 8 + fa * 4 + fb * 2);
}
}
