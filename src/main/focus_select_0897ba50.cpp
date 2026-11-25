/* Plays the select sound for the focused entry: looks up its slot id through the global table and starts the 5-frame highlight. */
struct Ent { char p0[0x10]; int id; };
struct Tab { char p0[0x30]; Ent **list; };
struct G { char p0[0x1C8]; Tab *t; };
struct Mgr { char p0[0x88]; G *g; char p1[0xA4 - 0x8C]; int hi; };
struct S0897BA50 {
    char p0[0x4C]; int idx; char p1[0x64 - 0x50]; unsigned char kind;
    static int lookup(S0897BA50 *s, Mgr *m)
    {
        int i = s->idx;
        if (i < 0) {
            return -1;
        }
        Ent **l = m->g->t->list;
        if (l == 0) {
            return -1;
        }
        return l[i]->id;
    }
};
extern "C" {
extern Mgr *D_09B6F478;
int func_08983DB4(int a, int b, int c, int d);
void func_08983CD8(int v);
void func_08983B74(int v);
void func_08973334(int a, int b, int c, int d, int e, int f);
void func_089A0384(int v);
int func_0897BA50(S0897BA50 *self)
{
    Mgr *m = D_09B6F478;
    func_08983CD8(func_08983DB4(self->kind, 0, 1, 2));
    func_08983B74(0);
    int hi = m->hi;
    if (hi == 0) {
        return 0;
    }
    int id = S0897BA50::lookup(self, m);
    if (id == -1) {
        return 0;
    }
    func_08973334(hi, id, 5, 1, 1, 1);
    func_089A0384(9);
    return 1;
}
}
