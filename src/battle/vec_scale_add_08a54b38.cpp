struct V4 { float x, y, z, w; };
struct Ch {
    char p0[0x180];
    short s180;
    char p1[0x1A8 - 0x182];
    float f1a8;
    char p2[0x550 - 0x1AC];
    V4 v550;
};
struct Ent { int pad; short s4; };
extern "C" {
void func_08A5B59C(V4 *);
int func_0883739C(Ch *);
void *func_08835778(Ch *);
Ent *func_088BF4CC(void *, int);
float func_08838300(Ch *);
float func_089A46DC(float);
void func_089A3A44(V4 *, float, V4 *);
void func_089A38A8(V4 *, V4 *, V4 *);
}
extern float D_08A54D80, D_08A54D84, D_08A54D88;
extern "C" void func_08A54B38(V4 *out, int unused, Ch *c) {
    V4 res;
    V4 base;
    V4 tmp;
    func_08A5B59C(&base);
    res = base;
    if (func_0883739C(c) != 0) {
        float f = D_08A54D84;
        Ch *keep = c;
        Ent *e = func_088BF4CC(func_08835778(c), 0x127);
        if (e != 0) {
            if (func_0883739C(c) != 0) {
                short *sp = &e->s4;
                f = f + (float)*sp * 0.001f;
            }
        }
        if (c->s180 == 0x1B8 && c->f1a8 < D_08A54D88) {
            f = f * func_089A46DC(c->f1a8 / D_08A54D88 * D_08A54D80 * 0.5f);
        } else {
            f = 0.0f;
        }
        float g = func_08838300(c);
        V4 *pt = &tmp;
        func_089A3A44(pt, f * g, &keep->v550);
        func_089A38A8(&res, &res, pt);
    }
    *out = res;
}
