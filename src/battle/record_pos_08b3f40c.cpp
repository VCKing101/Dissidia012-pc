// Runs four per-frame updates on a battle object, then copies its 2-float position from the indexed record (second set when the target has flag 0x100 and a pointer).
struct Rec08B3F40C { char pad[0x2C]; float a[2]; char pad2[8]; float b[2]; char pad3[0x94 - 0x44]; };
struct Rec2 { float x, y; };
struct Tgt08B3F40C { char pad[0x384]; unsigned flags; char pad2[0x1C]; int **p3a4; bool Hit() { return (flags & 0x100) != 0; } };
struct Obj08B3F40C { char pad[0xE0]; Rec2 pos; char pad2[0x10]; Tgt08B3F40C *tgt; char pad3[0x20]; int idx; };
extern "C" {
extern Rec08B3F40C *D_089D4798;
extern void func_08B3F7F4(void *a, void *b);
extern void func_08B3F500(void *a, void *b);
extern void func_08B3F5A8(void *a, void *b);
extern void func_08B3DE68(void *a, void *b);
void func_08B3F40C(void *a, Obj08B3F40C *o)
{
    Rec08B3F40C *r = &D_089D4798[o->idx];
    func_08B3F7F4(a, o);
    func_08B3F500(a, o);
    func_08B3F5A8(a, o);
    func_08B3DE68(a, o);
    Tgt08B3F40C *t = o->tgt;
    if (1 == t->Hit() && *t->p3a4 != 0) {
        o->pos = *(Rec2 *)r->b;
    } else {
        o->pos = *(Rec2 *)r->a;
    }
}
}
