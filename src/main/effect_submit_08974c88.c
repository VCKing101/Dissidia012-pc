/* build an effect transform from the owner's palette entry, then submit it. */
typedef struct Color { unsigned char r, g, b; } Color;
typedef struct Kind { short p; unsigned short idx2; } Kind;
typedef struct Sel { char pad[8]; Kind *k; } Sel;
typedef struct Res { char pad[0x48]; Color **tab; int pad2; int v50; } Res;
typedef struct Owner { char pad[0x8C]; Res *res; Sel *sel; } Owner;
typedef struct Rec { char pad[0x10]; float m[4]; char pad2[0x27]; unsigned char id; } Rec;
extern void func_08946AC4(void *self, void *res, int v, int r, int g, int b, void *out, float s);
extern void func_089C2E10(int a, int id, void *rec, void *m, void *out, int z);
void func_08974C88(void *e, Owner *o, Rec *rec, int *box)
{
    float buf[16];
    Res *res = o->res;
    Color *c = res->tab[o->sel->k->idx2];
    func_08946AC4(o, res, res->v50, c->r, c->g, c->b, buf, 1.0f);
    func_089C2E10(*box, rec->id, rec, rec->m, buf, 0);
}
