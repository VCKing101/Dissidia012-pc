/* Sets the vtable, the float at +0x50 and copies a 64-byte matrix into +0x10. */
typedef struct M { int v[16]; } M;
typedef struct S { int vt; char p[0xC]; M m; float f; } S;
extern int D_089DC050[];
S *func_0895F9E4(S *self, M *src, float f) {
    self->vt = (int)D_089DC050;
    self->f = f;
    self->m = *src;
    return self;
}
