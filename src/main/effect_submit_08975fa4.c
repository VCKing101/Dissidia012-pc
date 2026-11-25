/* build an effect transform from the owner's palette entry, then submit it. */
typedef struct Color { unsigned char r, g, b; } Color;
typedef struct Kind { short p; unsigned short idx2; } Kind;
typedef struct Sel { char pad[8]; Kind *k; } Sel;
typedef struct Res { char pad[0x48]; Color **tab; int pad2; int v50; } Res;
typedef struct Owner { char pad[0x8C]; Res *res; Sel *sel; } Owner;
typedef struct Rec { char pad[0x3F]; unsigned char id; char pad2[0x10]; float m[4]; float n[4]; } Rec;
extern void func_08946AC4(void *self, void *res, int v, int r, int g, int b, void *out, float s);
extern void func_089C30F4(int a, int id, void *m, void *n, void *out, int z);
void func_08975FA4(void *e, Owner *o, Rec *rec, int *box)
{
    float buf[16];
    Res *res = o->res;
    Color *c = res->tab[o->sel->k->idx2];
    func_08946AC4(o, res, res->v50, c->r, c->g, c->b, buf, 1.0f);
    func_089C30F4(*box, rec->id, rec->m, rec->n, buf, 0);
}
