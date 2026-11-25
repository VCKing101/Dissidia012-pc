/* Refreshes a sub-object: runs five update calls on its parts and re-applies the vector from its getter. */
typedef struct V3 { float x, y, z; } V3;
typedef struct P45D0C { char pad0[0x20]; V3 v; char pad1[0x154 - 0x2C]; char sub[0x10]; } P45D0C;
typedef struct S45D0C { char pad[0x18]; P45D0C *p; } S45D0C;
extern void func_08B4A49C(void *);
extern void func_08B45A14(void *);
extern void func_08B48974(void *, void *);
extern void func_08920BC0(void *, void *);
extern void func_08B4ADB4(V3 *, void *);
extern void func_089A34C0(void *, V3 *);
extern void func_08B4A51C(void *);
void func_08B45D0C(S45D0C *self)
{
    V3 v;
    V3 r;
    void *sub;
    func_08B4A49C(&self->p->v);
    func_08B45A14((char *)self->p + 0x28);
    func_08B48974((char *)self->p + 0x24, &self->p->v);
    func_08920BC0(*(void **)((char *)self->p + 0x1C), &self->p->v);
    sub = self->p->sub;
    func_08B4ADB4(&r, &self->p->v);
    v = r;
    func_089A34C0(sub, &v);
    func_08B4A51C(&self->p->v);
}
