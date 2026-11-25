typedef struct S { char pad[0x32]; unsigned short lim; float v; } S;
typedef struct Cfg { char pad[0x160]; float rate; } Cfg;
typedef struct Hd { char pad[0x88]; Cfg *cfg; } Hd;
typedef struct E { char pad[0x3E]; unsigned short half; } E;
extern E *func_08985234(int a, int b);
extern Hd *D_09B6F478;
void func_089850A4(int a, S *s, int c) {
    E *e = func_08985234(c, a);
    float rate = D_09B6F478->cfg->rate;
    unsigned n = e->half / 2;
    if (s->lim < n) {
        float r = s->v - (1.0f / (float)n) * rate;
        s->v = r;
        if (r < 0.0f) { s->v = 0.0f; }
    }
}
