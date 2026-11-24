// Builds a query record (kind 0x400 plus a bit for the id from func_088A663C) and, when func_088AA06C finds a match in the global list, calls func_088A7284 with the position at +0x100.
extern "C" {
extern int func_088A663C(int);
extern int func_088AA06C(int, void *, int, int, int);
extern void func_088A7284(int, float, float, float);
}
struct Query57 {
    int a;
    int kind;
    unsigned bits[2];
    static void setBit(Query57 *q, int idx) { int w = idx / 32; int r = idx % 32; unsigned cur = q->bits[w]; int one = 1; q->bits[w] = cur | (one << r); }
};
struct Cfg57 {
    char pad0[2];
    short id;
};
struct Own57 {
    char pad0[0x100];
    float x;
    float y;
    float z;
    char pad1[0x2F8 - 0x10C];
    Cfg57 *cfg;
};
extern "C" int *D_089D6DD0;
extern "C" void func_08B30334(Own57 *self)
{
    Cfg57 *c = self->cfg;
    Query57 q;
    q.a = 0;
    q.bits[0] = 0;
    q.bits[1] = 0;
    q.kind = 0x400;
    Query57::setBit(&q, func_088A663C(c->id));
    {
        int list = *D_089D6DD0;
        if (list != 0) {
            int found = func_088AA06C(list, &q, 0, 0, 0);
            if (found != 0) {
                func_088A7284(found, self->x, self->y, self->z);
            }
        }
    }
}
