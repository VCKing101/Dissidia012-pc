/* Builds a table from the global at D_089D8518, looks the argument up in it and, when the entry's byte at +8 differs from the object's value at +0x10, calls func_0888A2B4(-1, 0). */
typedef struct Ent08B363E8 { char c; char pad[19]; } Ent08B363E8;
typedef struct Tab08B363E8 { int h0; int h1; Ent08B363E8 e[27]; } Tab08B363E8;
typedef struct Obj08B363E8 { char pad[0x10]; int v; } Obj08B363E8;
extern int D_089D8518;
extern void func_0891F388(Tab08B363E8 *out, int src);
extern int func_0891A5B8(Tab08B363E8 *t, int a, int b);
extern void func_0888A2B4(int a, int b);
void func_08B363E8(Obj08B363E8 *self, int arg)
{
    Tab08B363E8 a;
    Tab08B363E8 b;
    int v = self->v;
    int i;
    func_0891F388(&b, D_089D8518);
    a = b;
    i = func_0891A5B8(&a, 0, arg);
    if (a.e[i].c != v) {
        func_0888A2B4(-1, 0);
    }
}
