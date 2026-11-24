/* Calls func_08B51770(rec->8, rec->0), func_0892F5F8(func_08B51968(rec->8), func_08B36490(self) + 0x25C), func_088A18B0(D_089D6C88, 0x1E, (int)D_089C9F54). */
struct Rec { int a; int pad; int b; };
struct Self { char pad[0xC8]; struct Rec *rec; };
extern float D_089C9F54;
extern void *D_089D6C88;
extern void func_08B51770(int b, int a);
extern int func_08B36490(struct Self *self);
extern int func_08B51968(int b);
extern void func_0892F5F8(int h, int p);
extern void func_088A18B0(void *p, int n, int v);
void func_08B398CC(struct Self *self)
{
    int t;
    func_08B51770(self->rec->b, self->rec->a);
    t = func_08B36490(self);
    func_0892F5F8(func_08B51968(self->rec->b), t + 0x25C);
    func_088A18B0(D_089D6C88, 0x1E, (int)D_089C9F54);
}
