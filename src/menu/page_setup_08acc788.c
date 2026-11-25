/* Menu page setup: loads the selected layout table entry, then eight rows of two lists. */
typedef struct { int a; int b; } T8;
typedef struct { void *p; char pad[0x34]; } T38;
typedef struct { char p0[0x400]; char a400[1]; char p1[0x880 - 0x401]; char a880[1];
    char p2[0xFF0 - 0x881]; char aFF0[1]; char p3[0x1224 - 0xFF1]; int f1224; } B;
typedef struct { char p[0xDB30]; B *b; } S;
extern T8 D_08ADA3E0[];
extern T38 D_08ADA428[];
extern void func_088D3F00(void *, void *, void *);
extern void func_088D44EC(void *, void *, void *);
extern void func_088D4B74(void *, int);
extern void func_08ACC020(S *);
void func_08ACC788(S *self) {
    int i;
    int sel = self->b->f1224;
    if (sel != -1) {
        func_088D3F00(self->b, self->b->a400, D_08ADA428[D_08ADA3E0[sel].a].p);
        func_088D3F00(self->b, self->b->a400, (char *)self->b + 0xC500);
        for (i = 0; i < 8; i++) {
            func_088D3F00(self->b, self->b->a400, (char *)self->b + i * 0x38 + 0xC548);
            func_088D44EC(self->b->a400, self->b->a880, (char *)self->b + i * 0x28 + 0xC708);
        }
        func_08ACC020(self);
        func_088D4B74(self->b->aFF0, 0);
    }
}
