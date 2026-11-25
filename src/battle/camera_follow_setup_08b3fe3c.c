// Prepares a battle camera-follow record: reset fields, work out the two distances and copy the stage record's values.
typedef struct V4 { float x, y, z, w; } V4;
typedef struct V2 { float x, y; } V2;
typedef struct A { char pad0[8]; float f8, fC, f10, f14; char c18; char pad19[3]; int f1c; } A;
typedef struct B { char pad0[0x40]; char vec40[0x60]; V4 va0; char padb0[0x20]; float fd0, fd4; V2 d8, e0, e8; } B;
typedef struct Rec { char pad[0x20]; float f64; char pad2[0x34 - 0x24]; V2 v78; V2 v80; } Rec;
typedef struct Elem { char pad[0x44]; Rec sub; char pad2[0x94 - 0x44 - sizeof(Rec)]; } Elem;
extern Elem *D_089D4798;
extern float *D_089D4794;
extern int func_08B40994(A *a, B *b);
extern int func_08B40A50(A *a, B *b);
extern void func_08B40804(A *a, V4 *v, B *b);
extern void func_08B40858(A *a, V4 *v, B *b);
extern void func_089A38FC(V4 *d, void *a, void *b);
extern float func_088686FC(V4 *v);
extern void func_089A383C(V4 *d, float x, float y, float z, float w);

void func_08B3FE3C(A *a, B *b)
{
    float *p = D_089D4794;
    Rec *r = &D_089D4798[*(int *)((char *)b + 0x11C)].sub;
    V4 v0, v2, v3, v1;
    float z;
    if ((a->f1c == 1 && func_08B40994(a, b) == 0) || (a->f1c == 2 && func_08B40A50(a, b) == 0)) {
        a->f1c = 0;
    }
    z = 0.0f;
    a->f8 = z;
    func_089A38FC(&v0, b, (char *)b + 0x40);
    a->fC = func_088686FC(&v0);
    a->f14 = z;
    func_08B40804(a, &v2, b);
    func_08B40858(a, &v3, b);
    func_089A38FC(&v1, &v2, &v3);
    v1.y = z;
    a->f10 = func_088686FC(&v1);
    a->c18 = 0;
    b->fd0 = r->f64;
    func_089A383C(&b->va0, 0.0f, 1.0f, 0.0f, 0.0f);
    b->d8 = r->v78;
    b->e0 = r->v78;
    b->e8 = r->v80;
    b->fd4 = *p;
}
