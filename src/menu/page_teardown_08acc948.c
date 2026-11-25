/* Menu page teardown: unloads the layout lists, frees two handles and clears the selection. */
typedef struct { int a; int b; } T8;
typedef struct { void *p; char pad[0x34]; } T38;
typedef struct { char p0[0x400]; char a400[1]; char p1[0x880 - 0x401]; char a880[1];
    char p2[0x954 - 0x881]; int f954; int f958; char p3[0x1224 - 0x95C]; int f1224; } B;
typedef struct { char p[0xDB30]; B *b; } S;
extern T8 D_08ADA3E0[];
extern T38 D_08ADA428[];
extern void func_0887BA5C(int);
extern void func_088D3EA0(void *, void *, void *);
extern void func_088D442C(void *, void *, void *);
void func_08ACC948(S *self) {
    int i;
    func_088D3EA0(self->b, self->b->a400, D_08ADA428[D_08ADA3E0[self->b->f1224].a].p);
    func_088D3EA0(self->b, self->b->a400, (char *)self->b + 0xC500);
    for (i = 0; i < 8; i++) {
        func_088D3EA0(self->b, self->b->a400, (char *)self->b + i * 0x38 + 0xC548);
        func_088D442C(self->b->a400, self->b->a880, (char *)self->b + i * 0x28 + 0xC708);
    }
    if (self->b->f954 != 0) {
        func_0887BA5C(self->b->f954);
        self->b->f954 = 0;
    }
    if (self->b->f958 != 0) {
        func_0887BA5C(self->b->f958);
        self->b->f958 = 0;
    }
    self->b->f1224 = -1;
}
