typedef struct Wk {
    char pad0[0x400]; char f400[0x480]; int f880[1]; char pad884[0x9F4 - 0x884]; int f9F4;
    char pad9F8[0x15EC - 0x9F8]; char f15EC[0x80]; char f166C[0x80]; char pad16EC[0x1E90 - 0x16EC]; int f1E90;
} Wk;
typedef struct Sc { char pad[0x28F0]; Wk *wk; } Sc;
typedef struct Pk { char pad[0x28]; unsigned short a; unsigned short b; } Pk;
extern Pk D_08A28890;
extern int D_089D6998;
extern int D_08A23FF0;
extern char D_08A289B0[], D_08A28850[];
extern void func_0887BA5C(int);
extern void func_0893476C(char *, char *, int);
extern void func_0887DF14(int, char *, int, char *);
extern int func_0887D868(int, int, char *, int, int, int, int, int);
extern int func_08A1D42C(int, int, int, int, int);
extern void func_088D428C(char *, int *, char *, int);
void func_08A2412C(Sc *self)
{
    char buf[64];
    if (self->wk->f880[D_08A28890.a] != 0) {
        func_0887BA5C(self->wk->f880[D_08A28890.a]);
        self->wk->f880[D_08A28890.a] = 0;
    }
    if (self->wk->f880[D_08A28890.b] != 0) {
        func_0887BA5C(self->wk->f880[D_08A28890.b]);
        self->wk->f880[D_08A28890.b] = 0;
    }
    func_0893476C(buf, D_08A289B0, self->wk->f1E90);
    func_0887DF14(D_089D6998, self->wk->f15EC, 0x80, buf);
    {
        char *lab = self->wk->f15EC;
        self->wk->f880[D_08A28890.a] = func_0887D868(D_089D6998, 0, lab, -1, 0, 0, 0, 0);
    }
    func_0893476C(buf, D_08A289B0, 0x14);
    func_0887DF14(D_089D6998, self->wk->f166C, 0x80, buf);
    {
        char *lab = self->wk->f166C;
        self->wk->f880[D_08A28890.b] = func_0887D868(D_089D6998, 0, lab, -1, 0, 0, 0, 0);
    }
    if (self->wk->f9F4 == 0)
        self->wk->f9F4 = func_08A1D42C(D_08A23FF0, 0, 0x69, 0, 0);
    func_088D428C(self->wk->f400, self->wk->f880, D_08A28850, 1);
}
