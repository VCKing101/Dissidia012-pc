struct V4 { float x, y, z, w; };
struct V3 { float x, y, z; };
struct V4b { V3 v; float w; };
struct Pair { float a, b; };
struct Cfg { char p[0x10]; Pair pr; };
struct Rec { float f0; char p[0xC]; float v[3]; float pad1c; unsigned char flag; };
struct Ch {
    char p0[0x130];
    char blk[0x50];
    short s180;
    char p1[0x1A8 - 0x182];
    float f1a8;
    char p1b[0x314 - 0x1AC];
    Cfg *cfg;
    char p1c[0x384 - 0x318];
    unsigned flags;
    char p2[0x3A8 - 0x388];
    Rec *rec;
};
extern "C" {
float func_08838300(Ch *);
void func_0882FA10(float, float, void *, int, int);
void func_0882FB90(void *, int, float, float);
void func_08834934(Ch *, int, int);
void func_0883497C(Ch *, float);
int func_088380F8(Ch *);
int func_089A3B70(V4 *);
void func_0883C7BC(Ch *, V4 *);
}
struct H { static bool eq1(int x) { return x == 1; } };
extern float D_08A61778, D_08A6177C, D_089C7C2C;
extern "C" void func_08A61298(void *unused, Ch *c) {
    Rec *r = c->rec;
    Pair *pr = &c->cfg->pr;
    float z = 0.0f;
    int pos = 1;
    if (r->f0 <= z) pos = 0;
    float t = func_08838300(c);
    r->f0 = r->f0 + t;
    if (pr->b < pr->a) {
        char *blk = c->blk;
        if (c->s180 == 700) {
            func_0882FA10(D_08A61778, z, blk, 0x2BD, 0);
        } else if (c->s180 == 0x2EE) {
            func_0882FA10(D_08A61778, z, blk, 0x2EF, 0);
        }
        func_0882FB90(blk, 0x65, D_089C7C2C, z);
        c->flags = c->flags | 0x800;
        func_08834934(c, -1, 0x66);
        func_0883497C(c, z);
    } else if (pos && func_088380F8(c) == 0) {
        V4 v;
        float ax = r->v[0], ay = r->v[1], az = r->v[2];
        v.x = ax; v.y = ay; v.z = az;
        v.w = 1.0f;
        if (!H::eq1(func_089A3B70(&v))) {
            if (r->flag != 0) {
                if (D_08A6177C < c->f1a8) {
                    func_0883C7BC(c, &v);
                }
            } else {
                func_0883C7BC(c, &v);
            }
        }
    }
}
