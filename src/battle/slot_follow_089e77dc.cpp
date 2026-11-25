struct Ob { char p[0x20]; unsigned short fl; };
struct Sub { char p[0x4C]; int v; };
struct Tgt { char p[0x394]; char *a; };
struct Sl { int pad[2]; void *h8; void *hc; int h10; };
struct V3 { float x, y, z, w; };
extern "C" {
void *func_089E95B0(int);
void func_088DA0A8(void *, int, void *, void *);
void func_088D9B84(void *, void *);
void func_088D9AF4(void *, void *);
void func_08838DCC(void *, V3 *, int);
float func_0884E46C(void *);
int func_08866E04(V3 *, V3 *);
void func_088D9C60(void *, int, int);
void func_088DA060(...);
}
extern int D_089E799C[];
extern float D_089E7998;
struct H { static bool has(unsigned v) { return (v & 0x20) != 0; } };
extern "C" int func_089E77DC(Sl *s) {
    void *h = s->h8;
    if (h != 0) {
        Ob *o = (Ob *)s->hc;
        if (o != 0) {
            if (!H::has(o->fl)) {
                Tgt *t = (Tgt *)func_089E95B0(s->h10);
                if (t == 0) return -1;
                void *u = *(void **)((char *)t + 0x39C);
                if (u == 0) return -2;
                char buf[0x40];
                char buf2[0x10];
                func_088DA0A8(s->h8, D_089E799C[s->h10], buf, buf2);
                func_088D9B84(s->hc, buf);
                func_088D9AF4(s->hc, buf2);
                char *q = *(char **)((char *)t + 0x394);
                if (q == 0) return -3;
                Sub *sub = (Sub *)(q + 0xA4);
                V3 v;
                func_08838DCC(t, &v, sub->v);
                float f = func_0884E46C(u);
                v.y = f * 0.5f + D_089E7998 + v.y;
                if (func_08866E04(&v, &v) == 0) return -4;
                func_088D9C60(s->hc, (int)v.x, (int)v.y);
            } else {
                if (o != 0) {
                    func_088DA060(o);
                    s->hc = 0;
                    h = s->h8;
                }
                if (h != 0) {
                    func_088DA060();
                    s->h8 = 0;
                }
            }
        }
    }
    return 0;
}
