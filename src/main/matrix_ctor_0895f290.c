/* constructor: set the table pointer, copy a 4x4 matrix and a 4-float vector from the arguments. */
typedef struct V4 { float x, y, z, w; } V4;
typedef struct M16 { int w[16]; } M16;
typedef struct Obj { void *tbl; char pad[12]; M16 m; V4 v; } Obj;
extern char D_089DC008[];
Obj *func_0895F290(Obj *self, M16 *m, V4 *v)
{
    self->tbl = D_089DC008;
    self->m = *m;
    self->v = *v;
    return self;
}
