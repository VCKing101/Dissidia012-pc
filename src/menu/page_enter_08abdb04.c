/* Menu page enter: sets up the 0x8F4 sub-block, its two handles and the screen layout. */
typedef struct { char p[0x8C]; int h8C; int h90; } P;
typedef struct {
    char p0[0x8F0]; int f8F0; char a8F4[1];
    char p1[0xA58 - 0x8F5]; unsigned int fA58; int fA5C;
    char p2[0xA70 - 0xA60]; int fA70;
    char p3[0xB51 - 0xA74]; char bB51;
} B;
typedef struct { char p[0x14C]; B *b; } S;
extern int D_089D69E8, D_089D6998;
extern char D_08AC3E0E[], D_08AC3E66[];
extern void func_0887BA5C(int);
extern int func_0887D868(int, int, void *, int, int, int, int, int);
extern int func_0887E534(int);
extern void func_08885364(int, int);
extern void func_088854C4(int, int, int);
extern void func_0888550C(int, int, int, int);
extern void func_088D3CF0(B *, void *, void *);
extern void func_088D428C(void *, void *, void *, int);
extern void func_088D46E4(void *, int, int, int, int, int);
extern int func_08ABC868(S *);
extern void func_08ABDC64(S *);
extern void func_08ABDDE4(S *);
void func_08ABDB04(S *self) {
    P *s1;
    char *s2;
    int ten = 0xA;
    int zero;
    func_088854C4(D_089D69E8, ten, 0x240C);
    func_0888550C(D_089D69E8, self->b->f8F0, 0, 0x1F);
    zero = 0;
    func_08885364(D_089D69E8, zero);
    s2 = (char *)self->b;
    s1 = (P *)(s2 + 0x8F4);
    s2 += 0x400;
    if (s1->h8C != 0) {
        func_0887BA5C(s1->h8C);
        s1->h8C = 0;
    }
    if (s1->h90 != 0) {
        func_0887BA5C(s1->h90);
        s1->h90 = 0;
    }
    s1->h8C = func_0887D868(D_089D6998, 0, (char *)self->b + 0xB2A, -1, 0, 0, 0, 0);
    s1->h90 = func_0887E534(s1->h8C);
    func_088D3CF0(self->b, s2, D_08AC3E0E);
    func_088D428C(s2, s1, D_08AC3E66, 1);
    func_088D46E4((char *)self->b + 0x8AC, self->b->fA70, 0, 1, 0, -1);
    func_08ABDC64(self);
    func_08ABDDE4(self);
    self->b->fA58 |= 0x20;
    self->b->fA5C = func_08ABC868(self);
    self->b->bB51 = 0;
}
