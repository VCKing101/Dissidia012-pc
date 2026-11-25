/* Two spheres touch: distance between the centres is at most the sum of the radii (the length goes through three struct copies). */
typedef struct V3 { float x, y, z; } V3;
typedef struct Obj { char p[0xC]; float radius; } Obj;
extern void func_089A3528(V3 *out, Obj *b, Obj *a);
extern float func_089A37DC(V3 *v);
int func_0887A204(Obj *a, Obj *b)
{
    V3 c;
    V3 d;
    V3 v;
    V3 unused;
    func_089A3528(&v, b, a);
    d = v;
    c = d;
    if (func_089A37DC(&c) > a->radius + b->radius) {
        return 0;
    }
    return 1;
}
