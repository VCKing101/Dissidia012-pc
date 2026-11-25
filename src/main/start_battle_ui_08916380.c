/* Clears the +0x7D7 flag, starts effect 3 with scale 16, runs step 0xE, then applies three values: a1, the id at +0x3C->0xC, and a2. */
typedef struct Info { char p[0xC]; signed char id; } Info;
typedef struct S { char p[0x3C]; Info *info; char q[0x797]; unsigned char flag; } S;
extern void func_08873D34(int a, int b, int c, int d, int e, float f);
extern void func_08873DEC(int a, int b);
extern void func_08914B64(S *self, int n);
extern void func_08B48604(int v);
extern void func_08B48660(int v);
extern void func_08B48614(int v);
void func_08916380(S *self, int a, int b) {
    self->flag = 0;
    func_08873D34(3, 0, 0, 0, 2, 16.0f);
    func_08873DEC(3, 2);
    func_08914B64(self, 0xE);
    func_08B48604(a);
    func_08B48660(self->info->id);
    func_08B48614(b);
}
