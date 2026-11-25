/* constructor: set the table pointer and copy three 4-float rows from the source, then default the last row. */
typedef struct V4 { float x, y, z, w; } V4;
typedef struct Src { V4 r0, r1, r2; } Src;
typedef struct Obj { void *tbl; char pad[12]; V4 r0, r1, r2; float f40, f44, f48, f4C; } Obj;
extern char D_089DBFC0[];
Obj *func_0895EB88(Obj *self, Src *s)
{
    self->tbl = D_089DBFC0;
    self->r0 = s->r0;
    self->r1 = s->r1;
    self->r2 = s->r2;
    self->f40 = 0.0f;
    self->f44 = 0.0f;
    self->f48 = 0.0f;
    self->f4C = 1.0f;
    return self;
}
