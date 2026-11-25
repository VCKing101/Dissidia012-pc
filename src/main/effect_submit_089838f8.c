/* build an effect transform from the owner's palette entry, then finalise and submit it. */
typedef struct Color { unsigned char r, g, b; } Color;
typedef struct Kind { char pad[8]; unsigned short idx8; } Kind;
typedef struct Sel { char pad[8]; Kind *k; } Sel;
typedef struct Res { char pad[0x48]; Color **tab; int pad2; int v50; } Res;
typedef struct Owner { char pad[0x8C]; Res *res; Sel *sel; } Owner;
extern void func_08946AC4(void *self, void *res, int v, int r, int g, int b, void *out, float s);
extern void func_0896FB5C(void *buf);
extern void func_0898A2C8(int a, void *b, void *c);
extern void *D_089DA278;
void func_089838F8(void *e, Owner *o, void *arg, int *box)
{
    float buf[16];
    Res *res = o->res;
    Sel *sel = o->sel;
    Color *c = res->tab[sel->k->idx8];
    void *g = D_089DA278;
    func_08946AC4(o, res, res->v50, c->r, c->g, c->b, buf, 1.0f);
    func_0896FB5C(buf);
    func_0898A2C8(*box, arg, g);
}
