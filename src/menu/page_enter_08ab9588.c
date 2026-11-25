/* Menu page enter: loads the list, picks the row source by flag, and sets window icons from the entry table. */
typedef struct { char p[0xE]; short h0E; } W;
typedef struct { unsigned short h0; unsigned short h2; char pad[10]; } E;
typedef struct {
    char p0[0x400]; char a400[1];
    char p1[0x470 - 0x401]; int f470; W *w474;
    char p2[0x484 - 0x478]; W *w484;
    char p3[0xD30 - 0x488]; unsigned int fD30;
    char p4[0xD38 - 0xD34]; int fD38; int fD3C; int fD40; int fD44;
} B;
typedef struct { char p[0x138]; B *b; } S;
extern E D_08ABB7FA[];
extern char D_08ABB7D2[];
extern void func_088D3B88(B *, void *, void *, int, int);
extern void func_088D9F5C(void *, int, int);
extern int func_08AB8EF4(S *);
extern void func_08AB90F8(S *);
extern void func_08AB91A8(S *, int);
extern void func_08AB9430(S *, int);
extern int func_08AB96BC(S *);
void func_08AB9588(S *self, unsigned char flag) {
    E *e;
    func_088D3B88(self->b, self->b->a400, D_08ABB7D2, 0, 0x120);
    self->b->fD3C = 0;
    if (flag) {
        self->b->fD40 = func_08AB96BC(self);
    } else {
        self->b->fD40 = func_08AB8EF4(self);
    }
    func_08AB90F8(self);
    func_088D9F5C(self->b->w474, 0, 1);
    {
        short one;
        one = 1;
        self->b->w474->h0E = one;
    }
    e = &D_08ABB7FA[self->b->fD44];
    func_088D9F5C(self->b->w484, e->h0, 1);
    self->b->w484->h0E = e->h2;
    func_088D9F5C((void *)self->b->f470, 0, 1);
    func_08AB91A8(self, 0);
    func_08AB9430(self, 0);
    self->b->fD38 = 0;
    self->b->fD30 |= 0x140;
}
