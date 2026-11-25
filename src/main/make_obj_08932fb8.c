// Runs the base init, creates the one-entry object array from +0x2C, registers it and sets its state to 4.
extern void func_08933034(void *);
extern int func_088DAA68(int);
extern void func_088D3CF0(int, void *, const void *);
extern char D_089CF340[];
struct S { char pad[0x2C]; int src; int pad2; int *o[1]; };
void func_08932FB8(struct S *self) {
    int i;
    struct S *p;
    { struct S *t = self; func_08933034(t); }
    i = 0;
    p = self;
    do { p->o[0] = (int *)func_088DAA68(self->src); i++; p = (struct S *)((char *)p + 4); } while (i < 1);
    func_088D3CF0(0, self->o, D_089CF340);
    *(short *)((char *)self->o[0] + 0xE) = 4;
}
