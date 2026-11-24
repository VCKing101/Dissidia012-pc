/* Creates the object held at +0x90 (constructing it when allocation succeeded) and registers it with its id list. */
extern int func_088627B4(void);
extern void *func_0885C6CC(int a, int b, int c, int d, int e, int f);
extern void func_08B4785C(void *p);
extern void func_08B479A0(void *a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k);
typedef struct { char pad0[4]; int v4; char pad1[0x30]; int v38; char pad2[0x14]; int v50; int v54; char pad3[0x38]; void *h90; } Obj;
void func_08B48A50(Obj *self) {
    void *p;
    int h = func_088627B4();
    self->v38 = h;
    int one = 1;
    p = self->h90 = func_0885C6CC(h, 0x55C, 0, 0, one, 0);
    void *q = 0;
    if (p != 0) {
        func_08B4785C(p);
        q = p;
    }
    self->h90 = q;
    func_08B479A0(q, self->v4, 1, 0, 0x24, 0x8A22, 0x8826, 0x8A24, 0x8827, 0x8A20, 0x8802);
    self->v54 = 0;
}
